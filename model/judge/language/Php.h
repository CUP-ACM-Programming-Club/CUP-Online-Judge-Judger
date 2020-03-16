//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_PHP_H
#define JUDGE_CLIENT_PHP_H


#include "Language.h"

class Php : public Language {
public:
    void run(int memory);
    void buildRuntime(const char* work_dir) override;
};


#endif //JUDGE_CLIENT_PHP_H
