//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PYPY3_H
#define JUDGE_CLIENT_PYPY3_H


#include "Language.h"

class PyPy3 : public Language{
public:
    void run(int memory);
    void buildRuntime(const char* work_dir);
};


#endif //JUDGE_CLIENT_PYPY3_H
