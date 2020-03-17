//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_CLANGPP_H
#define JUDGE_CLIENT_CLANGPP_H


#include "C11.h"

class Clangpp : public C11{
    std::string getFileSuffix() override;
};


#endif //JUDGE_CLIENT_CLANGPP_H
