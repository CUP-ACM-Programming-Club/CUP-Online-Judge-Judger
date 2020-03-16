//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Lua.h"
#include "unistd.h"
#include "util/util.h"

void Lua::run(int memory) {
    execl("/lua", "/lua", "Main", (char *) nullptr);
}

void Lua::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/bin", work_dir);
    execute_cmd("/bin/cp /usr/bin/lua %s/", work_dir);
}

double Lua::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Lua::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}


extlang createInstancelua() {
    return new Lua;
}

deslang destroyInstancelua (Language* language) {
    delete language;
}