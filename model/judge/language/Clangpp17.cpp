//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Clangpp17.h"

extern "C" Language* createInstanceclangpp17() {
    return new Clangpp17;
}

extern "C" void destroyInstanceclangpp17(Language* language) {
    delete language;
}