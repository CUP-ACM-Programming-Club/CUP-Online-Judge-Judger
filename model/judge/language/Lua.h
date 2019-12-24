//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_LUA_H
#define JUDGE_CLIENT_LUA_H


#include "Language.h"

class Lua : public Language{
public:
    void run(int memory);
};


#endif //JUDGE_CLIENT_LUA_H
