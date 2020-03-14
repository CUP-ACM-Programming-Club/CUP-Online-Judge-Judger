//
// Created by Haoyuan Li on 2019/12/24.
//

#include "PyPy3.h"
#include "unistd.h"
void PyPy3::run(int memory) {
    execl("/pypy3/bin/pypy3", "/pypy3/bin/pypy3", "Main.py", (char *) nullptr);
}

extlang createInstancepypy3() {
    return new PyPy3;
}

deslang destroyInstancepypy3(Language* language) {
    delete language;
}