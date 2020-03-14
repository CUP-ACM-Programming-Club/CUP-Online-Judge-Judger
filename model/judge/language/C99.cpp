//
// Created by Haoyuan Li on 2019/12/24.
//

#include "C99.h"
extern "C" Language* createInstancec99() {
    return new C99;
}

extern "C" void destroyInstancec99(Language* language) {
    delete language;
}