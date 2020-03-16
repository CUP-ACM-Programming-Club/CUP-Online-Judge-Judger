//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Python3.h"
#include "unistd.h"
void Python3::run(int memory) {
    execl("/python3", "/python3", "Main.py", (char *) nullptr);
}

void Python3::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    CPython::buildRuntime(work_dir);
}

double Python3::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Python3::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

extlang createInstancepython3() {
    return new Python3;
}

deslang destroyInstancepython3 (Language* language) {
    delete language;
}