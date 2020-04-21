//
// Created by Haoyuan Li on 2020/4/22.
//

#include "Kotlin.h"
#include <unistd.h>
#include <cstring>
#ifdef __i386
#include "syscall/java/syscall32.h"
#else
#include "syscall/java/syscall64.h"
#endif
#include "common/seccomp_helper.h"
#include <seccomp.h>

using std::memset;

#define SYSCALL_ARRAY LANG_JV

std::string Kotlin::getFileSuffix() {
    return "kt";
}

void Kotlin::run(int memory) {
    char java_xms[1 << 7];
    char java_xmx[1 << 7];
    sprintf(java_xms, "-Xmx%dM", memory);
    sprintf(java_xmx, "-XX:MaxMetaspaceSize=%dM", memory);
    execl("/usr/bin/java", "/usr/bin/java", "-jar", java_xms, java_xmx,
          "-Djava.security.manager",
          "-Djava.security.policy=./java.policy", "Main.jar", (char *) nullptr);
}

void Kotlin::compile(std::vector<std::string>& _args, const char* java_xms, const char* java_xmx) {
    Language::compile(_args, java_xms, java_xmx);
}

void Kotlin::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++)
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
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

extlang createInstancekotlin () {
    return new Kotlin;
}

deslang destroyInstancekotlin (Language* language) {
    delete language;
}
