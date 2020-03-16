//
// Created by Haoyuan Li on 2019/12/24.
//

#include "C11.h"
#include <unistd.h>
#include <iostream>
void C11::run(int memory) {
    execl("./Main", "./Main", (char *) nullptr);
}

void C11::buildRuntime(const char *work_dir) {
    // do nothing
}

extern "C" Language* createInstancec11() {
    return new C11;
}

extern "C" void destroyInstancec11(Language* language) {
    delete language;
}
