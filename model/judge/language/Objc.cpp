//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Objc.h"
extlang createInstanceobjectivec() {
    return new Objc;
}

deslang destroyInstanceobjectivec(Language* language) {
    delete language;
}

