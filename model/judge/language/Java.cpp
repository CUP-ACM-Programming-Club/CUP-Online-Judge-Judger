//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Java.h"
#include "unistd.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/resource.h>

using std::sprintf;
using std::memcpy;
using std::memset;
using std::to_string;

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

void Java::setCompileProcessLimit() {
    Language::setCPULimit();
    this->setAlarm();
    Language::setFSizeLimit();
}

void Java::setAlarm() {
    alarm(30);
}

void Java::setASLimit() {
    // do nothing
}

void Java::compile(std::vector<std::string> &_args, const char * java_xms, const char *java_xmx) {
    int len = _args.size();
    char *java_arg[len + 5];
    char java_buffer[len + 5][30];
    for (int i = 0; i < len; ++i) {
        memset(java_buffer[i], 0, sizeof(java_buffer[i]));
        memcpy(java_buffer[i], _args[i].c_str(), _args[i].length());
        java_arg[i] = java_buffer[i];
    }
    java_arg[len] = nullptr;
    sprintf(java_buffer[1], "-J%s", java_xms);
    sprintf(java_buffer[2], "-J%s", java_xmx);
    execvp(java_arg[0], (char *const *) java_arg);
}

void Java::buildRuntime(const char *work_dir) {
    // do nothing
}
