//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "JavaScript.h"
#include "unistd.h"
void JavaScript::run(int memory) {
    execl("/node", "/node", "Main.js", (char *) nullptr);
}

void JavaScript::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}
extlang createInstancejavascript() {
    return new JavaScript;
}

deslang destroyInstancejavascript(Language* language) {
    delete language;
}