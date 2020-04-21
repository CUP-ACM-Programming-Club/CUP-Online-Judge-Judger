//
// Created by Haoyuan Li on 2020/4/21.
//

#include "Kotlin.h"
#include <unistd.h>
#include <seccomp.h>
#include "util/util.h"
#include "common/seccomp_helper.h"
#ifdef __i386
#include "syscall/kotlin/syscall32.h"
#else
#include "syscall/kotlin/syscall64.h"
#endif

#define SYSCALL_ARRAY LANG_KTV

std::string Kotlin::getFileSuffix() {
    return "kt";
}

void Kotlin::run(int memory) {
    execl("./Main.kexe", "./Main.kexe", (char*) nullptr);
}

void Kotlin::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
}

void Kotlin::initCallCounter(int *call_counter) {
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
    }
}

void Kotlin::setProcessLimit() {
    Language::setProcessLimit();
}

double Kotlin::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Kotlin::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Kotlin::buildSeccompSandbox() {
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

bool Kotlin::gotErrorWhileRunning(bool error) {
    return error;
}

extlang createInstancekotlin () {
    return new Kotlin;
}

deslang destroyInstancekotlin (Language* language) {
    delete language;
}