//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Go.h"
#include <cstring>
using std::memset;
#ifdef __i386
#include "syscall/go/syscall32.h"
#else
#include "syscall/go/syscall64.h"
#endif

#include <seccomp.h>
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_GOV

const int COMPILE_STD_MB = 1 << 20;

void Go::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_max = LIM.rlim_cur = 280;
    setrlimit(RLIMIT_NPROC, &LIM);
}
extlang createInstancego () {
    return new Go;
}

deslang destroyInstancego (Language* language) {
    delete language;
}

void Go::setCompileProcessLimit() {
    Language::setCPULimit();
    Language::setAlarm();
    Language::setFSizeLimit();
    this->setASLimit();
}

void Go::setASLimit() {
    struct rlimit LIM;
    LIM.rlim_max = static_cast<rlim_t>(COMPILE_STD_MB << 11);
    LIM.rlim_cur = static_cast<rlim_t>(COMPILE_STD_MB << 11);
    setrlimit(RLIMIT_AS, &LIM);
}

double Go::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Go::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Go::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++)
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
}

std::string Go::getFileSuffix() {
    return "go";
}

void Go::runMemoryLimit(rlimit &LIM) {
    Language::runMemoryLimit(LIM);
}

bool Go::enableSim() {
    return Language::enableSim();
}

bool Go::gotErrorWhileRunning(bool error) {
    return Language::gotErrorWhileRunning(error);
}

void Go::buildSeccompSandbox() {
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
