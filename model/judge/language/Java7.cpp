//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Java7.h"
#include <cstdio>
#include "unistd.h"

using std::sprintf;
void Java7::run(int memory) {
    char java_xms[1u << 7];
    char java_xmx[1u << 7];
    sprintf(java_xms, "-Xmx%dM", memory);
    sprintf(java_xmx, "-XX:MaxPermSize=%dM", memory);
    execl("/usr/bin/java-7", "/usr/bin/java-7", java_xms, java_xmx,
          "-Djava.security.manager",
          "-Djava.security.policy=./java.policy", "Main", (char *) nullptr);
}
