//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PYTHON3_H
#define JUDGE_CLIENT_PYTHON3_H


#include "Language.h"
#include "common/CPython.h"
#include "common/BonusLimit.h"

class Python3 : public Language, public CPython, protected BonusLimit {
public:
    void run(int memory);
    void buildRuntime(const char* work_dir) override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    int getCompileResult(int status) override;
    std::string getFileSuffix() override;
    int fixACStatus(int acFlag) override;
    int getMemory(rusage ruse, pid_t pid) override;
    void fixACFlag(int& ACflg) override;
    void fixFlagWithVMIssue(char *work_dir, int &ACflg, int &topmemory,
                            int mem_lmt) override;
};


#endif //JUDGE_CLIENT_PYTHON3_H
