//
// Created by Haoyuan Li on 2019/12/24.
//

#include "PyPy.h"
#include "unistd.h"
void PyPy::run(int memory) {
    execl("/pypy/bin/pypy", "/pypy/bin/pypy", "Main.py", (char *) nullptr);
}
