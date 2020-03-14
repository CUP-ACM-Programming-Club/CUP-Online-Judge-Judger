//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Pascal.h"
extlang createInstancepascal() {
    return new Pascal;
}

deslang destroyInstancepascal(Language* language) {
    delete language;
}
