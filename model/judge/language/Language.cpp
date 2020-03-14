//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Language.h"
#include <iostream>

void Language::setProcessLimit() {
    std::cout << "Set limit from Language" << std::endl;
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 1;
    setrlimit(RLIMIT_NPROC, &LIM);
}

