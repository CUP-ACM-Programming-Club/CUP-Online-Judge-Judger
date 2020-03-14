//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Clang11.h"
extern "C" Language* createInstanceclang11 () {
    return new Clang11;
}

extern "C" void destroyInstanceclang11 (Language* language) {
    delete language;
}