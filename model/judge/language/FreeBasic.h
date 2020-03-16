//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_FREEBASIC_H
#define JUDGE_CLIENT_FREEBASIC_H


#include "C11.h"
#include "common/BonusLimit.h"

class FreeBasic : public C11, protected BonusLimit {
public:
    void buildRuntime(const char* work_dir);
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int timeLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    void setCompileExtraConfig() override;
    void setCompileMount(const char* work_dir) override;
};


#endif //JUDGE_CLIENT_FREEBASIC_H
