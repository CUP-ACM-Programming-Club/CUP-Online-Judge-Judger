//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Python3.h"
#include "unistd.h"
#include <cstring>
#include <iostream>
#include <fstream>
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

int Python3::fixACStatus(int acFlag) {
    std::cerr << "Try to get sizeof error.out" << std::endl;
    auto error_size = get_file_size("error.out");
    std::cerr << "Error size:" << error_size << std::endl;
    if (error_size > 0) {
        std::fstream ferr("error.out");
        std::string tmp, content;
        while (getline(ferr, tmp)) {
            content += tmp;
        }
        if (content.find("SyntaxError") != content.npos) {
            return RUNTIME_ERROR;
        }
        return acFlag;
    }
    return acFlag;
}

int Python3::getMemory(rusage ruse, pid_t pid) {
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

extlang createInstancepython3() {
    return new Python3;
}

deslang destroyInstancepython3 (Language* language) {
    delete language;
}