//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Clangpp.h"
extern "C" Language* createInstanceclangpp () {
    return new Clangpp;
}

extern "C" void destroyInstanceclangpp (Language* language) {
    delete language;
}

std::string Clangpp::getFileSuffix() {
    return "cc";
}
