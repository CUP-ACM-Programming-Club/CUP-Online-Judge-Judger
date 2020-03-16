//
// Created by Haoyuan Li on 2020/3/16.
//

#include "CPython.h"
#include "../util/util.h"

void CPython::buildRuntime(const char* work_dir) {
    execute_cmd("mkdir -p %s/usr/include", work_dir);
    execute_cmd("mkdir -p %s/dev", work_dir);
    execute_cmd("mkdir -p %s/usr/lib", work_dir);
    execute_cmd("mkdir -p %s/usr/lib64", work_dir);
    execute_cmd("mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("cp /usr/bin/python* %s/", work_dir);
    execute_cmd("cp /usr/local/bin/python* %s/", work_dir);
    execute_cmd("cp -a /usr/lib/python* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/local/lib/python* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib64/python* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/local/lib64/python* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/local/lib/python* %s/usr/local/lib/", work_dir);
    execute_cmd("cp -a /usr/include/python* %s/usr/include/", work_dir);
    execute_cmd("cp -a /usr/local/include/python* %s/usr/include/", work_dir);
    execute_cmd("cp -a /usr/lib/libpython* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/local/lib/libpython* %s/usr/lib/", work_dir);
    execute_cmd("/bin/mkdir -p %s/home/judge", work_dir);
    execute_cmd("/bin/chown judge %s", work_dir);
    execute_cmd("/bin/mkdir -p %s/etc", work_dir);
    execute_cmd("/bin/grep judge /etc/passwd>%s/etc/passwd", work_dir);
    execute_cmd("/bin/mount -o bind /dev %s/dev", work_dir);
}
