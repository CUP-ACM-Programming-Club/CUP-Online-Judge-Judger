//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Language.h"

void Language::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 1;
    setrlimit(RLIMIT_NPROC, &LIM);
}

void Language::run(int memory) {

}
