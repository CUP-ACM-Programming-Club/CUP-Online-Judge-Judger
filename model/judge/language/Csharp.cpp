//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Csharp.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/csharp/syscall32.h"
#else
#include "syscall/csharp/syscall64.h"
#endif
using std::memset;

#include "seccomp.h"
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_CSV

void Csharp::run(int memory) {
    execv(args[0], args);
}

void Csharp::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}

void Csharp::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir %s/usr", work_dir);
    execute_cmd("/bin/mkdir %s/proc", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/lib/mono/2.0", work_dir);
    execute_cmd("/bin/cp -a /usr/lib/mono %s/usr/lib/", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/lib64/mono/2.0", work_dir);
    execute_cmd("/bin/cp -a /usr/lib64/mono %s/usr/lib64/", work_dir);

    execute_cmd("/bin/cp /usr/lib/libgthread* %s/usr/lib/", work_dir);

    execute_cmd("/bin/mount -o bind /proc %s/proc", work_dir);
    execute_cmd("/bin/cp /usr/bin/mono* %s/", work_dir);

    execute_cmd("/bin/cp /usr/lib/libgthread* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /lib/libglib* %s/lib/", work_dir);
    execute_cmd("/bin/cp /lib/tls/i686/cmov/lib* %s/lib/tls/i686/cmov/",
                work_dir);
    execute_cmd("/bin/cp /lib/libpcre* %s/lib/", work_dir);
    execute_cmd("/bin/cp /lib/ld-linux* %s/lib/", work_dir);
    execute_cmd("/bin/cp /lib64/ld-linux* %s/lib64/", work_dir);
    execute_cmd("/bin/mkdir -p %s/home/judge", work_dir);
    execute_cmd("/bin/chown judge %s/home/judge", work_dir);
    execute_cmd("/bin/mkdir -p %s/etc", work_dir);
    execute_cmd("/bin/grep judge /etc/passwd>%s/etc/passwd", work_dir);
}

double Csharp::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Csharp::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Csharp::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++)
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
}

std::string Csharp::getFileSuffix() {
    return "cs";
}

bool Csharp::gotErrorWhileRunning(bool error) {
    return error;
}

void Csharp::buildSeccompSandbox() {
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

char **Csharp::getArgs() {
    return args;
}

extlang createInstancecsharp () {
    return new Csharp;
}

deslang destroyInstancecsharp (Language* language) {
    delete language;
}