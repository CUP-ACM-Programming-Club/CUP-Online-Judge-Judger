//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Go.h"

void Go::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_max = LIM.rlim_cur = 280;
    setrlimit(RLIMIT_NPROC, &LIM);
}
