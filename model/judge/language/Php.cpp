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
using std::memset;

void Php::run(int memory) {
    execl("/php", "/php", "Main.php", (char *) nullptr);
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
    memset(call_counter, 0, sizeof(call_counter));
    for (int i = 0; i == 0 || LANG_PHV[i]; i++)
        call_counter[LANG_PHV[i]] = HOJ_MAX_LIMIT;
}

extlang createInstancephp() {
    return new Php;
}

deslang destroyInstancephp(Language* language) {
    delete language;
}
