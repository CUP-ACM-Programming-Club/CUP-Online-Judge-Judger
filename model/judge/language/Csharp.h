//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_CSHARP_H
#define JUDGE_CLIENT_CSHARP_H


#include "Language.h"

class Csharp : public Language {
public:
    void run(int memory);
    void setProcessLimit();
};


#endif //JUDGE_CLIENT_CSHARP_H
