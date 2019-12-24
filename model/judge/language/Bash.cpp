//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Bash.h"
#include "unistd.h"

void Bash::run(int memory) {
    execl("/bin/bash", "/bin/bash", "Main.sh", (char *) nullptr);
}

void Bash::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_max = LIM.rlim_cur = 3;
    setrlimit(RLIMIT_NPROC, &LIM);
}
