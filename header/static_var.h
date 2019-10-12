//
// Created by Ryan Lee on 2018/7/17.
//

#ifndef JUDGE_CLIENT_STATIC_VAR_H
#define JUDGE_CLIENT_STATIC_VAR_H

#include <thread>
#include "../model/base/MySQLAutoPointer.h"
#include "../model/wrapper/websocket.h"
#include "../model/base/Bundle.h"
#include "../model/base/ThreadPool.h"

enum count {
    ZERO = 0,
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9
};

enum time {
    SECOND = 1,
    MINUTE = 60,
    HOUR = 3600,
    DAY = HOUR * 24
};

enum SpaceSizeConstants {
    ONE_KILOBYTE = 1u << 10,
    ONE_MEGABYTE = 1u << 20,
    ONE_GIGABYTE = 1u << 30
};

enum LanguageConstants {
    C11 = 0,
    CPP17 = 1,
    PASCAL = 2,
    JAVA = 3,
    RUBY = 4,
    BASH = 5,
    PYTHON2 = 6,
    PHP = 7,
    PERL = 8,
    CSHARP = 9,
    OBJC = 10,
    FREEBASIC = 11,
    SCHEMA = 12,
    CLANG = 13,
    CLANGPP = 14,
    LUA = 15,
    JAVASCRIPT = 16,
    GO = 17,
    PYTHON3 = 18,
    CPP11 = 19,
    CPP98 = 20,
    C99 = 21,
    KOTLIN = 22,
    JAVA8 = 23,
    JAVA7 = 24,
    PyPy = 25,
    PyPy3 = 26,
    JAVA6 = 27,
    CLANG11 = 28,
    CLANGPP17 = 29,
    OTHER
};

enum status {
    OJ_WT0 = 0,
    OJ_WT1 = 1,
    OJ_CI = 2,
    OJ_RI = 3,
    OJ_AC = 4,
    OJ_PE = 5,
    OJ_WA = 6,
    OJ_TL = 7,
    OJ_ML = 8,
    OJ_OL = 9,
    OJ_RE = 10,
    OJ_CE = 11,
    OJ_CO = 12,
    OJ_TR = 13
};

enum judge_status {
    ZERO_TIME = 0,
    ZERO_MEMORY = 0,
    ZERO_SIM = 0,
    NOT_FINISHED = 0,
    FINISHED = 1,
    ZERO_PASSPOINT = 0,
    ZERO_PASSRATE = 0,
    CHILD_PROCESS = 0,
    TEST_RUN_SUBMIT = 0,
    TEST_RUN_PROBLEM = 0,
    NONE_SPECIAL_JUDGE = 0,
    COMPILED = 0
};

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

enum ArgsType {
    _ERROR = 0x99,
    _DEBUG = 0,
    _LANG_NAME = 1,
    _NO_RECORD = 2,
    _DIR = 3,
    _RECORD_CALL = 4,
    _SOLUTION_ID = 5,
    _RUNNER_ID = 6,
    _ADMIN = 7,
    _NO_SIM = 8,
    _NO_MYSQL = 9
};

const int CODESIZE = 64 * ONE_KILOBYTE;
const int BUFFER_SIZE = 5 * ONE_KILOBYTE;
const int STD_MB = ONE_MEGABYTE;
const int COMPILE_STD_MB = (int) (STD_MB * 1.5);
//#define STD_T_LIM 2
const int STD_F_LIM = STD_MB * 32;
//#define STD_M_LIM (STD_MB<<7)
const int DEFAULT_SOLUTION_ID = 1000;
extern int DEBUG;
extern int NO_RECORD;
extern bool MYSQL_MODE;
extern char lang_ext[30][8];
extern char host_name[BUFFER_SIZE];
extern char user_name[BUFFER_SIZE];
extern char password[BUFFER_SIZE];
extern char db_name[BUFFER_SIZE];
extern char oj_home[BUFFER_SIZE];
//extern char data_list[BUFFER_SIZE][BUFFER_SIZE];
//extern int data_list_len = 0;
extern int javaTimeBonus;
extern int java_memory_bonus;
extern char java_xms[BUFFER_SIZE];
extern char java_xmx[BUFFER_SIZE];
extern int sim_enable;
extern int ALL_TEST_MODE;
extern int full_diff;
extern int use_max_time;

extern int http_judge;
extern char http_baseurl[BUFFER_SIZE];

extern char http_username[BUFFER_SIZE];
extern char http_password[BUFFER_SIZE];
extern int SHARE_MEMORY_RUN;

extern char record_call;
extern int use_ptrace;
extern int judger_number;
extern bool admin;
extern bool no_sim;
extern int solution_id;
extern MySQLAutoPointer conn;
extern websocket webSocket;
extern Bundle bundle;
extern ThreadPool threadPool;
#endif //JUDGE_CLIENT_STATIC_VAR_H