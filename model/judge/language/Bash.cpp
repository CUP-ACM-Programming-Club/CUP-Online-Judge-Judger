//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Bash.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/bash/syscall32.h"
#else
#include "syscall/bash/syscall64.h"
#endif

#include "seccomp.h"
#include "common/seccomp_helper.h"

using std::memset;

#define SYSCALL_ARRAY LANG_BV

void Bash::run(int memory) {
    execv(args[0], args);
}

void Bash::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_max = LIM.rlim_cur = 3;
    setrlimit(RLIMIT_NPROC, &LIM);
}

void Bash::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/cp `which bc`  %s/bin/", work_dir);
    execute_cmd("busybox dos2unix Main.sh", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/grep", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/awk", work_dir);
    execute_cmd("/bin/cp /bin/sed %s/bin/sed", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/cut", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/sort", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/join", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/wc", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/tr", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/dc", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/dd", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/cat", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/tail", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/head", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/xargs", work_dir);
    execute_cmd("chmod +rx %s/Main.sh", work_dir);
}

double Bash::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Bash::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Bash::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for(int i = 0; i == 0 || SYSCALL_ARRAY[i]; ++i) {
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
    }
}

int Bash::getCompileResult(int status) {
    return static_cast<int>(get_file_size("ce.txt"));
}

std::string Bash::getFileSuffix() {
    return "sh";
}

int Bash::getMemory(rusage ruse, pid_t pid) {
    const char* mark = "VmPeak:";
    const unsigned BUFFER_SIZE = 5 * (1 << 10);
    FILE *pf;
    char fn[BUFFER_SIZE], buf[BUFFER_SIZE];
    int ret = 0;
    sprintf(fn, "/proc/%d/status", pid);
    pf = fopen(fn, "re");
    auto m = static_cast<int>(strlen(mark));
    while (pf && fgets(buf, BUFFER_SIZE - 1, pf)) {

        buf[strlen(buf) - 1] = 0;
        if (strncmp(buf, mark, m) == 0) {
            sscanf(buf + m + 1, "%d", &ret);
        }
    }
    if (pf)
        fclose(pf);
    return ret << 10;
}

bool Bash::gotErrorWhileRunning(bool error) {
    return error;
}

void Bash::buildSeccompSandbox() {
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_KILL);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        if (SYSCALL_ARRAY[i] == 59) {
            continue;
        }
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYSCALL_ARRAY[i], 0);
    }
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A1(SCMP_CMP_EQ, (scmp_datum_t)(getArgs())));
    if (install_helper()) {
        printf("install helper failed");
        exit(1);
    }
    seccomp_load(ctx);
}

char **Bash::getArgs() {
    return args;
}

extlang createInstancebash() {
    return new Bash;
}

deslang destroyIntancebash(Language* language) {
    delete language;
}