//
// Created by Ryan Lee on 2019-07-02.
//
#include "static_var.h"
int database_port;
char host_name[BUFFER_SIZE];
char user_name[BUFFER_SIZE];
char password[BUFFER_SIZE];
char db_name[BUFFER_SIZE];
char oj_home[BUFFER_SIZE];
//static char data_list[BUFFER_SIZE][BUFFER_SIZE];
//static int data_list_len = 0;
int DEBUG = 0;
int NO_RECORD = 0;
bool MYSQL_MODE = true;
bool READ_FROM_STDIN = false;
int javaTimeBonus = 5;
int java_memory_bonus = 512;
char java_xms[BUFFER_SIZE];
char java_xmx[BUFFER_SIZE];
int sim_enable = 0;
int ALL_TEST_MODE = 1;
int full_diff = 0;
int use_max_time = 0;

int http_judge = 0;
char http_baseurl[BUFFER_SIZE];

char http_username[BUFFER_SIZE];
char http_password[BUFFER_SIZE];
int SHARE_MEMORY_RUN = 0;

char record_call = 0;
int use_ptrace = 1;
int enable_parallel = 0;
int judger_number = 0;
bool admin = false;
bool no_sim = false;
int solution_id;
//static int sleep_tmp;

WebSocketSender webSocket;

Bundle bundle;

JSONVectorReader languageNameReader;

ThreadPool threadPool(std::thread::hardware_concurrency());