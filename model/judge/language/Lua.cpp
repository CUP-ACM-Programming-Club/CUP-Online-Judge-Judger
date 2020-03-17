//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Lua.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/lua/syscall32.h"
#else
#include "syscall/lua/syscall64.h"
#endif
using std::memset;

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

void Lua::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || LANG_LUAV[i]; i++)
        call_counter[LANG_LUAV[i]] = HOJ_MAX_LIMIT;
}

std::string Lua::getFileSuffix() {
    return "lua";
}


extlang createInstancelua() {
    return new Lua;
}

deslang destroyInstancelua (Language* language) {
    delete language;
}