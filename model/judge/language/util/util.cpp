//
// Created by Haoyuan Li on 2020/3/16.
//

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
const unsigned BUFFER_SIZE = 1 << 20;

int execute_cmd(const char *fmt, ...) {
    char cmd[BUFFER_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(cmd, fmt, ap);
    int ret = system(cmd);
    va_end(ap);
    return ret;
}

long get_file_size(const char *filename) {
    struct stat f_stat{};

    if (stat(filename, &f_stat) == -1) {
        return 0;
    }

    return (long) f_stat.st_size;
}
