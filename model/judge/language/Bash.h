//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_BASH_H
#define JUDGE_CLIENT_BASH_H


#include "Language.h"
#include "common/BonusLimit.h"

class Bash : public Language, protected BonusLimit {
public:
    void run(int memory);
    void setProcessLimit();
    void buildRuntime(const char* work_dir) override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    void buildSeccompSandbox() override;
    int getCompileResult(int status) override;
    std::string getFileSuffix() override;
    int getMemory(rusage ruse, pid_t pid) override;
    bool gotErrorWhileRunning(bool error) override;
};


#endif //JUDGE_CLIENT_BASH_H
