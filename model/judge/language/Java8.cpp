//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Java8.h"
#include <cstdio>
#include "unistd.h"

using std::sprintf;

void Java8::run(int memory) {
    char java_xms[1 << 7];
    char java_xmx[1 << 7];
    sprintf(java_xms, "-Xmx%dM", memory);
    sprintf(java_xmx, "-XX:MaxMetaspaceSize=%dM", memory);
    execl("/usr/bin/java-8", "/usr/bin/java-8", java_xms, java_xmx,
          "-Djava.security.manager",
          "-Djava.security.policy=./java.policy", "Main", (char *) nullptr);
}
