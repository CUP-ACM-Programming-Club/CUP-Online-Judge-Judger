//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_CPP98_H
#define JUDGE_CLIENT_CPP98_H


#include "C11.h"

class Cpp98 : public C11 {
    std::string getFileSuffix() override;
};


#endif //JUDGE_CLIENT_CPP98_H
