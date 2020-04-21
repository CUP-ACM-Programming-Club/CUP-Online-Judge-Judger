//
// Created by Haoyuan Li on 2020/4/22.
//

#ifndef JUDGE_LIBRARY_KOTLIN_H
#define JUDGE_LIBRARY_KOTLIN_H

#include "Java.h"

class Kotlin : public Java {
public:
    void run(int memory) override;
    void compile(std::vector<std::string>&, const char*, const char*) override;
    std::string getFileSuffix() override;
    bool supportParallel() override;
};


#endif //JUDGE_LIBRARY_KOTLIN_H
