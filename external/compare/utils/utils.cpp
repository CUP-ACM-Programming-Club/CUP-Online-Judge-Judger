//
// Created by Haoyuan Li on 2020/3/30.
//

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "utils.h"

const unsigned BUFFER_SIZE = 1 << 20;

int execute_cmd(const char *fmt, ...) {
    char cmd[1 << 20];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(cmd, fmt, ap);
    int ret = system(cmd);
    va_end(ap);
    return ret;
}

int execute_timeout_cmd(long long timeout, const char *fmt, ...) {
    std::mutex m;
    std::condition_variable cv;
    int retValue;
    char cmd[BUFFER_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(cmd, fmt, ap);
    std::thread t([&]() {
        retValue = execute_cmd(cmd);
        cv.notify_one();
    });
    t.detach();
    {
        std::unique_lock<std::mutex> l(m);
        if (cv.wait_for(l, timeout * std::chrono::seconds(1)) == std::cv_status::timeout) {
            throw std::runtime_error("Timeout");
        }
    }
    return retValue;
}


const char *getFileNameFromPath(const char *path) {
    for (auto i = static_cast<int>(strlen(path)); i >= 0; i--) {
        if (path[i] == '/')
            return &path[i + 1];
    }
    return path;
}
