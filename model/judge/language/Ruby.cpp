//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Ruby.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/ruby/syscall32.h"
#else
#include "syscall/ruby/syscall64.h"
#endif
using std::memset;
void Ruby::run(int memory) {
    execl("/ruby", "/ruby", "Main.rb", (char *) nullptr);
}

void Ruby::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}
extlang createInstanceruby() {
    return new Ruby;
}

deslang destroyInstanceruby (Language* language) {
    delete language;
}

void Ruby::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("mkdir -p %s/usr", work_dir);
    execute_cmd("mkdir -p %s/usr/lib", work_dir);
    execute_cmd("mkdir -p %s/usr/lib64", work_dir);
    execute_cmd("cp -a /usr/lib/libruby* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib/ruby* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib64/ruby* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libruby* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/bin/ruby* %s/", work_dir);
}

double Ruby::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Ruby::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Ruby::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || LANG_RV[i]; ++i) {
        call_counter[LANG_RV[i]] = HOJ_MAX_LIMIT;
    }
}

int Ruby::getCompileResult(int status) {
    return static_cast<int>(get_file_size("ce.txt"));
}

std::string Ruby::getFileSuffix() {
    return "rb";
}
