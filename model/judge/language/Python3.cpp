//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Python3.h"
#include "unistd.h"
#include <cstring>
#ifdef __i386
#include "syscall/python3/syscall32.h"
#else
#include "syscall/python3/syscall64.h"
#include "util/util.h"

#endif
using std::memset;
void Python3::run(int memory) {
    execl("/python3", "/python3", "Main.py", (char *) nullptr);
}

void Python3::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    CPython::buildRuntime(work_dir);
}

double Python3::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Python3::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Python3::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || LANG_PY3V[i]; ++i) {
        call_counter[LANG_PY3V[i]] = HOJ_MAX_LIMIT;
    }
}

int Python3::getCompileResult(int status) {
    return static_cast<int>(get_file_size("ce.txt"));
}

std::string Python3::getFileSuffix() {
    return "py";
}

extlang createInstancepython3() {
    return new Python3;
}

deslang destroyInstancepython3 (Language* language) {
    delete language;
}