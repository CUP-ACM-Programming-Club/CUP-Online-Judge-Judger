//
// Created by Haoyuan Li on 2019/12/24.
//

#include "FreeBasic.h"
extlang createInstancefreebasic () {
    return new FreeBasic;
}

deslang destroyInstancefreebasic (Language* language) {
    delete language;
}