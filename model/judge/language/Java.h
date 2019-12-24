//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_JAVA_H
#define JUDGE_CLIENT_JAVA_H


#include "Language.h"

class Java : public Language{
public:
    void run(int memory);
    void setProcessLimit();
};


#endif //JUDGE_CLIENT_JAVA_H
