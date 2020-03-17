//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_JAVA_H
#define JUDGE_CLIENT_JAVA_H


#include "Language.h"
#include "common/BonusLimit.h"

class Java : public Language, protected BonusLimit {
public:
    void run(int memory);
    void setProcessLimit();
    void setCompileProcessLimit();
    void compile(std::vector<std::string>&, const char*, const char*);
    void buildRuntime(const char* work_dir);
    double buildTimeLimit(double timeLimit, double bonus) override;
    int buildMemoryLimit(int memoryLimit, int bonus) override;
    void setExtraPolicy(const char* oj_home, const char* work_dir) override;
    void initCallCounter(int* call_counter) override;
    void setCompileMount(const char* work_dir) override;
    std::string getFileSuffix() override;
private:
    void setAlarm();
    void setASLimit();
};


#endif //JUDGE_CLIENT_JAVA_H
