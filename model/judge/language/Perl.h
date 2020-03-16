//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PERL_H
#define JUDGE_CLIENT_PERL_H


#include "Language.h"

class Perl : public Language {
public:
    void run(int memory);
    void buildRuntime(const char* work_dir) override;
};


#endif //JUDGE_CLIENT_PERL_H
