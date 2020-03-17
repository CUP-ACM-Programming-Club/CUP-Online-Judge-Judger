//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_CLANGPP17_H
#define JUDGE_CLIENT_CLANGPP17_H


#include "C11.h"

class Clangpp17 : public C11 {
    std::string getFileSuffix() override;
};


#endif //JUDGE_CLIENT_CLANGPP17_H
