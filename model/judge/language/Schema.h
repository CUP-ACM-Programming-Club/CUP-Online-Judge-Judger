//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_SCHEMA_H
#define JUDGE_CLIENT_SCHEMA_H


#include "Language.h"
#include "common/BonusLimit.h"

class Schema : public Language, protected BonusLimit {
public:
    void run(int memory);
    void setProcessLimit();
    void buildRuntime(const char* work_dir);
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    std::string getFileSuffix() override;
    int getMemory(rusage ruse, pid_t pid) override;
};


#endif //JUDGE_CLIENT_SCHEMA_H
