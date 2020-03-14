//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Java.h"
#include "unistd.h"
#include <cstdio>
#include <sys/resource.h>

using std::sprintf;

void Java::run(int memory) {
    char java_xms[1 << 7];
    char java_xmx[1 << 7];
    sprintf(java_xms, "-Xmx%dM", memory);
    sprintf(java_xmx, "-XX:MaxMetaspaceSize=%dM", memory);
    execl("/usr/bin/java", "/usr/bin/java", java_xms, java_xmx,
          "-Djava.security.manager",
          "-Djava.security.policy=./java.policy", "Main", (char *) nullptr);
}

void Java::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}
extlang createInstancejava () {
    return new Java;
}

deslang destroyInstanceJava (Language* language) {
    delete language;
}