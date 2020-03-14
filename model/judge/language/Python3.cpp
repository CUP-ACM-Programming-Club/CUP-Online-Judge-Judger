//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Python3.h"
#include "unistd.h"
void Python3::run(int memory) {
    execl("/python3", "/python3", "Main.py", (char *) nullptr);
}

extlang createInstancepython3() {
    return new Python3;
}

deslang destroyInstancepython3 (Language* language) {
    delete language;
}