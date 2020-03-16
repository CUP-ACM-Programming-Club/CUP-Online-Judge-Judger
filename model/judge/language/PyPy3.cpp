//
// Created by Haoyuan Li on 2019/12/24.
//

#include "PyPy3.h"
#include "unistd.h"
#include "util/util.h"

void PyPy3::run(int memory) {
    execl("/pypy3/bin/pypy3", "/pypy3/bin/pypy3", "Main.py", (char *) nullptr);
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

extlang createInstancepypy3() {
    return new PyPy3;
}

deslang destroyInstancepypy3(Language* language) {
    delete language;
}