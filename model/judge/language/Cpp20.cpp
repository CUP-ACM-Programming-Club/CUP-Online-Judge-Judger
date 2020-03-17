//
// Created by Haoyuan Li on 2020/3/17.
//

#include "Cpp20.h"

extlang createInstancecpp20 () {
    return new Cpp20;
}

deslang destroyInstancecpp20 (Language* language) {
    delete language;
}

std::string Cpp20::getFileSuffix() {
    return "cc";
}
