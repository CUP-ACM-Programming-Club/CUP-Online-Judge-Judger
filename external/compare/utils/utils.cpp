//
// Created by Haoyuan Li on 2020/3/30.
//

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "utils.h"

int execute_cmd(const char *fmt, ...) {
    char cmd[1 << 20];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(cmd, fmt, ap);
    int ret = system(cmd);
    va_end(ap);
    return ret;
}

const char *getFileNameFromPath(const char *path) {
    for (auto i = static_cast<int>(strlen(path)); i >= 0; i--) {
        if (path[i] == '/')
            return &path[i + 1];
    }
    return path;
}
