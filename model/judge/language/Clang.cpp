//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Clang.h"

extern "C" Language* createInstanceclang() {
    return new Clang;
}

extern "C" void destroyInstanceclang(Language* language) {
    delete language;
}