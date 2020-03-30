//
// Created by Haoyuan Li on 2020/3/30.
//

#ifndef COMPARE_UTILS_H
#define COMPARE_UTILS_H
extern int execute_cmd(const char *fmt, ...);

extern const char *getFileNameFromPath(const char *path);

enum judge_procedure {
    WAITING = 0,
    WAITING_REJUDGE = 1,
    COMPILING = 2,
    RUNNING_JUDGING = 3,
    ACCEPT = 4,
    PRESENTATION_ERROR = 5,
    WRONG_ANSWER = 6,
    TIME_LIMIT_EXCEEDED = 7,
    MEMORY_LIMIT_EXCEEDED = 8,
    OUTPUT_LIMIT_EXCEEDED = 9,
    RUNTIME_ERROR = 10,
    COMPILE_ERROR = 11,
    COMPILE_OK = 12,
    TEST_RUN = 13,
    SUBMITTED = 14,
    SYSTEM_REJECTED = 15
};

#endif //COMPARE_UTILS_H
