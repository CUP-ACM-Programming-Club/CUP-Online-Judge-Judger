//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Java.h"
#include "unistd.h"
#include "util/util.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/resource.h>
#ifdef __i386
#include "syscall/java/syscall32.h"
#else
#include "syscall/java/syscall64.h"
#endif

#include <seccomp.h>
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_JV

const int STD_MB = 1 << 20;

using std::sprintf;
using std::memcpy;
using std::memset;
using std::to_string;

void Java::run(int memory) {
    char java_xms[1 << 7];
    char java_xmx[1 << 7];
    sprintf(java_xms, "-Xmx%dM", memory);
    sprintf(java_xmx, "-XX:MaxMetaspaceSize=%dM", memory);
    args[1] = java_xms;
    args[2] = java_xmx;
    execv("/usr/bin/java", args);
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

double Java::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Java::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Java::setExtraPolicy(const char* oj_home, const char* work_dir) {
    execute_cmd("/bin/cp %s/etc/java0.policy %s/java.policy", oj_home, work_dir);
    execute_cmd("chmod 755 %s/java.policy", work_dir);
    execute_cmd("chown judge %s/java.policy", work_dir);
}

void Java::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++)
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
}

void Java::setCompileMount(const char *work_dir) {
    // do nothing
}

std::string Java::getFileSuffix() {
    return "java";
}

void Java::buildChrootSandbox(const char *work_dir) {
    // do nothing
}

bool Java::enableSim() {
    return true;
}

void Java::fixFlagWithVMIssue(char *work_dir, int &ACflg, int &topmemory, int mem_lmt) {
    int comp_res;
    execute_cmd("chmod 700 %s/error.out", work_dir);
    execute_cmd("cat %s/error.out", work_dir);
    comp_res = execute_cmd("/bin/grep 'Exception'  %s/error.out", work_dir);
    if (!comp_res) {
        printf("Exception reported\n");
        ACflg = RUNTIME_ERROR;
    }
    execute_cmd("cat %s/error.out", work_dir);

    comp_res = execute_cmd(
            "/bin/grep 'java.lang.OutOfMemoryError'  %s/error.out", work_dir);

    if (!comp_res) {
        printf("JVM need more Memory!");
        ACflg = MEMORY_LIMIT_EXCEEDED;
        topmemory = mem_lmt * STD_MB;
    }

    if (!comp_res) {
        printf("JVM need more Memory or Threads!");
        ACflg = MEMORY_LIMIT_EXCEEDED;
        topmemory = mem_lmt * STD_MB;
    }
    comp_res = execute_cmd("/bin/grep 'Could not create'  %s/error.out",
                           work_dir);
    if (!comp_res) {
        printf("jvm need more resource,tweak -Xmx(OJ_JAVA_BONUS) Settings");
        ACflg = RUNTIME_ERROR;
    }
}

bool Java::gotErrorWhileRunning(bool error) {
    return error;
}

bool Java::isValidExitCode(int exitcode) {
    return (exitcode == 17 || Language::isValidExitCode(exitcode));
}

void Java::buildSeccompSandbox() {
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_KILL);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        if (SYSCALL_ARRAY[i] == 59) {
            continue;
        }
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYSCALL_ARRAY[i], 0);
    }
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A1(SCMP_CMP_EQ, (scmp_datum_t)(getArgs())));
    if (install_helper()) {
        printf("install helper failed");
        exit(1);
    }
    seccomp_load(ctx);
}

char **Java::getArgs() {
    return args;
}

