//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Python2.h"
#include "unistd.h"
void Python2::run(int memory) {
    execl("/python2", "/python2", "Main.py", (char *) nullptr);
}
