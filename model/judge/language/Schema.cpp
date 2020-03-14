//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Schema.h"
#include "unistd.h"
void Schema::run(int memory) {
    execl("/guile", "/guile", "Main.scm", (char *) nullptr);
}

void Schema::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}

extlang createInstanceschema () {
    return new Schema;
}

deslang destroyInstanceschema (Language* language) {
    delete language;
}