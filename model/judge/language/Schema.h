//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_SCHEMA_H
#define JUDGE_CLIENT_SCHEMA_H


#include "Language.h"

class Schema : public Language {
public:
    void run(int memory);
    void setProcessLimit();
};


#endif //JUDGE_CLIENT_SCHEMA_H
