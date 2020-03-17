//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Pascal.h"
#include <cstring>
#ifdef __i386
#include "syscall/pascal/syscall32.h"
#else
#include "syscall/pascal/syscall64.h"
#endif
using std::memset;
extlang createInstancepascal() {
    return new Pascal;
}

deslang destroyInstancepascal(Language* language) {
    delete language;
}

void Pascal::buildRuntime(const char *work_dir) {
    // do nothing
}

void Pascal::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || LANG_PV[i]; i++)
        call_counter[LANG_PV[i]] = HOJ_MAX_LIMIT;
}

void Pascal::setCompileExtraConfig() {
    freopen("ce.txt", "w", stdout);
}

std::string Pascal::getFileSuffix() {
    return "pas";
}

int Pascal::getMemory(rusage ruse, pid_t pid) {
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
