//
// Created by Haoyuan Li on 2020/3/26.
//

#ifndef COMPARE_COMPAREIMPL_H
#define COMPARE_COMPAREIMPL_H


#include "Compare.h"
#define IGNORE_ESOL

class CompareImpl: public Compare::Compare {
public:
    int compare(const char* stdAnswerFile, const char* userOutputFile) override;
};


#endif //COMPARE_COMPAREIMPL_H
