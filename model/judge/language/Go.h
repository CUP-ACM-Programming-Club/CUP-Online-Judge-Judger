//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_GO_H
#define JUDGE_CLIENT_GO_H


#include "C11.h"
#include "common/BonusLimit.h"

class Go : public C11, protected BonusLimit {
public:
    void setProcessLimit() override;
    void setCompileProcessLimit() override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    std::string getFileSuffix() override;
    void runMemoryLimit(rlimit& LIM) override;
    bool enableSim() override;
    bool gotErrorWhileRunning(bool error) override;
private:
    void setASLimit() override;
};


#endif //JUDGE_CLIENT_GO_H
