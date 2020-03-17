//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PASCAL_H
#define JUDGE_CLIENT_PASCAL_H


#include "C11.h"

class Pascal : public C11 {
public:
    void buildRuntime(const char* work_dir) override;
    void initCallCounter(int* call_counter) override;
    void setCompileExtraConfig();
    std::string getFileSuffix() override;
    int getMemory(rusage ruse, pid_t pid) override;
};


#endif //JUDGE_CLIENT_PASCAL_H
