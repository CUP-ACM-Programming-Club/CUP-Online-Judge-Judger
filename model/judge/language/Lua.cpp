//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Lua.h"
#include "unistd.h"
void Lua::run(int memory) {
    execl("/lua", "/lua", "Main", (char *) nullptr);
}

extlang createInstancelua() {
    return new Lua;
}

deslang destroyInstancelua (Language* language) {
    delete language;
}