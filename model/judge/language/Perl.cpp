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

int Perl::getMemory(rusage ruse, pid_t pid) {
    const char* mark = "VmPeak:";
    const unsigned BUFFER_SIZE = 5 * (1 << 10);
    FILE *pf;
    char fn[BUFFER_SIZE], buf[BUFFER_SIZE];
    int ret = 0;
    sprintf(fn, "/proc/%d/status", pid);
    pf = fopen(fn, "re");
    auto m = static_cast<int>(strlen(mark));
    while (pf && fgets(buf, BUFFER_SIZE - 1, pf)) {

        buf[strlen(buf) - 1] = 0;
        if (strncmp(buf, mark, m) == 0) {
            sscanf(buf + m + 1, "%d", &ret);
        }
    }
    if (pf)
        fclose(pf);
    return ret << 10;
}

extlang createInstanceperl () {
    return new Perl;
}

deslang destroyInstanceperl (Language* language) {
    delete language;
}
