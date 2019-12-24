//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Java6.h"
#include <cstdio>
#include "unistd.h";
using std::sprintf;

void Java6::run(int memory) {
    char java_xms[1 << 7];
    char java_xmx[1 << 7];
    sprintf(java_xms, "-Xmx%dM", memory);
    sprintf(java_xmx, "-XX:MaxPermSize=%dM", memory);
    execl("/usr/bin/java-6", "/usr/bin/java-6", java_xms, java_xmx,
          "-Djava.security.manager",
          "-Djava.security.policy=./java.policy", "Main", (char *) nullptr);
}
