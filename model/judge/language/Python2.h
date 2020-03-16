//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PYTHON2_H
#define JUDGE_CLIENT_PYTHON2_H


#include "Language.h"
#include "common/CPython.h"
#include "common/BonusLimit.h"

class Python2 : public Language, public CPython, protected BonusLimit {
public:
    void run(int memory);
    void buildRuntime(const char* work_dir) override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
};


#endif //JUDGE_CLIENT_PYTHON2_H
