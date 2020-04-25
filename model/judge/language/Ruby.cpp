//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Ruby.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/ruby/syscall32.h"
#else
#include "syscall/ruby/syscall64.h"
#endif

#include <seccomp.h>
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_RV

using std::memset;
void Ruby::run(int memory) {
    execv(args[0], args);
}

void Ruby::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}
extlang createInstanceruby() {
    return new Ruby;
}

deslang destroyInstanceruby (Language* language) {
    delete language;
}

void Ruby::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("mkdir -p %s/usr", work_dir);
    execute_cmd("mkdir -p %s/usr/lib", work_dir);
    execute_cmd("mkdir -p %s/usr/lib64", work_dir);
    execute_cmd("cp -a /usr/lib/libruby* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib/ruby* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib64/ruby* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libruby* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/bin/ruby* %s/", work_dir);
}

double Ruby::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Ruby::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Ruby::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; ++i) {
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
    }
}

int Ruby::getCompileResult(int status) {
    return static_cast<int>(get_file_size("ce.txt"));
}

std::string Ruby::getFileSuffix() {
    return "rb";
}

int Ruby::getMemory(rusage ruse, pid_t pid) {
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

void Ruby::buildSeccompSandbox() {
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

char **Ruby::getArgs() {
    return args;
}
