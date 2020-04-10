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

#include <seccomp.h>
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_PY3V

const int STD_MB = 1 << 20;
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
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; ++i) {
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
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

void Python3::fixACFlag(int &ACflg) {
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
            ACflg = RUNTIME_ERROR;
        }
    }
}

void Python3::fixFlagWithVMIssue(char *work_dir, int &ACflg, int &topmemory, int mem_lmt) {
    int comp_res = execute_cmd(
            "/bin/grep 'MemoryError'  %s/error.out", work_dir);

    if (!comp_res) {
        printf("Python need more Memory!");
        ACflg = MEMORY_LIMIT_EXCEEDED;
        topmemory = mem_lmt * STD_MB;
    }
}

void Python3::buildSeccompSandbox() {
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_TRAP);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYSCALL_ARRAY[i], 0);
    }
    if (install_helper()) {
        printf("install helper failed");
        exit(1);
    }
    seccomp_load(ctx);
}

extlang createInstancepython3() {
    return new Python3;
}

deslang destroyInstancepython3 (Language* language) {
    delete language;
}