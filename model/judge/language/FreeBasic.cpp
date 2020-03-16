//
// Created by Haoyuan Li on 2019/12/24.
//

#include "FreeBasic.h"
#include "util/util.h"

extlang createInstancefreebasic () {
    return new FreeBasic;
}

deslang destroyInstancefreebasic (Language* language) {
    delete language;
}

void FreeBasic::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/bin", work_dir);
    execute_cmd("/bin/cp /usr/local/lib/freebasic %s/usr/local/lib/", work_dir);
    execute_cmd("/bin/cp /usr/local/bin/fbc %s/", work_dir);
    execute_cmd("/bin/cp -a /lib32/* %s/lib/", work_dir);
}

double FreeBasic::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int FreeBasic::buildMemoryLimit(int timeLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(timeLimit, bonus);
}
