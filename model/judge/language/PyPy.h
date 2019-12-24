//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PYPY_H
#define JUDGE_CLIENT_PYPY_H


#include "Language.h"

class PyPy : public Language{
public:
    void run(int memory);
};


#endif //JUDGE_CLIENT_PYPY_H
