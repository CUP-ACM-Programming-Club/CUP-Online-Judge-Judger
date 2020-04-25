//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PERL_H
#define JUDGE_CLIENT_PERL_H


#include "Language.h"
#include "common/BonusLimit.h"

class Perl : public Language, protected BonusLimit {
public:
    void run(int memory);
    void buildRuntime(const char* work_dir) override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    void buildSeccompSandbox() override;
    std::string getFileSuffix() override;
    int getMemory(rusage ruse, pid_t pid) override;
protected:
    char** getArgs() override;
private:
    char** args = (char*[]){"/perl", "Main.pl", (char *) nullptr};
};


#endif //JUDGE_CLIENT_PERL_H
