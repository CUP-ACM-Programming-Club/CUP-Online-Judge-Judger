//
// Created by Haoyuan Li on 2019/12/24.
//

#include "FreeBasic.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/freebasic/syscall32.h"
#else
#include "syscall/freebasic/syscall64.h"
#endif

extlang createInstancefreebasic () {
    return new FreeBasic;
}

deslang destroyInstancefreebasic (Language* language) {
    delete language;
}

void FreeBasic::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/bin", work_dir);
    execute_cmd("/bin/cp /usr/local/lib/freebasic %s/usr/local/lib/", work_dir);
    execute_cmd("/bin/cp /usr/local/bin/fbc %s/", work_dir);
    execute_cmd("/bin/cp -a /lib32/* %s/lib/", work_dir);
}

double FreeBasic::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int FreeBasic::buildMemoryLimit(int timeLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(timeLimit, bonus);
}

void FreeBasic::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || LANG_BASICV[i]; i++)
        call_counter[LANG_BASICV[i]] = HOJ_MAX_LIMIT;
}

void FreeBasic::setCompileExtraConfig() {
    freopen("ce.txt", "w", stdout);
}

void FreeBasic::setCompileMount(const char* work_dir) {
    // do nothing
}

std::string FreeBasic::getFileSuffix() {
    return "bas";
}

int FreeBasic::getMemory(rusage ruse, pid_t pid) {
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
