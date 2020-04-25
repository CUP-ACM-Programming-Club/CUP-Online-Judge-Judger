//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Php.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>

#ifdef __i386
#include "syscall/php/syscall32.h"
#else

#include "syscall/php/syscall64.h"

#endif

#include <seccomp.h>
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_PHV

using std::memset;

void Php::run(int memory) {
    execv(args[0], args);
}

void Php::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir %s/usr", work_dir);
    execute_cmd("/bin/mkdir %s/usr/lib", work_dir);
    execute_cmd("/bin/cp /usr/lib/libedit* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libdb* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libgssapi_krb5* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libkrb5* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libk5crypto* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libedit* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libdb* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libgssapi_krb5* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libkrb5* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libk5crypto* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libxml2* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/x86_64-linux-gnu/libxml2.so* %s/usr/lib/",
                work_dir);
    execute_cmd("/bin/cp /usr/bin/php* %s/", work_dir);
    execute_cmd("chmod +rx %s/Main.php", work_dir);
}

double Php::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Php::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Php::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++)
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
}

std::string Php::getFileSuffix() {
    return "php";
}

void Php::buildSeccompSandbox() {
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_KILL);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        if (SYSCALL_ARRAY[i] == 59) {
            continue;
        }
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYSCALL_ARRAY[i], 0);
    }
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A1(SCMP_CMP_EQ, (scmp_datum_t) (getArgs())));
    if (install_helper()) {
        printf("install helper failed");
        exit(1);
    }
    seccomp_load(ctx);
}

char **Php::getArgs() {
    return args;
}

extlang createInstancephp() {
    return new Php;
}

deslang destroyInstancephp(Language *language) {
    delete language;
}
