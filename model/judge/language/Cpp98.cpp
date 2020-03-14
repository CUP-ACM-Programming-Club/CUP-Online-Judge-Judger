//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Cpp98.h"

extlang createInstancecpp98 () {
    return new Cpp98;
}

deslang destroyInstancecpp98 (Language* language) {
    delete language;
}