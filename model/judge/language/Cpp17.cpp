//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Cpp17.h"

extlang createInstancecpp17 () {
    return new Cpp17;
}

deslang destroyInstancecpp17 (Language* language) {
    delete language;
}