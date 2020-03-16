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
