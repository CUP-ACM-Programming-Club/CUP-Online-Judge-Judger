//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_JAVA_H
#define JUDGE_CLIENT_JAVA_H


#include "Language.h"

class Java : public Language{
public:
    void run(int memory);
    void setProcessLimit();
    void setCompileProcessLimit();
    void compile(std::vector<std::string>&, const char*, const char*);
    void buildRuntime(const char* work_dir);
private:
    void setAlarm();
    void setASLimit();
};


#endif //JUDGE_CLIENT_JAVA_H
