//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_C11_H
#define JUDGE_CLIENT_C11_H


#include "Language.h"

class C11 : public Language{
public:
    void run(int memory);
    void buildRuntime(const char* work_dir) override;
    void initCallCounter(int* call_counter) override;

};


#endif //JUDGE_CLIENT_C11_H
