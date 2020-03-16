//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_BASH_H
#define JUDGE_CLIENT_BASH_H


#include "Language.h"

class Bash : public Language {
public:
    void run(int memory);
    void setProcessLimit();
    void buildRuntime(const char* work_dir) override;
};


#endif //JUDGE_CLIENT_BASH_H
