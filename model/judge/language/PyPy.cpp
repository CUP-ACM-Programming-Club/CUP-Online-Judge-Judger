//
// Created by Haoyuan Li on 2019/12/24.
//

#include "PyPy.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#include <fstream>
#include <iostream>
#ifdef __i386
#include "syscall/pypy/syscall32.h"
#else
#include "syscall/pypy/syscall64.h"
#endif
using std::memset;

void PyPy::run(int memory) {
    execl("/pypy/bin/pypy", "/pypy/bin/pypy", "Main.py", (char *) nullptr);
}

void PyPy::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("mkdir -p %s/usr/include", work_dir);
    execute_cmd("mkdir -p %s/dev", work_dir);
    execute_cmd("mkdir -p %s/usr/lib", work_dir);
    execute_cmd("mkdir -p %s/usr/lib64", work_dir);
    execute_cmd("mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("cp -a /usr/local/pypy %s/", work_dir);
    // execute_cmd("cp -a /usr/include/pypy* %s/usr/include/", work_dir);
    // execute_cmd("cp -a /usr/lib/libpython* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib64/libpthread.so.0 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libc.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libutil.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libdl.so.2 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libbz2.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libz.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libm.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/librt.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libcrypt.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libgcc_s.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libfreebl3.so %s/usr/lib64/", work_dir);
    execute_cmd("/bin/mkdir -p %s/home/judge", work_dir);
    execute_cmd("/bin/chown judge %s", work_dir);
    execute_cmd("/bin/mkdir -p %s/etc", work_dir);
    execute_cmd("/bin/grep judge /etc/passwd>%s/etc/passwd", work_dir);
    execute_cmd("/bin/mount -o bind /dev %s/dev", work_dir);
}

double PyPy::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int PyPy::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void PyPy::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || LANG_PYPYV[i]; ++i) {
        call_counter[LANG_PYPYV[i]] = HOJ_MAX_LIMIT;
    }
}

std::string PyPy::getFileSuffix() {
    return "py";
}

int PyPy::fixACStatus(int acFlag) {
    std::cerr << "Try to get sizeof error.out" << std::endl;
    auto error_size = get_file_size("error.out");
    std::cerr << "Error size:" << error_size << std::endl;
    if (error_size > 0) {
        std::fstream ferr("error.out");
        std::string tmp, content;
        while (getline(ferr, tmp)) {
            content += tmp;
        }
        if (content.find("SyntaxError") != content.npos) {
            return RUNTIME_ERROR;
        }
        return acFlag;
    }
    return acFlag;
}

int PyPy::getMemory(rusage ruse, pid_t pid) {
    const char* mark = "VmPeak:";
    const unsigned BUFFER_SIZE = 5 * (1 << 10);
    FILE *pf;
    char fn[BUFFER_SIZE], buf[BUFFER_SIZE];
    int ret = 0;
    sprintf(fn, "/proc/%d/status", pid);
    pf = fopen(fn, "re");
    auto m = static_cast<int>(strlen(mark));
    while (pf && fgets(buf, BUFFER_SIZE - 1, pf)) {

        buf[strlen(buf) - 1] = 0;
        if (strncmp(buf, mark, m) == 0) {
            sscanf(buf + m + 1, "%d", &ret);
        }
    }
    if (pf)
        fclose(pf);
    return ret << 10;
}

extlang createInstancepypy () {
    return new PyPy;
}

deslang destroyInstancepypy (Language* language) {
    delete language;
}
