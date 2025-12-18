//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_JAVA_H
#define JUDGE_CLIENT_JAVA_H


#include "Language.h"
#include "common/BonusLimit.h"

class Java : public Language, protected BonusLimit {
public:
    void run(int memory) override;
    void setProcessLimit() override;
    void setCompileProcessLimit() override;
    void compile(std::vector<std::string>&, const char*, const char*) override;
    void buildRuntime(const char* work_dir) override;
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void setExtraPolicy(const char* oj_home, const char* work_dir) override;
    void initCallCounter(int* call_counter) override;
    void buildSeccompSandbox() override;
    void setCompileMount(const char* work_dir) override;
    void buildChrootSandbox(const char* work_dir) override;
    std::string getFileSuffix() override;
    bool enableSim() override;
    void fixFlagWithVMIssue(char *work_dir, int &ACflg, int &topmemory,
                            int mem_lmt) override;
    bool gotErrorWhileRunning(bool error) override;
    bool isValidExitCode(int exitcode) override;
protected:
    char** getArgs() override;
private:
    void setAlarm() override;
    void setASLimit() override;
    char **args = (char*[]){"/usr/bin/java", "", "",
                                    "-Djava.security.manager",
                                    "-Djava.security.policy=./java.policy", "Main", (char *) nullptr};
};


#endif //JUDGE_CLIENT_JAVA_H
