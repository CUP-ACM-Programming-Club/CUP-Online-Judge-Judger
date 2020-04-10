//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_OBJC_H
#define JUDGE_CLIENT_OBJC_H


#include "C11.h"

class Objc : public C11 {
public:
    void buildRuntime(const char* work_dir) override;
    void initCallCounter(int* call_counter) override;
    void buildSeccompSandbox() override;
    std::string getFileSuffix() override;
    int getMemory(rusage ruse, pid_t pid) override;
};


#endif //JUDGE_CLIENT_OBJC_H
