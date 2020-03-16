//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Python2.h"
#include "unistd.h"
void Python2::run(int memory) {
    execl("/python2", "/python2", "Main.py", (char *) nullptr);
}

void Python2::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    CPython::buildRuntime(work_dir);
}

double Python2::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Python2::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

extlang createInstancepython2() {
    return new Python2;
}

deslang destroyInstancepython2(Language* language) {
    delete language;
}

