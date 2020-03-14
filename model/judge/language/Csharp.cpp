//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Csharp.h"
#include "unistd.h"

void Csharp::run(int memory) {
    execl("/mono", "/mono", "--debug", "Main.exe", (char *) nullptr);
}

void Csharp::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}
extlang createInstancecsharp () {
    return new Csharp;
}

deslang destroyInstancecsharp (Language* language) {
    delete language;
}