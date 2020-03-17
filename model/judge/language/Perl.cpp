//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Perl.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/perl/syscall32.h"
#else
#include "syscall/perl/syscall64.h"
#endif
using std::memset;

void Perl::run(int memory) {
    execl("/perl", "/perl", "Main.pl", (char *) nullptr);
}

void Perl::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir %s/usr", work_dir);
    execute_cmd("/bin/mkdir %s/usr/lib", work_dir);
    execute_cmd("/bin/cp /usr/lib/libperl* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/bin/perl* %s/", work_dir);
}

double Perl::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Perl::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Perl::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || LANG_PLV[i]; i++)
        call_counter[LANG_PLV[i]] = HOJ_MAX_LIMIT;
}

std::string Perl::getFileSuffix() {
    return "pl";
}

extlang createInstanceperl () {
    return new Perl;
}

deslang destroyInstanceperl (Language* language) {
    delete language;
}
