//
// Created by Haoyuan Li on 2019/12/24.
//

#include "PyPy3.h"
#include "unistd.h"
#include "util/util.h"
#include <cstring>
#include <iostream>
#include <fstream>
#ifdef __i386
#include "syscall/pypy3/syscall32.h"
#else
#include "syscall/pypy3/syscall64.h"
#endif

#include <seccomp.h>
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_PYPY3V

using std::memset;
const int STD_MB = 1 << 20;

void PyPy3::run(int memory) {
    execv(args[0], args);
}

void PyPy3::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("mkdir -p %s/usr/include", work_dir);
    execute_cmd("mkdir -p %s/dev", work_dir);
    execute_cmd("mkdir -p %s/usr/lib", work_dir);
    execute_cmd("mkdir -p %s/usr/lib64", work_dir);
    execute_cmd("mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("mkdir -p %s/proc", work_dir);
    execute_cmd("cp -a /proc/cpuinfo %s/proc/", work_dir);
    execute_cmd("cp -a /usr/local/pypy3 %s/pypy3", work_dir);
    execute_cmd("cp -a /usr/local/pypy3/bin/libpypy3-c.so %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/local/pypy3/lib %s/usr/lib64", work_dir);
    execute_cmd("cp -a /usr/local/pypy3/lib %s/usr/lib", work_dir);
    execute_cmd("cp -a /usr/lib64/linux-vdso.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libpthread.so.0 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libc.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libutil.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libdl.so.2 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libbz2.so.1.0 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libbz2.so.1.0.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libbz2.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libexpat.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libm.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libz.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/librt.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libcrypt.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libffi.so.6 %s/usr/lib64/", work_dir);
    //execute_cmd("cp -a /usr/lib64/libncursesw.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libtinfow.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libgcc_s.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/ld-linux-x86-64.so.2 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libfreebl3.so %s/usr/lib64/", work_dir);

    execute_cmd("/bin/mkdir -p %s/home/judge", work_dir);
    execute_cmd("/bin/chown -R judge %s", work_dir);
    execute_cmd("/bin/mkdir -p %s/etc", work_dir);
    execute_cmd("/bin/grep judge /etc/passwd>%s/etc/passwd", work_dir);
    execute_cmd("/bin/mount -o bind /dev %s/dev", work_dir);
}

double PyPy3::buildTimeLimit(double timeLimit, double bonus) {
    return BonusLimit::buildBonusTimeLimit(timeLimit, bonus);
}

int PyPy3::buildMemoryLimit(int memoryLimit, int bonus) {
    return BonusLimit::buildBonusMemoryLimit(memoryLimit, bonus);
}

void PyPy3::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; ++i) {
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
    }

}

std::string PyPy3::getFileSuffix() {
    return "py";
}

int PyPy3::fixACStatus(int acFlag) {
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

int PyPy3::getMemory(rusage ruse, pid_t pid) {
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

void PyPy3::fixACFlag(int &ACflg) {
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
            ACflg = RUNTIME_ERROR;
        }
    }
}

void PyPy3::fixFlagWithVMIssue(char *work_dir, int &ACflg, int &topmemory, int mem_lmt) {
    int comp_res = execute_cmd(
            "/bin/grep 'MemoryError'  %s/error.out", work_dir);

    if (!comp_res) {
        printf("Python need more Memory!");
        ACflg = MEMORY_LIMIT_EXCEEDED;
        topmemory = mem_lmt * STD_MB;
    }
}

void PyPy3::buildSeccompSandbox() {
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

char **PyPy3::getArgs() {
    return args;
}

extlang createInstancepypy3() {
    return new PyPy3;
}

deslang destroyInstancepypy3(Language* language) {
    delete language;
}