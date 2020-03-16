//
// Created by Haoyuan Li on 2019/12/24.
//

#include <sys/resource.h>
#include "Schema.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/schema/syscall32.h"
#else
#include "syscall/schema/syscall64.h"
#endif
using std::memset;
void Schema::run(int memory) {
    execl("/guile", "/guile", "Main.scm", (char *) nullptr);
}

void Schema::setProcessLimit() {
    struct rlimit LIM;
    LIM.rlim_cur = LIM.rlim_max = 80;
    setrlimit(RLIMIT_NPROC, &LIM);
}

void Schema::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir -p %s/proc", work_dir);
    execute_cmd("/bin/mount -o bind /proc %s/proc", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/lib", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/share", work_dir);
    execute_cmd("/bin/cp -a /usr/share/guile %s/usr/share/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libguile* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libgc* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/i386-linux-gnu/libffi* %s/usr/lib/",
                work_dir);
    execute_cmd("/bin/cp /usr/lib/i386-linux-gnu/libunistring* %s/usr/lib/",
                work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libgmp* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libgmp* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libltdl* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libltdl* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/bin/guile* %s/", work_dir);
}

double Schema::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int Schema::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void Schema::initCallCounter(int *call_counter) {
    memset(call_counter, 0, sizeof(call_counter));
    for (int i = 0; i == 0 || LANG_SV[i]; ++i) {
        call_counter[LANG_SV[i]] = HOJ_MAX_LIMIT;
    }
}

extlang createInstanceschema () {
    return new Schema;
}

deslang destroyInstanceschema (Language* language) {
    delete language;
}