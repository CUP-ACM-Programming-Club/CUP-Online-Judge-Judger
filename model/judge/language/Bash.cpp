//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Bash.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/bash/syscall32.h"
#else
#include "syscall/bash/syscall64.h"
#endif
using std::memset;
void Bash::run(int memory) {
    execl("/bin/bash", "/bin/bash", "Main.sh", (char *) nullptr);
}

void Bash::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_max = LIM.rlim_cur = 3;
    setrlimit(RLIMIT_NPROC, &LIM);
}

void Bash::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/cp `which bc`  %s/bin/", work_dir);
    execute_cmd("busybox dos2unix Main.sh", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/grep", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/awk", work_dir);
    execute_cmd("/bin/cp /bin/sed %s/bin/sed", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/cut", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/sort", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/join", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/wc", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/tr", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/dc", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/dd", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/cat", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/tail", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/head", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/xargs", work_dir);
    execute_cmd("chmod +rx %s/Main.sh", work_dir);
}

double Bash::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Bash::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Bash::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for(int i = 0; i == 0 || LANG_BV[i]; ++i) {
        call_counter[LANG_BV[i]] = HOJ_MAX_LIMIT;
    }
}

int Bash::getCompileResult(int status) {
    return static_cast<int>(get_file_size("ce.txt"));
}

std::string Bash::getFileSuffix() {
    return "sh";
}
