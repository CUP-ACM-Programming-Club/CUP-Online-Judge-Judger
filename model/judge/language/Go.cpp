//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Go.h"

const int COMPILE_STD_MB = 1 << 20;

void Go::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_max = LIM.rlim_cur = 280;
    setrlimit(RLIMIT_NPROC, &LIM);
}
extlang createInstancego () {
    return new Go;
}

deslang destroyInstancego (Language* language) {
    delete language;
}

void Go::setCompileProcessLimit() {
    Language::setCPULimit();
    Language::setAlarm();
    Language::setFSizeLimit();
    this->setASLimit();
}

void Go::setASLimit() {
    struct rlimit LIM;
    LIM.rlim_max = static_cast<rlim_t>(COMPILE_STD_MB << 11);
    LIM.rlim_cur = static_cast<rlim_t>(COMPILE_STD_MB << 11);
    setrlimit(RLIMIT_AS, &LIM);
}
