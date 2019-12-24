//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Ruby.h"
#include "unistd.h"

void Ruby::run(int memory) {
    execl("/ruby", "/ruby", "Main.rb", (char *) nullptr);
}

void Ruby::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}
