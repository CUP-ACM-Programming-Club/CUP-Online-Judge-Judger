//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PYPY3_H
#define JUDGE_CLIENT_PYPY3_H


#include "Language.h"
#include "common/BonusLimit.h"

class PyPy3 : public Language, protected BonusLimit {
public:
    void run(int memory);
    void buildRuntime(const char* work_dir);
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
};


#endif //JUDGE_CLIENT_PYPY3_H
