//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PYTHON2_H
#define JUDGE_CLIENT_PYTHON2_H


#include "Language.h"
#include "common/CPython.h"

class Python2 : public Language, public CPython{
public:
    void run(int memory);
    void buildRuntime(const char* work_dir) override;
};


#endif //JUDGE_CLIENT_PYTHON2_H
