//
// Created by Haoyuan Li on 2020/4/21.
//

#ifndef JUDGE_LIBRARY_KOTLINNATIVE_H
#define JUDGE_LIBRARY_KOTLINNATIVE_H


#include "Language.h"
#include "common/BonusLimit.h"

class KotlinNative : public Language, BonusLimit {
public:
    void run(int memory) override;
    void setProcessLimit() override;
    void buildRuntime(const char* work_dir) override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    void buildSeccompSandbox() override;
    void setCompileProcessLimit() override;
    std::string getFileSuffix() override;
    void setCompileMount(const char* work_dir) override;
    bool gotErrorWhileRunning(bool error) override;
protected:
    void setAlarm() override;
    char** getArgs() override;
private:
    char** args = (char*[]){"./Main.kexe", (char*) nullptr};
};


#endif //JUDGE_LIBRARY_KOTLINNATIVE_H
