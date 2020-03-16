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
