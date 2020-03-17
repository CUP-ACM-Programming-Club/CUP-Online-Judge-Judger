//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Cpp11.h"

extlang createInstancecpp11 () {
    return new Cpp11;
}

deslang destroyInstancecpp11 (Language* language) {
    delete language;
}

std::string Cpp11::getFileSuffix() {
    return "cc";
}
