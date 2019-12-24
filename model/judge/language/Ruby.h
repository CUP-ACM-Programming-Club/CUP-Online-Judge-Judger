//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_RUBY_H
#define JUDGE_CLIENT_RUBY_H


#include "Language.h"

class Ruby : public Language{
public:
    void run(int memory);
    void setProcessLimit();
};


#endif //JUDGE_CLIENT_RUBY_H
