//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_JAVASCRIPT_H
#define JUDGE_CLIENT_JAVASCRIPT_H


#include "Language.h"
#include "common/BonusLimit.h"

class JavaScript : public Language, protected BonusLimit {
public:
    void run(int memory) override;
    void setProcessLimit() override;
    void buildRuntime(const char* work_dir) override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void initCallCounter(int* call_counter) override;
    void buildSeccompSandbox() override;
    void compile(std::vector<std::string>&, const char*, const char*) override;
    std::string getFileSuffix() override;
protected:
    char** getArgs() override;
private:
    char** args = (char*[]) {"/node", "Main.js", (char *) nullptr};
};


#endif //JUDGE_CLIENT_JAVASCRIPT_H
