//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Schema.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/schema/syscall32.h"
#else
#include "syscall/schema/syscall64.h"
#endif

#include <seccomp.h>
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_SV

using std::memset;
void Schema::run(int memory) {
    execv(args[0], args);
}

void Schema::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}

void Schema::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir -p %s/proc", work_dir);
    execute_cmd("/bin/mount -o bind /proc %s/proc", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/lib", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/share", work_dir);
    execute_cmd("/bin/cp -a /usr/share/guile %s/usr/share/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libguile* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libgc* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/i386-linux-gnu/libffi* %s/usr/lib/",
                work_dir);
    execute_cmd("/bin/cp /usr/lib/i386-linux-gnu/libunistring* %s/usr/lib/",
                work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libgmp* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libgmp* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libltdl* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libltdl* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/bin/guile* %s/", work_dir);
}

double Schema::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Schema::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Schema::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; ++i) {
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
    }
}

std::string Schema::getFileSuffix() {
    return "scm";
}

int Schema::getMemory(rusage ruse, pid_t pid) {
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

void Schema::buildSeccompSandbox() {
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_TRAP);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYSCALL_ARRAY[i], 0);
    }
    if (install_helper()) {
        printf("install helper failed");
        exit(1);
    }
    seccomp_load(ctx);
}

char **Schema::getArgs() {
    return args;
}

extlang createInstanceschema () {
    return new Schema;
}

deslang destroyInstanceschema (Language* language) {
    delete language;
}