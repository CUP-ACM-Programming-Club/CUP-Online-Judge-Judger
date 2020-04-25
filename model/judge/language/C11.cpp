//
// Created by Haoyuan Li on 2019/12/24.
//

#include "C11.h"
#include <unistd.h>
#include <cstring>
#ifdef __i386
#include "syscall/c11/syscall32.h"
#else
#include "syscall/c11/syscall64.h"
#endif
#include "seccomp.h"
#include "common/seccomp_helper.h"
using std::memset;

#define SYSCALL_ARRAY LANG_CV

void C11::run(int memory) {
    execv("./Main", args);
}

void C11::buildRuntime(const char *work_dir) {
    // do nothing
}

void C11::initCallCounter(int *call_counter) {
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
    }
}

void C11::setCompileMount(const char *work_dir) {
    // do nothing
}

std::string C11::getFileSuffix() {
    return "c";
}

void C11::runMemoryLimit(rlimit &LIM) {
    setrlimit(RLIMIT_AS, &LIM);
}

bool C11::enableSim() {
    return true;
}

bool C11::gotErrorWhileRunning(bool error) {
    return error;
}

void C11::buildSeccompSandbox() {
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_TRAP);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        if (SYSCALL_ARRAY[i] == 59) {
            continue;
        }
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYSCALL_ARRAY[i], 0);
    }
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t)(args)));
    if (install_helper()) {
        printf("install helper failed");
        exit(1);
    }
    seccomp_load(ctx);
}

extern "C" Language* createInstancec11() {
    return new C11;
}

extern "C" void destroyInstancec11(Language* language) {
    delete language;
}
