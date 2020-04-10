//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "JavaScript.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/javascript/syscall32.h"
#else
#include "syscall/javascript/syscall64.h"
#endif

#include <seccomp.h>
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_JSV

using std::memset;


void JavaScript::run(int memory) {
    execl("/node", "/node", "Main.js", (char *) nullptr);
}

void JavaScript::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}

void JavaScript::buildRuntime(const char *work_dir) {
    execute_cmd("mkdir -p %s/dev", work_dir);
    execute_cmd("/bin/mount -o bind /dev %s/dev", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/lib64 %s/lib64/ %s/lib64/", work_dir, work_dir, work_dir);
    execute_cmd("/bin/cp /lib64/libz.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64libuv.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicui18n.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicuuc.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicudata.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libtinfo.so.*  %s/lib64/", work_dir);

    execute_cmd("/bin/cp /usr/lib64/libcares.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libv8.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib/libssl.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libcrypto.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libdl.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/librt.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libstdc++.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libpthread.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libc.so.6  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libm.so.6  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libgcc_s.so.1  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/ld-linux.so.*  %s/lib64/", work_dir);

    execute_cmd("/bin/mkdir -p %s/usr/lib %s/lib64/", work_dir, work_dir);

    execute_cmd("/bin/cp /lib64/libz.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libuv.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/librt.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libpthread.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libdl.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libssl.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libcrypto.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicui18n.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicuuc.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libstdc++.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libm.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libgcc_s.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libc.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/ld-linux-x86-64.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicudata.so.* %s/lib64/", work_dir);


    execute_cmd("/bin/cp /usr/local/bin/node %s/", work_dir);
}

double JavaScript::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int JavaScript::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void JavaScript::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++)
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
}

std::string JavaScript::getFileSuffix() {
    return "js";
}

void JavaScript::buildSeccompSandbox() {
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

extlang createInstancejavascript() {
    return new JavaScript;
}

deslang destroyInstancejavascript(Language* language) {
    delete language;
}