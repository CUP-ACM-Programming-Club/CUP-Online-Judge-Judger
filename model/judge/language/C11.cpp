//
// Created by Haoyuan Li on 2019/12/24.
//

#include "C11.h"
#include <unistd.h>
#include <cstring>
#ifdef __i386
#include "syscall/c11/syscall32.h"
#else
#include "syscall/c11/syscall64.h"
#endif
using std::memset;
void C11::run(int memory) {
    execl("./Main", "./Main", (char *) nullptr);
}

void C11::buildRuntime(const char *work_dir) {
    // do nothing
}

void C11::initCallCounter(int *call_counter) {
    for (int i = 0; i == 0 || LANG_CV[i]; i++) {
        call_counter[LANG_CV[i]] = HOJ_MAX_LIMIT;
    }
}

void C11::setCompileMount(const char *work_dir) {
    // do nothing
}

std::string C11::getFileSuffix() {
    return "c";
}

void C11::runMemoryLimit(rlimit &LIM) {
    setrlimit(RLIMIT_AS, &LIM);
}

bool C11::enableSim() {
    return true;
}

bool C11::gotErrorWhileRunning(bool error) {
    return error;
}

extern "C" Language* createInstancec11() {
    return new C11;
}

extern "C" void destroyInstancec11(Language* language) {
    delete language;
}
