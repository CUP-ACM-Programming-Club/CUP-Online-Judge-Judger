//
// Created by Haoyuan Li on 2020/4/21.
//

#ifndef JUDGE_LIBRARY_KOTLIN_H
#define JUDGE_LIBRARY_KOTLIN_H


#include "Language.h"
#include "common/BonusLimit.h"

class Kotlin : public Language, BonusLimit {
public:
    void run(int memory) override;
    void setProcessLimit() override;
    void buildRuntime(const char* work_dir) override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    void buildSeccompSandbox() override;
    std::string getFileSuffix() override;
    bool gotErrorWhileRunning(bool error) override;
};


#endif //JUDGE_LIBRARY_KOTLIN_H
