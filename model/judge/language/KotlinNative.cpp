//
// Created by Haoyuan Li on 2020/4/21.
//

#include "KotlinNative.h"
#include <unistd.h>
#include <seccomp.h>
#include "util/util.h"
#include "common/seccomp_helper.h"
#ifdef __i386
#include "syscall/kotlin-native/syscall32.h"
#else
#include "syscall/kotlin-native/syscall64.h"
#endif

#define SYSCALL_ARRAY LANG_KTV

std::string KotlinNative::getFileSuffix() {
    return "kt";
}

void KotlinNative::run(int memory) {
    execl("./Main.kexe", "./Main.kexe", (char*) nullptr);
}

void KotlinNative::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
}

void KotlinNative::initCallCounter(int *call_counter) {
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
    }
}

void KotlinNative::setProcessLimit() {
    Language::setProcessLimit();
}

double KotlinNative::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int KotlinNative::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void KotlinNative::buildSeccompSandbox() {
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

bool KotlinNative::gotErrorWhileRunning(bool error) {
    return error;
}

void KotlinNative::setCompileProcessLimit() {
    Language::setCPULimit();
    this->setAlarm();
    // Language::setFSizeLimit();
}

void KotlinNative::setAlarm() {
    alarm(50);
}

void KotlinNative::setCompileMount(const char *work_dir) {
    // do nothing
}

extlang createInstancekotlinnative () {
    return new KotlinNative;
}

deslang destroyInstancekotlinnative (Language* language) {
    delete language;
}