//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_CSHARP_H
#define JUDGE_CLIENT_CSHARP_H


#include "Language.h"
#include "common/BonusLimit.h"

class Csharp : public Language, protected BonusLimit {
public:
    void run(int memory);
    void setProcessLimit();
    void buildRuntime(const char* work_dir) override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    void buildSeccompSandbox() override;
    std::string getFileSuffix() override;
    bool gotErrorWhileRunning(bool error) override;
protected:
    char** getArgs() override;
private:
    char** args = (char*[]){"/mono", "--debug", "Main.exe", (char *) nullptr};
};


#endif //JUDGE_CLIENT_CSHARP_H
