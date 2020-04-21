//
// Created by Haoyuan Li on 2020/4/22.
//

#include "Kotlin.h"
#include <unistd.h>
#include <cstring>
#include "common/seccomp_helper.h"
#include <seccomp.h>

using std::memset;

std::string Kotlin::getFileSuffix() {
    return "kt";
}

void Kotlin::run(int memory) {
    char java_xms[1 << 7];
    char java_xmx[1 << 7];
    sprintf(java_xms, "-Xmx%dM", memory);
    sprintf(java_xmx, "-XX:MaxMetaspaceSize=%dM", memory);
    execl("/usr/bin/java", "/usr/bin/java", "-jar", java_xms, java_xmx,
          "-Djava.security.manager",
          "-Djava.security.policy=./java.policy", "Main.jar", (char *) nullptr);
}

void Kotlin::compile(std::vector<std::string>& _args, const char* java_xms, const char* java_xmx) {
    Language::compile(_args, java_xms, java_xmx);
}

bool Kotlin::supportParallel() {
    return false;
}

extlang createInstancekotlin () {
    return new Kotlin;
}

deslang destroyInstancekotlin (Language* language) {
    delete language;
}
