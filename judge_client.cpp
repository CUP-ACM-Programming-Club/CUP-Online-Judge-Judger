//
// Created by admin on 2017/10/18.
//

//
#define IGNORE_ESOL
// File:   main.cc
// Author: sempr
// refacted by zhblue
/*
 * Copyright 2008 sempr <iamsempr@gmail.com>
 *
 * Refacted and modified by zhblue<newsclan@gmail.com>
 * Bug report email newsclan@gmail.com
 *
 *
 * This file is part of HUSTOJ.
 *
 * HUSTOJ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * HUSTOJ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HUSTOJ. if not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <cstdarg>
#include <cctype>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/resource.h>
//#include <sys/types.h>
#ifndef _NO_MYSQL
#ifdef __APPLE_CC__

#include <mysql.h>

#else
#include <mysql/mysql.h>
#endif
#endif

#include "header/okcalls.h"
#include "model/websocket.h"
#include "header/static_var.h"

#include "model/JSONVectorReader.h"
#include "model/Bundle.h"

#include "library/judge_lib.h"
#include "model/MySQLAutoPointer.h"
using namespace std;
using json = nlohmann::json;
using CompilerArgsReader = JSONVectorReader;
using ConfigReader = JSONVectorReader;
//


/*copy from ZOJ
 http://code.google.com/p/zoj/source/browse/trunk/judge_client/client/tracer.cc?spec=svn367&r=367#39
 */
#ifdef __i386
#define REG_SYSCALL orig_eax
#define REG_RET eax
#define REG_ARG0 ebx
#define REG_ARG1 ecx
#else
#define REG_SYSCALL orig_rax
#define REG_RET rax
#define REG_ARG0 rdi
#define REG_ARG1 rsi

#endif


static char host_name[BUFFER_SIZE];
static char user_name[BUFFER_SIZE];
static char password[BUFFER_SIZE];
static char db_name[BUFFER_SIZE];
static char oj_home[BUFFER_SIZE];
//static char data_list[BUFFER_SIZE][BUFFER_SIZE];
//static int data_list_len = 0;

static int port_number;
static int max_running;
static int sleep_time;
static int java_time_bonus = 5;
static int java_memory_bonus = 512;
static char java_xms[BUFFER_SIZE];
static char java_xmx[BUFFER_SIZE];
static int sim_enable = 0;
static int ALL_TEST_MODE = 0;
static int full_diff = 0;
static int use_max_time = 0;

static int http_judge = 0;
static char http_baseurl[BUFFER_SIZE];

static char http_username[BUFFER_SIZE];
static char http_password[BUFFER_SIZE];
static int SHARE_MEMORY_RUN = 0;

static char record_call = 0;
static int use_ptrace = 1;
static int judger_number = 0;
static bool admin = false;
int solution_id;
//static int sleep_tmp;
#define ZOJ_COM
MySQLAutoPointer conn;

websocket webSocket;




const int call_array_size = 512;
int call_counter[call_array_size];
static char LANG_NAME[BUFFER_SIZE];

void init_syscalls_limits(int lang) {
    int i;
    memset(call_counter, 0, sizeof(call_counter));
    if (DEBUG)
        write_log(oj_home, "init_call_counter:%d", lang);
    if (record_call) { // C & C++
        for (i = 0; i < call_array_size; i++) {
            call_counter[i] = 0;
        }
    } else if (lang == C11 || lang == CPP17 || lang == CLANG || lang == CLANGPP || lang == CLANG11 ||
               lang == CLANGPP17 || lang == C99 || lang == CPP11 ||
               lang == CPP98) { // C & C++
        for (i = 0; i == 0 || LANG_CV[i]; i++) {
            call_counter[LANG_CV[i]] = HOJ_MAX_LIMIT;
        }
    } else if (lang == PASCAL) { // Pascal
        for (i = 0; i == 0 || LANG_PV[i]; i++)
            call_counter[LANG_PV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == JAVA || lang == JAVA7 || lang == JAVA8 || lang == JAVA6) { // Java
        for (i = 0; i == 0 || LANG_JV[i]; i++)
            call_counter[LANG_JV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == RUBY) { // Ruby
        for (i = 0; i == 0 || LANG_RV[i]; i++)
            call_counter[LANG_RV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == BASH) { // Bash
        for (i = 0; i == 0 || LANG_BV[i]; i++)
            call_counter[LANG_BV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PYTHON2) { // Python
        for (i = 0; i == 0 || LANG_YV[i]; i++)
            call_counter[LANG_YV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PYTHON3) {
        for (i = 0; i == 0 || LANG_PY3V[i]; i++)
            call_counter[LANG_PY3V[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PHP) { // php
        for (i = 0; i == 0 || LANG_PHV[i]; i++)
            call_counter[LANG_PHV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PERL) { // perl
        for (i = 0; i == 0 || LANG_PLV[i]; i++)
            call_counter[LANG_PLV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == CSHARP) { // mono c#
        for (i = 0; i == 0 || LANG_CSV[i]; i++)
            call_counter[LANG_CSV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == OBJC) { //objective c
        for (i = 0; i == 0 || LANG_OV[i]; i++)
            call_counter[LANG_OV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == FREEBASIC) { //free basic
        for (i = 0; i == 0 || LANG_BASICV[i]; i++)
            call_counter[LANG_BASICV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == SCHEMA) { //scheme guile
        for (i = 0; i == 0 || LANG_SV[i]; i++)
            call_counter[LANG_SV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == LUA) { //lua
        for (i = 0; i == 0 || LANG_LUAV[i]; i++)
            call_counter[LANG_LUAV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == JAVASCRIPT) { //nodejs
        for (i = 0; i == 0 || LANG_JSV[i]; i++)
            call_counter[LANG_JSV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == GO) { //go
        for (i = 0; i == 0 || LANG_GOV[i]; i++)
            call_counter[LANG_GOV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PyPy) {
        for (i = 0; i == 0 || LANG_PYPYV[i]; ++i) {
            call_counter[LANG_PYPYV[i]] = HOJ_MAX_LIMIT;
        }
    } else if (lang == PyPy3) {
        for (i = 0; i == 0 || LANG_PYPY3V[i]; ++i) {
            call_counter[LANG_PYPY3V[i]] = HOJ_MAX_LIMIT;
        }
    }
}


// read the configue file
void init_mysql_conf() {
    string configDIR = oj_home;
    configDIR += "/etc/config.json";
    ConfigReader configReader(configDIR);

    FILE *fp = nullptr;
    char buf[BUFFER_SIZE];
    host_name[0] = 0;
    user_name[0] = 0;
    password[0] = 0;
    db_name[0] = 0;
    port_number = 3306;
    max_running = 3;
    sleep_time = 3;
    strcpy(java_xms, "-Xms32m");
    strcpy(java_xmx, "-Xmx256m");
    sprintf(buf, "%s/etc/judge.conf", oj_home);
    fp = fopen("./etc/judge.conf", "re");
    if (fp != nullptr) {
        while (fgets(buf, BUFFER_SIZE - 1, fp)) {
            read_buf(buf, "OJ_HOST_NAME", host_name);
            read_buf(buf, "OJ_USER_NAME", user_name);
            read_buf(buf, "OJ_PASSWORD", password);
            read_buf(buf, "OJ_DB_NAME", db_name);
            read_int(buf, "OJ_PORT_NUMBER", port_number);
            read_int(buf, "OJ_JAVA_TIME_BONUS", java_time_bonus);
            read_int(buf, "OJ_JAVA_MEMORY_BONUS", java_memory_bonus);
            read_int(buf, "OJ_SIM_ENABLE", sim_enable);
            read_buf(buf, "OJ_JAVA_XMS", java_xms);
            read_buf(buf, "OJ_JAVA_XMX", java_xmx);
            read_int(buf, "OJ_HTTP_JUDGE", http_judge);
            read_buf(buf, "OJ_HTTP_BASEURL", http_baseurl);
            read_buf(buf, "OJ_HTTP_USERNAME", http_username);
            read_buf(buf, "OJ_HTTP_PASSWORD", http_password);
            read_int(buf, "OJ_OI_MODE", ALL_TEST_MODE);
            read_int(buf, "OJ_FULL_DIFF", full_diff);
            read_int(buf, "OJ_SHM_RUN", SHARE_MEMORY_RUN);
            read_int(buf, "OJ_USE_MAX_TIME", use_max_time);
            read_int(buf, "OJ_USE_PTRACE", use_ptrace);

        }
        //fclose(fp);
    }
    conn.setPort(port_number);
    conn.setDBName(db_name);
    conn.setUserName(user_name);
    conn.setPassword(password);
    conn.setHostName(host_name);
    //  fclose(fp);
}


void find_next_nonspace(int &c1, int &c2, FILE *&f1, FILE *&f2, int &ret) {
    // Find the next non-space character or \n.
    while ((isspace(c1)) || (isspace(c2))) {
        if (c1 != c2) {
            if (c2 == EOF) {
                do {
                    c1 = fgetc(f1);
                } while (isspace(c1));
                continue;
            } else if (c1 == EOF) {
                do {
                    c2 = fgetc(f2);
                } while (isspace(c2));
                continue;
#ifdef IGNORE_ESOL
            } else if (isspace(c1) && isspace(c2)) {
                while (c2 == '\n' && isspace(c1) && c1 != '\n')
                    c1 = fgetc(f1);
                while (c1 == '\n' && isspace(c2) && c2 != '\n')
                    c2 = fgetc(f2);

#else
                } else if ((c1 == '\r' && c2 == '\n')) {
                c1 = fgetc(f1);
            } else if ((c2 == '\r' && c1 == '\n')) {
                c2 = fgetc(f2);
#endif
            } else {
                if (DEBUG)
                    printf("%d=%c\t%d=%c", c1, c1, c2, c2);
                ret = PRESENTATION_ERROR;
            }
        }
        if (isspace(c1)) {
            c1 = fgetc(f1);
        }
        if (isspace(c2)) {
            c2 = fgetc(f2);
        }
    }
}

/***
 int compare_diff(const char *file1,const char *file2){
 char diff[1024];
 sprintf(diff,"diff -q -B -b -w --strip-trailing-cr %s %s",file1,file2);
 int d=system(diff);
 if (d) return OJ_WA;
 sprintf(diff,"diff -q -B --strip-trailing-cr %s %s",file1,file2);
 int p=system(diff);
 if (p) return OJ_PE;
 else return OJ_AC;

 }
 */



/*
 * translated from ZOJ judger r367
 * http://code.google.com/p/zoj/source/browse/trunk/judge_client/client/text_checker.cc#25
 *
 */
int choose = 1;


int compare_zoj(const char *file1, const char *file2) {
    if (DEBUG && choose) {
        do {
            fstream user_out(file1), error_file(file2);
            string tmp;
            cout << "Display user file?(0/1)" << endl;
            cin >> choose;
            if (choose) {
                cout << "user running file" << endl;
                while (getline(user_out, tmp))
                    cout << tmp << endl;
            } else break;
            cout << "Display error file?(0/1)" << endl;
            cin >> choose;
            if (choose) {
                cout << "error msg" << endl;
                while (getline(error_file, tmp))
                    cerr << tmp << endl;
            } else break;
        } while (0);
    }
    int ret = ACCEPT;
    int c1, c2;
    FILE *f1, *f2;
    f1 = fopen(file1, "re");
    f2 = fopen(file2, "re");
    auto judge_file_end_or_detect_presentation_error = [&](int c1, int c2, FILE *&f1, FILE *&f2, int &ret) -> void {
        if (is_not_character(c1) && c1 != EOF) {
            auto temp_ret = ret;
            if (c2 != EOF)
                ret = PRESENTATION_ERROR;
            move_to_next_nonspace_character(c1, f1, ret);
            if (c1 == EOF) {
                if (is_not_character(c2)) {
                    move_to_next_nonspace_character(c2, f2, ret);
                }
                if (c2 == EOF) {
                    ret = temp_ret;
                }
            }
        }
    };
    if (!f1 || !f2) {
        ret = RUNTIME_ERROR;
    } else
        for (;;) {
            // Find the first non-space character at the beginning of line.
            // Blank lines are skipped.
            c1 = fgetc(f1);
            c2 = fgetc(f2);
            find_next_nonspace(c1, c2, f1, f2, ret);
            // Compare the current line.
            for (;;) {
                // Read until 2 files return a space or 0 together.
                while ((!isspace(c1) && c1) || (!isspace(c2) && c2)) {
                    if (c1 == EOF && c2 == EOF) {
                        goto end;
                    }
                    if (c1 == EOF || c2 == EOF) {
                        break;
                    }

                    if (c1 != c2) {
                        if (DEBUG) {
                            cerr << "c1:" << (char) c1 << " c2:" << (char) c2 << endl;
                        }
                        // Consecutive non-space characters should be all exactly the same
                        ret = WRONG_ANSWER;
                        goto end;
                    }
                    c1 = fgetc(f1);
                    c2 = fgetc(f2);
                    while (c1 == '\r')c1 = fgetc(f1);
                    //while(c2 == '\r')c2 = fgetc(f2);
                    if (c1 != c2) {
                        cerr << "c1:" << c1 << " c2:" << c2 << endl;
                        judge_file_end_or_detect_presentation_error(c1, c2, f1, f2, ret);
                        judge_file_end_or_detect_presentation_error(c2, c1, f2, f1, ret);
                    }
                }
                find_next_nonspace(c1, c2, f1, f2, ret);
                if (c1 == EOF && c2 == EOF) {
                    goto end;
                }
                if (c1 == EOF || c2 == EOF) {
                    ret = WRONG_ANSWER;
                    goto end;
                }

                if ((c1 == '\n' || !c1) && (c2 == '\n' || !c2)) {
                    break;
                }
            }
        }
    end:
    if (ret == PRESENTATION_ERROR) {
        if (!check_valid_presentation_error(file1, file2)) {
            ret = WRONG_ANSWER;
        }
    }
    if (ret == WRONG_ANSWER || ret == PRESENTATION_ERROR) {
        if (full_diff)
            make_diff_out_full(f1, f2, c1, c2, file1);
        else
            make_diff_out_simple(f1, f2, c1, c2, file1);
    }
    if (f1)
        fclose(f1);
    if (f2)
        fclose(f2);
    return ret;
}


int compare(const char *file1, const char *file2) {
#ifdef ZOJ_COM
    //compare ported and improved from zoj don't limit file size
    return compare_zoj(file1, file2);
#endif
#ifndef ZOJ_COM
    //the original compare from the first version of hustoj has file size limit
    //and waste memory
    FILE *f1,*f2;
    char *s1,*s2,*p1,*p2;
    int PEflg;
    s1=new char[STD_F_LIM+512];
    s2=new char[STD_F_LIM+512];
    if (!(f1=fopen(file1,"re")))
        return OJ_AC;
    for (p1=s1;EOF!=fscanf(f1,"%s",p1);)
        while (*p1) p1++;
    fclose(f1);
    if (!(f2=fopen(file2,"re")))
        return OJ_RE;
    for (p2=s2;EOF!=fscanf(f2,"%s",p2);)
        while (*p2) p2++;
    fclose(f2);
    if (strcmp(s1,s2)!=0) {
        //              printf("A:%s\nB:%s\n",s1,s2);
        delete[] s1;
        delete[] s2;

        return OJ_WA;
    } else {
        f1=fopen(file1,"re");
        f2=fopen(file2,"re");
        PEflg=0;
        while (PEflg==0 && fgets(s1,STD_F_LIM,f1) && fgets(s2,STD_F_LIM,f2)) {
            delnextline(s1);
            delnextline(s2);
            if (strcmp(s1,s2)==0) continue;
            else PEflg=1;
        }
        delete [] s1;
        delete [] s2;
        fclose(f1);fclose(f2);
        if (PEflg) return OJ_PE;
        else return OJ_AC;
    }
#endif
}


/* write result back to database */
void _update_solution_mysql(int solution_id, int result, double time, int memory,
                            int sim, int sim_s_id, double pass_rate) {
    char sql[BUFFER_SIZE];
    if (ALL_TEST_MODE) {
        sprintf(sql,
                "UPDATE solution SET result=%d,time=%f,memory=%d,pass_rate=%f,judger='%s' WHERE solution_id=%d LIMIT 1%c",
                result, time, memory, pass_rate, http_username, solution_id, 0);
    } else {
        sprintf(sql,
                "UPDATE solution SET result=%d,time=%f,memory=%d,judger='%s' WHERE solution_id=%d LIMIT 1%c",
                result, time, memory, http_username, solution_id, 0);
    }
    //      printf("sql= %s\n",sql);
    if (conn.query(conn, sql, strlen(sql))) {
        printf("..update failed! %s\n", mysql_error(conn));
    }
    if (sim) {
        sprintf(sql,
                "insert into sim(s_id,sim_s_id,sim) values(%d,%d,%d) on duplicate key update  sim_s_id=%d,sim=%d",
                solution_id, sim_s_id, sim, sim_s_id, sim);
        //      printf("sql= %s\n",sql);
        if (conn.query(conn, sql, strlen(sql))) {
            //              printf("..update failed! %s\n",mysql_error(conn));
        }

    }

}

void update_solution(int solution_id, int result, double time, int memory, int sim,
                     int sim_s_id, double pass_rate) {
    if (NO_RECORD) {
        return;
    }
    if (result == TIME_LIMIT_EXCEEDED && memory == ZERO_MEMORY)
        result = MEMORY_LIMIT_EXCEEDED;
    _update_solution_mysql(solution_id, result, time, memory, sim, sim_s_id,
                           pass_rate);
}

/* write compile error message back to database */
void _addceinfo_mysql(int solution_id) {
    char sql[(1 << 16)], *end;
    char ceinfo[(1 << 16)], *cend;
    FILE *fp = fopen("ce.txt", "re");
    snprintf(sql, (1 << 16) - 1, "DELETE FROM compileinfo WHERE solution_id=%d",
             solution_id);
    conn.query(conn, sql, strlen(sql));
    cend = ceinfo;
    while (fgets(cend, 1024, fp)) {
        cend += strlen(cend);
        if (cend - ceinfo > 4096)
            break;
    }
    cend = nullptr;
    end = sql;
    strcpy(end, "INSERT INTO compileinfo VALUES(");
    end += strlen(sql);
    *end++ = '\'';
    end += sprintf(end, "%d", solution_id);
    *end++ = '\'';
    *end++ = ',';
    *end++ = '\'';
    end += mysql_real_escape_string(conn, end, ceinfo, strlen(ceinfo));
    *end++ = '\'';
    *end++ = ')';
    *end = 0;
    //      printf("%s\n",ceinfo);
    if (conn.query(conn, sql, end - sql))
        printf("%s\n", mysql_error(conn));
    fclose(fp);
}


void addceinfo(int solution_id) {
    _addceinfo_mysql(solution_id);
}

/* write runtime error message back to database */
void _addreinfo_mysql(int solution_id, const char *filename) {
    char sql[(1 << 16)], *end;
    char reinfo[(1 << 16)], *rend;
    FILE *fp = fopen(filename, "re");
    snprintf(sql, (1 << 16) - 1, "DELETE FROM runtimeinfo WHERE solution_id=%d",
             solution_id);
    conn.query(conn, sql, strlen(sql));
    rend = reinfo;
    while (fgets(rend, 1024, fp)) {
        rend += strlen(rend);
        if (rend - reinfo > 4096)
            break;
    }
    rend = nullptr;
    end = sql;
    strcpy(end, "INSERT INTO runtimeinfo VALUES(");
    end += strlen(sql);
    *end++ = '\'';
    end += sprintf(end, "%d", solution_id);
    *end++ = '\'';
    *end++ = ',';
    *end++ = '\'';
    end += mysql_real_escape_string(conn, end, reinfo, strlen(reinfo));
    *end++ = '\'';
    *end++ = ')';
    *end = 0;
    //      printf("%s\n",ceinfo);
    if (conn.query(conn, sql, end - sql))
        printf("%s\n", mysql_error(conn));
    fclose(fp);
}

void add_reinfo_mysql_by_string(int solution_id, string content) {
    string sql = "DELETE FROM runtimeinfo WHERE solution_id = " + to_string(solution_id);
    conn.query(conn, sql.c_str(), sql.length());
    if (content.length() > 4096) {
        content = content.substr(0, 4096);
    }
    sql = "INSERT INTO runtimeinfo VALUES('" + to_string(solution_id) + "','";
    char from_buffer[4192], to_buffer[4192];
    strcpy(from_buffer, content.c_str());
    mysql_real_escape_string(conn, to_buffer, from_buffer, content.length());
    sql += to_buffer;
    sql += "')";
    if (DEBUG) {
        cout << "SQL: " << sql << endl;
    }
    if (conn.query(conn, sql.c_str(), sql.length())) {
        cerr << "MYSQL error:" << mysql_error(conn) << endl;
    }
}


void addreinfo(int solution_id) {
    _addreinfo_mysql(solution_id, "error.out");
}

void adddiffinfo(int solution_id) {
    _addreinfo_mysql(solution_id, "diff.out");
}

void addcustomout(int solution_id) {
    _addreinfo_mysql(solution_id, "user.out");
}

void _update_user_mysql(char *user_id) {
    char sql[BUFFER_SIZE];
    sprintf(sql,
            R"(UPDATE `users` SET `solved`=(SELECT count(DISTINCT `problem_id`) FROM `solution` WHERE `user_id`='%s' AND `result`='4') WHERE `user_id`='%s')",
            user_id, user_id);
    if (conn.query(conn, sql, strlen(sql)))
        write_log(oj_home, mysql_error(conn));
    sprintf(sql,
            R"(UPDATE `users` SET `submit`=(SELECT count(*) FROM `solution` WHERE `user_id`='%s' and problem_id>0) WHERE `user_id`='%s')",
            user_id, user_id);
    if (conn.query(conn, sql, strlen(sql)))
        write_log(oj_home, mysql_error(conn));
}


void update_user(char *user_id) {
    _update_user_mysql(user_id);
}

void _update_problem_mysql(int p_id) {
    char sql[BUFFER_SIZE];
    sprintf(sql,
            R"(UPDATE `problem` SET `accepted`=(SELECT count(*) FROM `solution` WHERE `problem_id`='%d' AND `result`='4') WHERE `problem_id`='%d')",
            p_id, p_id);
    if (conn.query(conn, sql, strlen(sql)))
        write_log(oj_home, mysql_error(conn));
    sprintf(sql,
            R"(UPDATE `problem` SET `submit`=(SELECT count(*) FROM `solution` WHERE `problem_id`='%d') WHERE `problem_id`='%d')",
            p_id, p_id);
    if (conn.query(conn, sql, strlen(sql)))
        write_log(oj_home, mysql_error(conn));
}

void update_problem(int pid) {
    _update_problem_mysql(pid);
}


int compile(int lang, char *work_dir) {
    int pid;
    Bundle bundle;
    bundle.setSolutionID(solution_id);
    bundle.setResult(COMPILING);
    bundle.setFinished(NOT_FINISHED);
    bundle.setUsedTime(ZERO_TIME);
    bundle.setMemoryUse(ZERO_MEMORY);
    bundle.setPassPoint(ZERO_PASSPOINT);
    bundle.setPassRate(ZERO_PASSRATE);
    webSocket << bundle.toJSONString();
    //webSocket << ws_send(solution_id, 2, NOT_FINISHED, ZERO_TIME, ZERO_MEMORY, ZERO_PASSPOINT, ZERO_PASSRATE);
    string configJSONDir = oj_home;
    configJSONDir += "/etc/compile.json";
    CompilerArgsReader compilerArgsReader(configJSONDir);
    pid = fork();
    if (pid == CHILD_PROCESS) {
        struct rlimit LIM{};
        LIM.rlim_max = 60;
        LIM.rlim_cur = 60;
        setrlimit(RLIMIT_CPU, &LIM);
        int cpu_alarm_limit = 10;
        if (lang == JAVA || lang == JAVA7 || lang == JAVA8 || lang == JAVA6) {
            cpu_alarm_limit = 30;
        }
        alarm(static_cast<unsigned int>(cpu_alarm_limit));
        LIM.rlim_max = static_cast<rlim_t>(10 * COMPILE_STD_MB);
        LIM.rlim_cur = static_cast<rlim_t>(10 * COMPILE_STD_MB);
        setrlimit(RLIMIT_FSIZE, &LIM);

        if (lang == JAVA || lang == GO || lang == JAVA8 || lang == JAVA7 || lang == JAVA6) {
            LIM.rlim_max = static_cast<rlim_t>(COMPILE_STD_MB << 11);
            LIM.rlim_cur = static_cast<rlim_t>(COMPILE_STD_MB << 11);
        } else {
            LIM.rlim_max = static_cast<rlim_t>(COMPILE_STD_MB * 256);
            LIM.rlim_cur = static_cast<rlim_t>(COMPILE_STD_MB * 256);
        }
        if (lang != JAVA && lang != JAVA8 && lang != JAVA7 && lang != JAVA6) {
            setrlimit(RLIMIT_AS, &LIM);
        }
        if (lang != PASCAL && lang != FREEBASIC) {
            freopen("ce.txt", "w", stderr);
            //freopen("/dev/null", "w", stdout);
        } else {
            freopen("ce.txt", "w", stdout);
        }
        if (lang != CPP17 && lang != JAVA && lang != 9 && lang != FREEBASIC
             && lang != JAVA7 && lang != JAVA8 && lang != JAVA6) {
            execute_cmd("mkdir -p bin usr lib lib64 etc/alternatives proc tmp dev");
            execute_cmd("chown judge *");
            execute_cmd("mount -o bind /bin bin");
            execute_cmd("mount -o bind /usr usr");
            execute_cmd("mount -o bind /lib lib");
#ifndef __i386
            execute_cmd("mount -o bind /lib64 lib64");
#endif
            execute_cmd("mount -o bind /etc/alternatives etc/alternatives");
            execute_cmd("mount -o bind /proc proc");
            if (lang > PASCAL && lang != OBJC && lang != CLANG && lang != CLANGPP && lang != CLANG11 &&
                lang != CLANGPP17 && lang != CPP11 &&
                lang != CPP98 &&
                lang != C99)
                execute_cmd("mount -o bind /dev dev");
            chroot(work_dir);
            //chroot(work_dir);
        }
        while (setgid(1536) != 0)
            sleep(1);
        while (setuid(1536) != 0)
            sleep(1);
        while (setresuid(1536, 1536, 1536) != 0)
            sleep(1);
        if (DEBUG)
            cout << "Lang:" << lang << endl;
        if (isJava(lang)) {
            auto _args = compilerArgsReader.GetArray(to_string(lang));
            int len = _args.size();
            char *java_arg[len + 5];
            char java_buffer[len + 5][30];
            for (int i = 0; i < len; ++i) {
                memset(java_buffer[i], 0, sizeof(java_buffer[i]));
                memcpy(java_buffer[i], _args[i].c_str(), _args[i].length());
                java_arg[i] = java_buffer[i];
            }
            java_arg[len] = nullptr;
            sprintf(java_buffer[1], "-J%s", java_xms);
            sprintf(java_buffer[2], "-J%s", java_xmx);
            execvp(java_arg[0], (char *const *) java_arg);
        } else {
            vector<string> _args = compilerArgsReader.GetArray(to_string(lang));
            if (_args.empty()) {
                cout << "Noting to do" << endl;
                exit(0);
            }
            int len = static_cast<int>(_args.size());
            const char *args[len + 5];
            for (int i = 0; i < len; ++i) {
                args[i] = _args[i].c_str();
            }
            args[len] = nullptr;
            if (DEBUG) {
                cout << args[0] << endl;
                for (int i = 1; i < len; ++i) {
                    cout << args[i] << " ";
                }
                cout << endl;
            }
            execvp(args[0], (char *const *) args);
        }
        if (DEBUG)
            printf("compile end!\n");
        //exit(!system("cat ce.txt"));
        exit(0);
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        if (lang > JAVA && lang < PHP)
            status = static_cast<int>(get_file_size("ce.txt"));
        if (DEBUG)
            printf("status=%d\n", status);
        execute_cmd("/bin/umount -f bin usr lib lib64 etc/alternatives proc dev 2>&1 >/dev/null");
        execute_cmd("/bin/umount -f %s/* 2>&1 >/dev/null", work_dir);
        umount(work_dir);
        return status;
    }

}

/*
 int read_proc_statm(int pid){
 FILE * pf;
 char fn[4096];
 int ret;
 sprintf(fn,"/proc/%d/statm",pid);
 pf=fopen(fn,"r");
 fscanf(pf,"%d",&ret);
 fclose(pf);
 return ret;
 }
 */
int get_proc_status(int pid, const char *mark) {
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
    return ret;
}


void get_solution(int solution_id, char *work_dir, int lang, char *usercode) {
    char sql[BUFFER_SIZE], src_pth[BUFFER_SIZE];
    // get the source code
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(sql, "SELECT source FROM source_code WHERE solution_id=%d",
            solution_id);
    conn.query(conn, sql, strlen(sql));
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    sprintf(usercode, "%s", row[0]);
    // create the src file
    sprintf(src_pth, "Main.%s", lang_ext[lang]);
    if (DEBUG) {
        printf("Main=%s", src_pth);
        cout << usercode << endl;
    }
    FILE *fp_src = fopen(src_pth, "we");
    fprintf(fp_src, "%s", row[0]);
    mysql_free_result(res);
    fclose(fp_src);
}


void get_custominput(int solution_id, char *work_dir) {
    char sql[BUFFER_SIZE], src_pth[BUFFER_SIZE];
    // get the source code
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(sql, "SELECT input_text FROM custominput WHERE solution_id=%d",
            solution_id);
    conn.query(conn, sql, strlen(sql));
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    if (row != nullptr) {

        // create the src file
        sprintf(src_pth, "data.in");
        FILE *fp_src = fopen(src_pth, "w");
        fprintf(fp_src, "%s", row[0]);
        fclose(fp_src);

    }
    mysql_free_result(res);
}


void get_solution_info(int solution_id, int &p_id, char *user_id,
                       int &lang) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[BUFFER_SIZE];
    // get the problem id and user id from Table:solution
    sprintf(sql,
            "SELECT problem_id, user_id, language FROM solution where solution_id=%d",
            solution_id);
    //printf("%s\n",sql);
    conn.query(conn, sql, strlen(sql));
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    p_id = atoi(row[0]);
    strcpy(user_id, row[1]);
    lang = atoi(row[2]);
    mysql_free_result(res);
}


void get_problem_info(int p_id, double &time_lmt, int &mem_lmt, int &isspj) {
    char sql[BUFFER_SIZE];
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(sql,
            "SELECT time_limit,memory_limit,spj FROM problem where problem_id=%d",
            p_id);
    conn.query(conn, sql, strlen(sql));
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    time_lmt = atof(row[0]);
    mem_lmt = atoi(row[1]);
    isspj = (row[2][0] == '1');
    mysql_free_result(res);
    if (time_lmt <= 0)
        time_lmt = 1;

}

void prepare_files(const char *filename, int namelen, char *infile, int &p_id,
                   char *work_dir, char *outfile, char *userfile, int runner_id) {
    //              printf("ACflg=%d %d check a file!\n",ACflg,solution_id);

    char fname0[BUFFER_SIZE];
    char fname[BUFFER_SIZE];
    strncpy(fname0, filename, static_cast<size_t>(namelen));
    fname0[namelen] = 0;
    escape(fname, fname0);
    printf("%s\n%s\n", fname0, fname);
    sprintf(infile, "%s/data/%d/%s.in", oj_home, p_id, fname);
    execute_cmd("/bin/cp '%s' %s/data.in", infile, work_dir);
    execute_cmd("/bin/cp %s/data/%d/*.dic %s/", oj_home, p_id, work_dir);

    sprintf(outfile, "%s/data/%d/%s.out", oj_home, p_id, fname0);
    sprintf(userfile, "%s/run%d/user.out", oj_home, runner_id);
}

void run_solution(int &lang, char *work_dir, double &time_lmt, double &usedtime,
                  int &mem_lmt) {
    nice(19);
    // now the user is "judger"
    chdir(work_dir);
    // open the files
    freopen("data.in", "r", stdin);
    //if(!DEBUG)
    //{
    freopen("user.out", "w", stdout);
    freopen("error.out", "a+", stderr);

    //}
    // trace me
    if (use_ptrace)
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    // run me
    if (!isJava(lang)) {
        chroot(work_dir);
    }

    while (setgid(1536) != 0)
        sleep(1);
    while (setuid(1536) != 0)
        sleep(1);
    while (setresuid(1536, 1536, 1536) != 0)
        sleep(1);

    //      char java_p1[BUFFER_SIZE], java_p2[BUFFER_SIZE];
    // child
    // set the limit
    struct rlimit LIM{}; // time limit, file limit& memory limit
    // time limit
    if (ALL_TEST_MODE)
        LIM.rlim_cur = static_cast<rlim_t>(time_lmt + 1);
    else
        LIM.rlim_cur = static_cast<rlim_t>((time_lmt - usedtime / 1000) + 1);
    LIM.rlim_max = LIM.rlim_cur;
    //if(DEBUG) printf("LIM_CPU=%d",(int)(LIM.rlim_cur));
    setrlimit(RLIMIT_CPU, &LIM);
    alarm(0);
    // alarm(time_lmt * 10);
    alarm(static_cast<unsigned int>(time_lmt * 10));

    // file limit
    LIM.rlim_max = ((STD_F_LIM << 2) + STD_MB);
    LIM.rlim_cur = (STD_F_LIM << 2);
    setrlimit(RLIMIT_FSIZE, &LIM);
    // proc limit
    switch (lang) {
        case GO:
            LIM.rlim_cur = LIM.rlim_max = 280;
            break;
        case JAVA:  //java
        case RUBY:  //ruby
        case CSHARP: //C#
        case SCHEMA:
        case JAVASCRIPT:
        case JAVA7:
        case JAVA8:
        case JAVA6:
            LIM.rlim_cur = LIM.rlim_max = 80;
            break;
        case BASH: //bash
            LIM.rlim_cur = LIM.rlim_max = 3;
            break;
        default:
            LIM.rlim_cur = LIM.rlim_max = 1;
    }

    setrlimit(RLIMIT_NPROC, &LIM);

    // set the stack
    LIM.rlim_cur = static_cast<rlim_t>(STD_MB << 7);
    LIM.rlim_max = static_cast<rlim_t>(STD_MB << 7);
    setrlimit(RLIMIT_STACK, &LIM);
    // set the memory
    LIM.rlim_cur = static_cast<rlim_t>(STD_MB * mem_lmt / 2 * 3);
    LIM.rlim_max = static_cast<rlim_t>(STD_MB * mem_lmt * 2);
    if (lang < JAVA || (lang >= CLANG && lang <= CLANGPP) || (lang >= CPP11 && lang <= C99) || lang == CLANG11 ||
        lang == CLANGPP17) {
        setrlimit(RLIMIT_AS, &LIM);
    }

    switch (lang) {
        case C11:
        case CPP17:
        case PASCAL:
        case OBJC:
        case FREEBASIC:
        case CLANG:
        case CLANGPP:
        case CLANG11:
        case CLANGPP17:
        case GO:
        case CPP11:
        case CPP98:
        case C99:
            execl("./Main", "./Main", (char *) nullptr);
            break;
        case JAVA:
            sprintf(java_xms, "-Xmx%dM", mem_lmt);
            sprintf(java_xmx, "-XX:MaxMetaspaceSize=%dM", mem_lmt);
            execl("/usr/bin/java", "/usr/bin/java", java_xms, java_xmx,
                  "-Djava.security.manager",
                  "-Djava.security.policy=./java.policy", "Main", (char *) nullptr);
            break;
        case JAVA7:
            sprintf(java_xms, "-Xmx%dM", mem_lmt);
            sprintf(java_xmx, "-XX:MaxPermSize=%dM", mem_lmt);
            execl("/usr/bin/java-7", "/usr/bin/java-7", java_xms, java_xmx,
                  "-Djava.security.manager",
                  "-Djava.security.policy=./java.policy", "Main", (char *) nullptr);
            break;
        case JAVA8:
            sprintf(java_xms, "-Xmx%dM", mem_lmt);
            sprintf(java_xmx, "-XX:MaxMetaspaceSize=%dM", mem_lmt);
            execl("/usr/bin/java-8", "/usr/bin/java-8", java_xms, java_xmx,
                  "-Djava.security.manager",
                  "-Djava.security.policy=./java.policy", "Main", (char *) nullptr);
            break;
        case JAVA6:
            sprintf(java_xms, "-Xmx%dM", mem_lmt);
            sprintf(java_xmx, "-XX:MaxPermSize=%dM", mem_lmt);
            execl("/usr/bin/java-6", "/usr/bin/java-6", java_xms, java_xmx,
                  "-Djava.security.manager",
                  "-Djava.security.policy=./java.policy", "Main", (char *) nullptr);
            break;
        case RUBY:
            //system("/ruby Main.rb<data.in");
            execl("/ruby", "/ruby", "Main.rb", (char *) nullptr);
            break;
        case BASH: //bash
            execl("/bin/bash", "/bin/bash", "Main.sh", (char *) nullptr);
            break;
        case PYTHON2: //Python
            execl("/python2", "/python2", "Main.py", (char *) nullptr);
            break;
        case PHP: //php
            execl("/php", "/php", "Main.php", (char *) nullptr);
            break;
        case PERL: //perl
            execl("/perl", "/perl", "Main.pl", (char *) nullptr);
            break;
        case CSHARP: //Mono C#
            execl("/mono", "/mono", "--debug", "Main.exe", (char *) nullptr);
            break;
        case SCHEMA: //guile
            execl("/guile", "/guile", "Main.scm", (char *) nullptr);
            break;
        case LUA: //guile
            execl("/lua", "/lua", "Main", (char *) nullptr);
            break;
        case JAVASCRIPT: //SpiderMonkey
            execl("/node", "/node", "Main.js", (char *) nullptr);
            break;
        case PYTHON3://python 3
            //system("./python3 Main.py<data.in>>user.out");
            execl("/python3", "/python3", "Main.py", (char *) nullptr);
            break;
        case PyPy:
            execl("/pypy/bin/pypy", "/pypy/bin/pypy", "Main.py", (char *) nullptr);
            break;
        case PyPy3:
            execl("/pypy3/bin/pypy3", "/pypy3/bin/pypy3", "Main.py", (char *) nullptr);
            break;
        default:
            break;
    }
    //sleep(1);
    fflush(stderr);
    exit(0);
}


int special_judge(char *oj_home, int problem_id, char *infile, char *outfile,
                  char *userfile, char *usercode, string& global_work_dir) {

    pid_t pid;
    printf("pid=%d\n", problem_id);
    /*
    string fullpath = oj_home;
    fullpath += string("/data/") + to_string(problem_id);
    auto dp = opendir(fullpath.c_str());
    dirent *dirp;
    bool isWindowsSpecialJudge = false;
    if (dp == nullptr) {

    } else {
        for (; dirp = readdir(dp);) {
            string filename(dirp->d_name);
            if (filename.find(".exe") != filename.npos) {
                isWindowsSpecialJudge = true;
            }
        }
    }
     */
    printf("%s\n", usercode);
    string outfiled(global_work_dir);
    outfiled += "usercode.code";
    ofstream userout(outfiled);
    userout << usercode;
    userout.close();
    fstream users(userfile);
    string tmp;
    while (getline(users, tmp))
        cout << tmp << endl;
    pid = fork();
    int ret = 0;
    if (pid == CHILD_PROCESS) {
        /*
        if (false && !isWindowsSpecialJudge) {
            while (setgid(1536) != 0)
                sleep(1);
            while (setuid(1536) != 0)
                sleep(1);
            while (setresuid(1536, 1536, 1536) != 0)
                sleep(1);
        }
         */
        freopen("diff.out", "a", stdout);

        struct rlimit LIM{}; // time limit, file limit& memory limit

        LIM.rlim_cur = FIVE;
        LIM.rlim_max = LIM.rlim_cur;
        setrlimit(RLIMIT_CPU, &LIM);
        alarm(ZERO);
        alarm(10);

        // file limit
        LIM.rlim_max = static_cast<rlim_t>(STD_F_LIM + STD_MB);
        LIM.rlim_cur = static_cast<rlim_t>(STD_F_LIM);
        setrlimit(RLIMIT_FSIZE, &LIM);
        LIM.rlim_cur = LIM.rlim_max = 50 * STD_MB;
        setrlimit(RLIMIT_FSIZE, &LIM);
        LIM.rlim_cur = LIM.rlim_max = 1024 * STD_MB;
        setrlimit(RLIMIT_AS, &LIM);
        string dir = oj_home;
        dir += "/data/" + to_string(problem_id) + "/";
        if (~access((dir + "spj").c_str(), 0)) {
            LIM.rlim_max = LIM.rlim_cur = 1;
            setrlimit(RLIMIT_NPROC, &LIM);
            ret = execute_cmd((dir + "spj %s %s %s %s").c_str(),
                              infile, outfile, userfile, outfiled.c_str());
        } else if (~access((dir + "spj.js").c_str(), 0)) {
            LIM.rlim_cur = LIM.rlim_max = 80;
            setrlimit(RLIMIT_NPROC, &LIM);
            ret = execute_cmd(("node " + dir + "spj.js %s %s %s %s").c_str(),
                              infile, outfile, userfile, outfiled.c_str());
        } else if (~access((dir + "spj.py").c_str(), 0)) {
            LIM.rlim_max = LIM.rlim_cur = 1;
            ret = execute_cmd(("python3 " + dir + "spj.py %s %s %s %s").c_str(),
                              infile, outfile, userfile, outfiled.c_str());
        }
        freopen("/dev/tty", "w", stdout);
        if (DEBUG) {
            if (get_file_size("diff.out")) {
                ifstream spjout("diff.out");
                string s;
                while (getline(spjout, s)) {
                    cout << s << endl;
                }
            }
        }
        //cout << "Debug return code:" << ret << endl;
        if (WIFEXITED(ret)) {
            cout << "WIFEXITED:" << WIFEXITED(ret) << endl;
        } else {
            cout << "ERROR WIFEXITED:" << WIFEXITED(ret) << endl;
        }
        if (DEBUG)
            printf("spj1=%d\n", ret);
        if (ret)
            ret = WEXITSTATUS(ret);
        if (ret && ret < ACCEPT) {
            ret = WRONG_ANSWER;
        }
        exit(ret);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (DEBUG) {
            cout << "status:" << status << endl;
        }
        ret = WEXITSTATUS(status);
        if (DEBUG)
            printf("spj2=%d\n", ret);
    }
    return ret;
}

void fix_python_syntax_error_response(int &ACflg, int lang) {
    if (ACflg != ACCEPT) return;
    if (lang == PYTHON2 || lang == PYTHON3 || lang == PyPy || lang == PyPy3) {
        cerr << "Try to get sizeof error.out" << endl;
        auto error_size = get_file_size("error.out");
        cerr << "Error size:" << error_size << endl;
        if (error_size > 0) {
            fstream ferr("error.out");
            string tmp, content;
            while (getline(ferr, tmp)) {
                content += tmp;
            }
            if (content.find("SyntaxError") != content.npos) {
                ACflg = RUNTIME_ERROR;
            }
        }
    }
}

void judge_solution(int &ACflg, double &usedtime, double time_lmt, int isspj,
                    int p_id, char *infile, char *outfile, char *userfile, char *usercode, int &PEflg,
                    int lang, char *work_dir, int &topmemory, int mem_lmt,
                    int solution_id, int num_of_test, string& global_work_dir) {
    //usedtime-=1000;
    cout << "Used time" << endl;
    cout << usedtime << endl;
    cout << time_lmt * 1000 * (use_max_time ? 1 : num_of_test) << endl;
    int comp_res;
    if (!ALL_TEST_MODE)
        num_of_test = static_cast<int>(1.0);
    if (ACflg == ACCEPT
        && usedtime > time_lmt * 1000 * (use_max_time ? 1 : num_of_test)) {
        cout << "Time Limit Exceeded" << endl;
        usedtime = time_lmt * 1000;
        ACflg = TIME_LIMIT_EXCEEDED;
    }
    if (topmemory > mem_lmt * STD_MB)
        ACflg = MEMORY_LIMIT_EXCEEDED; //issues79
    fix_python_syntax_error_response(ACflg, lang);
    // compare
    if (ACflg == ACCEPT) {
        if (isspj) {
            comp_res = special_judge(oj_home, p_id, infile, outfile, userfile, usercode, global_work_dir);
            if (comp_res < 4) {
                if (comp_res == 0)
                    comp_res = ACCEPT;
                else {
                    if (DEBUG) {
                        cout << "Fail test " << infile << endl;
                    }
                    comp_res = WRONG_ANSWER;
                }
            }
        } else {
            comp_res = compare(outfile, userfile);
        }
        if (comp_res == WRONG_ANSWER) {
            ACflg = WRONG_ANSWER;
            if (DEBUG)
                printf("fail test %s\n", infile);
        } else if (comp_res == PRESENTATION_ERROR)
            PEflg = PRESENTATION_ERROR;
        ACflg = comp_res;
    }
    //jvm popup messages, if don't consider them will get miss-WrongAnswer
    if (lang == JAVA || lang == JAVA7 || lang == JAVA8 || lang == JAVA6) {
        fix_java_mis_judge(work_dir, ACflg, topmemory, mem_lmt);
    }
    if (lang == PYTHON2 || lang == PYTHON3 || lang == PyPy || lang == PyPy3) {
        fix_python_mis_judge(work_dir, ACflg, topmemory, mem_lmt);
    }
}

int get_page_fault_mem(struct rusage &ruse, pid_t &pidApp) {
    //java use pagefault
    //int m_vmpeak, m_vmdata;
    int m_minflt;
    m_minflt = static_cast<int>(ruse.ru_minflt * getpagesize());
    /*
    if (0 && DEBUG) {
        m_vmpeak = get_proc_status(pidApp, "VmPeak:");
        m_vmdata = get_proc_status(pidApp, "VmData:");
        printf("VmPeak:%d KB VmData:%d KB minflt:%d KB\n", m_vmpeak, m_vmdata,
               m_minflt >> 10);
    }
     */
    return m_minflt;
}

void print_runtimeerror(const char *err) {
    FILE *ferr = fopen("error.out", "a+");
    fprintf(ferr, "Runtime Error:%s\n", err);
    fclose(ferr);
}


void watch_solution(pid_t pidApp, char *infile, int &ACflg, int isspj,
                    char *userfile, char *outfile, int solution_id, int lang,
                    int &topmemory, int mem_lmt, double &usedtime, double time_lmt, int &p_id,
                    int &PEflg, char *work_dir) {
    // parent
    int tempmemory;

    if (DEBUG)
        printf("pid=%d judging %s\n", pidApp, infile);

    int status, sig, exitcode;
    struct user_regs_struct reg{};
    struct rusage ruse{};
    if (topmemory == 0)
        topmemory = get_proc_status(pidApp, "VmRSS:") << 10;
    while (true) {
        // check the usage

        wait4(pidApp, &status, 0, &ruse);

        //jvm gc ask VM before need,so used kernel page fault times and page size
        if (lang == JAVA || lang == PHP ||
            lang == JAVASCRIPT || lang == CSHARP ||
            lang == GO || lang == JAVA7 || lang == JAVA8 || lang == JAVA6 || lang == CLANG || lang == CLANGPP ||
            lang == CLANG11 || lang == CLANGPP17) {
            tempmemory = get_page_fault_mem(ruse, pidApp);
        } else {        //other use VmPeak
            tempmemory = get_proc_status(pidApp, "VmPeak:") << 10;
        }
        topmemory = max(tempmemory, topmemory);
        if (topmemory > mem_lmt * STD_MB) {
            if (DEBUG)
                printf("out of memory %d\n", topmemory);
            if (ACflg == ACCEPT)
                ACflg = MEMORY_LIMIT_EXCEEDED;
            ptrace(PTRACE_KILL, pidApp, NULL, NULL);
            break;
        }
        //sig = status >> 8;/*status >> 8 EXITCODE*/

        if (WIFEXITED(status))
            break;
        if (get_file_size("error.out")) {
            if (DEBUG) {
                cerr << "Catch error:" << endl;
                string tmp;
                fstream err("error.out");
                while (getline(err, tmp))
                    cerr << tmp << endl;
            }
            //ACflg = OJ_RE;
            fstream file("error.out", ios::ate);
            stringstream buffer;
            buffer << file.rdbuf();
            string contents(buffer.str());
            if (contents.find("Killed") != contents.npos) {
                write_log(oj_home, contents.c_str());
                print_runtimeerror(contents.c_str());
                //ptrace(PTRACE_KILL, pidApp, NULL, NULL);
                //print_runtimeerror(contents.c_str());
                if (!ALL_TEST_MODE)
                    break;
            }
        }
        if ((lang < RUBY || lang == CSHARP) && get_file_size("error.out") && !ALL_TEST_MODE) {
            ACflg = RUNTIME_ERROR;
            //addreinfo(solution_id);
            ptrace(PTRACE_KILL, pidApp, NULL, NULL);
            break;
        }


        if (!isspj
            && get_file_size(userfile)
               > get_file_size(outfile) * 2 + 1024) {
            ACflg = OUTPUT_LIMIT_EXCEEDED;
            ptrace(PTRACE_KILL, pidApp, NULL, NULL);
            break;
        }

        exitcode = WEXITSTATUS(status);
        /*exitcode == 5 waiting for next CPU allocation          * ruby using system to run,exit 17 ok
         *  */
        if ((lang >= JAVA && exitcode == 17) || exitcode == 0x05 || exitcode == 0)
            //go on and on
            ;
        else {

            if (DEBUG) {
                printf("status>>8=%d\n", exitcode);

            }
            //psignal(exitcode, NULL);

            if (ACflg == ACCEPT) {
                switch (exitcode) {
                    case SIGCHLD:
                    case SIGALRM:
                        alarm(0);
                    case SIGKILL:
                    case SIGXCPU:
                        ACflg = TIME_LIMIT_EXCEEDED;
                        break;
                    case SIGXFSZ:
                        ACflg = OUTPUT_LIMIT_EXCEEDED;
                        break;
                    default:
                        ACflg = RUNTIME_ERROR;
                }
                print_runtimeerror(strsignal(exitcode));
            }
            ptrace(PTRACE_KILL, pidApp, NULL, NULL);
            break;
        }
        if (WIFSIGNALED(status)) {
            /*  WIFSIGNALED: if the process is terminated by signal
             *
             *  psignal(int sig, char *s)，like perror(char *s)，print out s, with error msg from system of sig
             * sig = 5 means Trace/breakpoint trap
             * sig = 11 means Segmentation fault
             * sig = 25 means File size limit exceeded
             */
            sig = WTERMSIG(status);
            if (DEBUG) {
                printf("WTERMSIG=%d\n", sig);
                psignal(sig, nullptr);
            }
            if (ACflg == ACCEPT) {
                switch (sig) {
                    case SIGCHLD:
                    case SIGALRM:
                        alarm(0);
                    case SIGKILL:
                    case SIGXCPU:
                        ACflg = TIME_LIMIT_EXCEEDED;
                        break;
                    case SIGXFSZ:
                        ACflg = OUTPUT_LIMIT_EXCEEDED;
                        break;
                    default:
                        ACflg = RUNTIME_ERROR;
                }
                print_runtimeerror(strsignal(sig));
            }
            break;
        }
        /*     comment from http://www.felix021.com/blog/read.php?1662

         WIFSTOPPED: return true if the process is paused or stopped while ptrace is watching on it
         WSTOPSIG: get the signal if it was stopped by signal
         */

        // check the system calls
        ptrace(PTRACE_GETREGS, pidApp, NULL, &reg);
        if (call_counter[reg.REG_SYSCALL]) {
            //call_counter[reg.REG_SYSCALL]--;
        } else if (record_call) {
            call_counter[reg.REG_SYSCALL] = 1;

        } else { //do not limit JVM syscall for using different JVM
            ACflg = RUNTIME_ERROR;
            string _error;
            _error = string("Current Program use not allowed system call.\nSolution ID:") + to_string(solution_id) +
                     "\n";
            _error += string("Syscall ID:") + to_string(reg.REG_SYSCALL) + "\n";

            write_log(oj_home, _error.c_str());
            print_runtimeerror(_error.c_str());
            ptrace(PTRACE_KILL, pidApp, NULL, NULL);
        }
        ptrace(PTRACE_SYSCALL, pidApp, NULL, NULL);
    }

    usedtime += (ruse.ru_utime.tv_sec * 1000 + ruse.ru_utime.tv_usec / 1000);
    usedtime += (ruse.ru_stime.tv_sec * 1000 + ruse.ru_stime.tv_usec / 1000);

    //clean_session(pidApp);
}


void init_parameters(int argc, char **argv, int &solution_id,
                     int &runner_id) {
    if (argc < 3) {
        fprintf(stderr, "Usage:%s solution_id runner_id.\n", argv[0]);
        fprintf(stderr, "Multi:%s solution_id runner_id judge_base_path.\n",
                argv[0]);
        fprintf(stderr,
                "Debug:%s solution_id runner_id judge_base_path debug.\n",
                argv[0]);
        exit(1);
    }
    bool error = false;
    for (int i = 1; i < argc; ++i) {
        int argType = detectArgType(argv[i]);
        if (argType == _ERROR) {
            error = true;
            break;
        } else if (argType == _DEBUG) {
            DEBUG = true;
        } else if (argType == _NO_RECORD) {
            NO_RECORD = 1;
        } else if (argType == _RECORD_CALL) {
            record_call = 1;
        } else if (argType == _ADMIN) {
            admin = true;
        } else {
            ++i;
            if (i >= argc) {
                error = true;
                break;
            }
            switch (argType) {
                case _LANG_NAME:
                    strcpy(LANG_NAME, argv[i]);
                    break;
                case _DIR:
                    strcpy(oj_home, argv[i]);
                    break;
                case _SOLUTION_ID:
                    solution_id = atoi(argv[i]);
                    break;
                case _RUNNER_ID:
                    judger_number = runner_id = atoi(argv[i]);
                    break;
            }
        }
    }
    if (!error) {
        chdir(oj_home);
        return;
    }
    DEBUG = (argc > 4);
    if (argc > 5 && !strcmp(argv[5], "DEBUG")) {
        NO_RECORD = 1;
    } else {
        record_call = (argc > 5);
    }
    if (argc > 5) {
        strcpy(LANG_NAME, argv[5]);
    }
    if (argc > 3)
        strcpy(oj_home, argv[3]);
    else
        strcpy(oj_home, "/home/judge");

    chdir(oj_home); // change the dir// init our work

    solution_id = atoi(argv[1]);
    runner_id = atoi(argv[2]);
    judger_number = runner_id;
}

int get_sim(int solution_id, int lang, int pid, int &sim_s_id) {
    char src_pth[BUFFER_SIZE];
    //char cmd[BUFFER_SIZE];
    sprintf(src_pth, "Main.%s", lang_ext[lang]);
    if (DEBUG) {
        cout << "get sim: " << src_pth << endl;
    }
    int sim = 0;
    if (admin) {}
    else {
        sim = execute_cmd("/usr/bin/sim.sh %s %d", src_pth, pid);
    }
    if (!sim) {
        if (DEBUG) {
            cout << "SIM is not detected!" << endl;
        }
        execute_cmd("/bin/mkdir ../data/%d/ac/", pid);

        execute_cmd("/bin/cp %s ../data/%d/ac/%d.%s", src_pth, pid, solution_id,
                    lang_ext[lang]);
        //c cpp will
        if (lang == C11 || lang == C99)
            execute_cmd("/bin/ln -s ../data/%d/ac/%d.%s ../data/%d/ac/%d.%s", pid,
                        solution_id, lang_ext[0], pid, solution_id,
                        lang_ext[1]);
        if (lang == CPP17 || lang == CPP11 || lang == CPP98)
            execute_cmd("/bin/ln -s ../data/%d/ac/%d.%s ../data/%d/ac/%d.%s", pid,
                        solution_id, lang_ext[1], pid, solution_id,
                        lang_ext[0]);

    } else {

        FILE *pf;
        pf = fopen("sim", "r");
        if (pf) {
            fscanf(pf, "%d%d", &sim, &sim_s_id);
            fclose(pf);
        }
        if (DEBUG) {
            cout << "FIND SIM! sim:" << sim << " sim_s_id:" << sim_s_id << endl;
        }
    }

    MYSQL_RES *res;
    MYSQL_ROW row;
    string sql = "SELECT user_id FROM solution WHERE solution_id=" + to_string(solution_id);
    if (DEBUG) {
        cout << sql << endl;
    }
    conn.query(conn, sql.c_str(), sql.length());
    res = mysql_store_result(conn);
    string uid;
    row = mysql_fetch_row(res);
    if (row)
        uid = row[0];
    sql = "SELECT user_id FROM solution WHERE solution_id=" + to_string(sim_s_id);
    if (DEBUG) {
        cout << uid << endl;
    }
    conn.query(conn, sql.c_str(), sql.length());
    res = mysql_store_result(conn);
    string cpid;
    row = mysql_fetch_row(res);
    if (row)
        cpid = row[0];
    if (DEBUG) {
        cout << cpid << endl;
    }
    if (uid == cpid)
        sim = 0;
    //if (solution_id <= sim_s_id)
    //  sim = 0;

    return sim;
}

void mk_shm_workdir(char *work_dir) {
    char shm_path[BUFFER_SIZE];
    sprintf(shm_path, "/dev/shm/hustoj/%s", work_dir);
    execute_cmd("/bin/mkdir -p %s", shm_path);
    execute_cmd("/bin/ln -s %s %s/", shm_path, oj_home);
    execute_cmd("/bin/chown judge %s ", shm_path);
    execute_cmd("chmod 755 %s ", shm_path);
    //sim need a soft link in shm_dir to work correctly
    sprintf(shm_path, "/dev/shm/hustoj/%s/", oj_home);
    execute_cmd("/bin/ln -s %s/data %s", oj_home, shm_path);
}


void print_call_array() {
    printf("int LANG_%sV[256]={", LANG_NAME);
    int i = 0;
    for (i = 0; i < call_array_size; i++) {
        if (call_counter[i]) {
            printf("%d,", i);
        }
    }
    printf("0};\n");

    printf("int LANG_%sC[256]={", LANG_NAME);
    for (i = 0; i < call_array_size; i++) {
        if (call_counter[i]) {
            printf("HOJ_MAX_LIMIT,");
        }
    }
    printf("0};\n");

}


int main(int argc, char **argv) {
    webSocket.connect("ws://localhost:5100");
    char work_dir[BUFFER_SIZE];
    char usercode[CODESIZE];
    char user_id[BUFFER_SIZE];
    solution_id = DEFAULT_SOLUTION_ID;
    int runner_id = 0;
    int p_id, mem_lmt, lang, SPECIAL_JUDGE, sim, sim_s_id = ZERO_SIM;
    double max_case_time = ZERO_TIME;
    double time_lmt;
    init_parameters(argc, argv, solution_id, runner_id);
    init_mysql_conf();
    if (!conn.start()) {
        exit(0); //exit if mysql is down
    }
    //set work directory to start running & judging
    sprintf(work_dir, "%s/run%d/", oj_home, runner_id);
    string global_work_dir = string(work_dir);
    clean_workdir(work_dir);

    if (SHARE_MEMORY_RUN)
        mk_shm_workdir(work_dir);

    chdir(work_dir);
    get_solution_info(solution_id, p_id, user_id, lang);
    get_problem_info(abs(p_id), time_lmt, mem_lmt, SPECIAL_JUDGE);
    //get the limit
    if (p_id <= TEST_RUN_SUBMIT) {//Is custom input
        SPECIAL_JUDGE = NONE_SPECIAL_JUDGE;
    }
    //copy source file
    get_solution(solution_id, work_dir, lang, usercode);
    //java is lucky
    if ((lang >= JAVA && lang != OBJC && lang != CLANG && lang != CLANGPP && lang != CLANG11 && lang != CLANGPP17 &&
         lang < CPP11) || lang >= JAVA8) {  // Clang Clang++ not VM or Script
        // the limit for java
        time_lmt = time_lmt * java_time_bonus + java_time_bonus;
        mem_lmt = mem_lmt + java_memory_bonus;
        // copy java.policy
        if (isJava(lang)) {
            execute_cmd("/bin/cp %s/etc/java0.policy %s/java.policy", oj_home, work_dir);
            execute_cmd("chmod 755 %s/java.policy", work_dir);
            execute_cmd("chown judge %s/java.policy", work_dir);
        }
    }

    //never bigger than judged set value;
    if (time_lmt > 300 * SECOND || time_lmt < ZERO)
        time_lmt = 300 * SECOND;
    if (mem_lmt > ONE_KILOBYTE || mem_lmt < ONE)
        mem_lmt = ONE_KILOBYTE;//ONE_KILOBYTE MB
    if (DEBUG) {
        printf("time: %f mem: %d\n", time_lmt, mem_lmt);
    }


    if (compile(lang, work_dir) != COMPILED) {
        addceinfo(solution_id);
        string _compile_info, tmp;
        fstream ceinformation("ce.txt");
        while (getline(ceinformation, tmp)) {
            _compile_info += tmp + "\n";
        }
        Bundle bundle;
        bundle.setSolutionID(solution_id);
        bundle.setResult(COMPILE_ERROR);
        bundle.setFinished(FINISHED);
        bundle.setUsedTime(ZERO_TIME);
        bundle.setMemoryUse(ZERO_MEMORY);
        bundle.setPassPoint(ZERO_PASSPOINT);
        bundle.setPassRate(ZERO_PASSRATE);
        bundle.setCompileInfo(_compile_info);
        webSocket << bundle.toJSONString();
        update_solution(solution_id, COMPILE_ERROR, ZERO_TIME, ZERO_MEMORY, ZERO_SIM, ZERO_SIM, ZERO_PASSRATE);
        update_user(user_id);
        update_problem(p_id);
        //mysql_close(conn);
        clean_workdir(work_dir);
        write_log(oj_home, "compile error");
        exit(0);
    } else {
        update_solution(solution_id, RUNNING_JUDGING, ZERO_TIME, ZERO_MEMORY, ZERO_SIM, ZERO_SIM, ZERO_PASSRATE);
        umount(work_dir);
    }
    //exit(0);
    // run
    char fullpath[BUFFER_SIZE];
    char infile[BUFFER_SIZE];
    char outfile[BUFFER_SIZE];
    char userfile[BUFFER_SIZE];
    sprintf(fullpath, "%s/data/%d", oj_home, p_id); // the fullpath of data dir

    // open DIRs
    DIR *dp;
    dirent *dirp;
    if (p_id > CHILD_PROCESS && (dp = opendir(fullpath)) == nullptr) {
        write_log(oj_home, "No such dir:%s!\n", fullpath);
        //mysql_close(conn);
        exit(-1);
    }

    int ACflg, PEflg;
    ACflg = PEflg = ACCEPT;
    int topmemory = ZERO_MEMORY;
    double usedtime = ZERO_TIME;

    //create chroot for ruby bash python
    switch (lang) {
        case RUBY:
            copy_ruby_runtime(work_dir);
            break;
        case BASH:
            copy_bash_runtime(work_dir);
            break;
        case PYTHON2:
        case PYTHON3:
            copy_python_runtime(work_dir);
            break;
        case PyPy:
            copy_pypy_runtime(work_dir);
            break;
        case PyPy3:
            copy_pypy3_runtime(work_dir);
            break;
        case PHP:
            copy_php_runtime(work_dir);
            break;
        case PERL:
            copy_perl_runtime(work_dir);
            break;
        case CSHARP:
            copy_mono_runtime(work_dir);
            break;
        case OBJC:
            copy_objc_runtime(work_dir);
            break;
        case FREEBASIC:
            copy_freebasic_runtime(work_dir);
            break;
        case SCHEMA:
            copy_guile_runtime(work_dir);
            break;
        case LUA:
            copy_lua_runtime(work_dir);
            break;
        case JAVASCRIPT:
            copy_js_runtime(work_dir);
        default:
            break;
    }
    // read files and run
    double pass_rate = ZERO_PASSRATE;
    int num_of_test = 0;
    int finalACflg = ACflg;
    if (p_id <= TEST_RUN_PROBLEM) {  //custom input running
        printf("running a custom input...\n");
        get_custominput(solution_id, work_dir);
        init_syscalls_limits(lang);
        Bundle bundle;
        bundle.setSolutionID(solution_id);
        bundle.setResult(RUNNING_JUDGING);
        bundle.setFinished(NOT_FINISHED);
        bundle.setUsedTime(ZERO_TIME);
        bundle.setMemoryUse(ZERO_MEMORY);
        bundle.setPassPoint(ZERO_PASSPOINT);
        bundle.setPassRate(ZERO_PASSRATE);
        webSocket << bundle.toJSONString();
        //        webSocket << ws_send(solution_id, RUNNING_JUDGING, NOT_FINISHED, ZERO_TIME, ZERO_MEMORY, ZERO_PASSPOINT,
 //                            ZERO_PASSRATE);
        pid_t pidApp = fork();

        if (pidApp == CHILD_PROCESS) {
            run_solution(lang, work_dir, time_lmt, usedtime, mem_lmt);
        } else {
            watch_solution(pidApp, infile, ACflg, SPECIAL_JUDGE, userfile, outfile,
                           solution_id, lang, topmemory, mem_lmt, usedtime, time_lmt,
                           p_id, PEflg, work_dir);

        }
        fix_python_syntax_error_response(ACflg, lang);
        string error_message;
        if (ACflg == TIME_LIMIT_EXCEEDED) {
            usedtime = time_lmt * 1000;
            error_message = "Time Limit Exceeded.Kill Process.\n";
            //add_reinfo_mysql_by_string(solution_id, error_message);
        } else if (ACflg == RUNTIME_ERROR) {
            if (DEBUG)
                printf("add RE info of %d..... \n", solution_id);
            addreinfo(solution_id);
        } else if (ACflg == MEMORY_LIMIT_EXCEEDED) {
            error_message = "Memory Limit Exceeded.Kill Process.\n";
            //add_reinfo_mysql_by_string(solution_id, error_message);
        }
        string test_run_out;
        if (ACflg == ACCEPT) {
            char reinfo[(1u << 16)];
            FILE *fp = fopen("user.out", "re");
            while (fgets(reinfo, 1u << 16, fp)) {
                string tmp(reinfo);
                test_run_out += tmp;
                if (test_run_out.length() > 4096)
                    break;
            }
            fclose(fp);
        } else {
            test_run_out = error_message;
        }

        if (test_run_out.length() > FOUR * ONE_KILOBYTE) {
            auto omit = to_string(test_run_out.length() - FOUR * ONE_KILOBYTE);
            test_run_out = test_run_out.substr(0, FOUR * ONE_KILOBYTE);
            test_run_out += "\n......Omit " + omit + " characters.";
        }
        if (DEBUG) {
            cout << "test_run_out:" << endl << test_run_out << endl;
        }

        if(usedtime == time_lmt * 1000) {
            test_run_out += "\n测试运行中发生运行超时，程序被强制停止";
        }

        bundle.clear();
        bundle.setSolutionID(solution_id);
        bundle.setResult(TEST_RUN);
        bundle.setFinished(FINISHED);
        bundle.setUsedTime(usedtime);
        bundle.setMemoryUse(topmemory / ONE_KILOBYTE);
        bundle.setPassPoint(ZERO_PASSPOINT);
        bundle.setPassRate(ZERO_PASSRATE);
        bundle.setTestRunResult(test_run_out);
        webSocket << bundle.toJSONString();
//        webSocket << ws_send(solution_id, TEST_RUN, FINISHED, usedtime, topmemory / ONE_KILOBYTE, ZERO_PASSPOINT,
  //                           ZERO_PASSRATE,
   //                          test_run_out);

        auto fpid = fork();
        if (fpid == CHILD_PROCESS) {
            if (!conn.start()) {
                if (DEBUG) {
                    cout << "Init mysql connection ERROR in custom input database insert" << endl;
                }
            } else {
                add_reinfo_mysql_by_string(solution_id, test_run_out);
                //mysql_close(conn);
            }
            exit(0);
        }

        update_solution(solution_id, TEST_RUN, usedtime, topmemory / ONE_KILOBYTE, ZERO_SIM, ZERO_SIM, ZERO_PASSRATE);
        clean_workdir(work_dir);
        //mysql_close(conn);
        exit(0);
    }
    int total_point = 0;
    int pass_point = ZERO_PASSPOINT;
    vector<pair<string, int> >inFileList;
    while((dirp = readdir(dp)) != nullptr) {
        auto fileLen = isInFile(dirp->d_name);
        if(isInFile(dirp->d_name)) {
            inFileList.emplace_back(dirp->d_name, fileLen);
        }
    }
    total_point = inFileList.size();
    //dp = opendir(fullpath);
    Bundle bundle;
    bundle.setSolutionID(solution_id);
    bundle.setResult(RUNNING_JUDGING);
    bundle.setFinished(NOT_FINISHED);
    bundle.setUsedTime(ZERO_TIME);
    bundle.setMemoryUse(ZERO_MEMORY);
    bundle.setPassPoint(ZERO_PASSPOINT);
    bundle.setPassRate(ZERO_PASSRATE);
    bundle.setTotalPoint(total_point);
    webSocket << bundle.toJSONString();
    //webSocket << ws_send(solution_id, RUNNING_JUDGING, NOT_FINISHED, ZERO_TIME, ZERO_MEMORY, ZERO_PASSPOINT,
    //                     ZERO_PASSRATE);


    for (auto& infilePair: inFileList) {
        if(!(ALL_TEST_MODE || ACflg == ACCEPT || ACflg == PRESENTATION_ERROR) && ACflg != TIME_LIMIT_EXCEEDED) {
            break;
        }
        if(ACflg == RUNTIME_ERROR) {
            break;
        }

        if (ACflg <= PRESENTATION_ERROR) {
            ++num_of_test;
            prepare_files(infilePair.first.c_str(), infilePair.second, infile, p_id, work_dir, outfile,
                          userfile, runner_id);
            init_syscalls_limits(lang);

            pid_t pidApp = fork();

            if (pidApp == CHILD_PROCESS) {
                if (DEBUG) {
                    printf("Running solution\n");
                    cout << "Time limit ALL_TEST_MODE:" << (time_lmt + 1) << endl;
                    cout << "Time limit NORMAL:" << ((time_lmt - usedtime / 1000) + 1) << endl;
                }
                run_solution(lang, work_dir, time_lmt, usedtime, mem_lmt);
            } else {

                if (DEBUG) {
                    cout << "Run test point:" << num_of_test << endl;
                }
                watch_solution(pidApp, infile, ACflg, SPECIAL_JUDGE, userfile, outfile,
                               solution_id, lang, topmemory, mem_lmt, usedtime, time_lmt,
                               p_id, PEflg, work_dir);
                judge_solution(ACflg, usedtime, time_lmt, SPECIAL_JUDGE, p_id, infile,
                               outfile, userfile, usercode, PEflg, lang, work_dir, topmemory,
                               mem_lmt, solution_id, num_of_test, global_work_dir);
                if (use_max_time) {
                    max_case_time = max(usedtime, max_case_time);
                    usedtime = ZERO_TIME;
                }
                //clean_session(pidApp);
            }

            if (usedtime > time_lmt * 1000 || ACflg == TIME_LIMIT_EXCEEDED) {
                cout << "Time Limit Exceeded" << endl;
                ACflg = TIME_LIMIT_EXCEEDED;
                usedtime = time_lmt * 1000;
            }

            if (ACflg == ACCEPT) {
                ++pass_point;
            }

            if (ALL_TEST_MODE) {
                if (ACflg == ACCEPT) {
                    ++pass_rate;
                }
                if (finalACflg < ACflg) {
                    finalACflg = ACflg;
                }

                ACflg = ACCEPT;
            }
        }
        bundle.setUsedTime(min(usedtime, time_lmt * 1000));
        bundle.setMemoryUse(min(topmemory / ONE_KILOBYTE, mem_lmt * STD_MB / ONE_KILOBYTE));
        bundle.setPassPoint(pass_point);
        bundle.setPassRate(pass_rate / num_of_test);
        webSocket << bundle.toJSONString();
    }
    if (ACflg == ACCEPT && PEflg == PRESENTATION_ERROR)
        ACflg = PRESENTATION_ERROR;
    if (sim_enable && ACflg == ACCEPT && (!ALL_TEST_MODE || finalACflg == ACCEPT)
        && (lang < BASH || lang == CLANG || lang == CLANGPP || lang >= CPP11)) { //bash don't supported
        sim = get_sim(solution_id, lang, p_id, sim_s_id);
    } else {
        sim = ZERO_SIM;
    }
    //if(ACflg == OJ_RE)addreinfo(solution_id);

    if ((ALL_TEST_MODE && finalACflg == RUNTIME_ERROR) || ACflg == RUNTIME_ERROR) {
        if (DEBUG)
            printf("add RE info of %d..... \n", solution_id);
        auto pid = fork();
        if (pid == CHILD_PROCESS) {
            conn.start();
            addreinfo(solution_id);
            //mysql_close(conn);
            exit(0);
        }
    }
    if (use_max_time) {
        usedtime = max_case_time;
    }


    if (ACflg == TIME_LIMIT_EXCEEDED || (ALL_TEST_MODE && finalACflg == TIME_LIMIT_EXCEEDED)) {
        usedtime = time_lmt * 1000;
    }
    bundle.setResult(ALL_TEST_MODE ? finalACflg : ACflg);
    bundle.setFinished(FINISHED);
    bundle.setUsedTime(usedtime);
    bundle.setMemoryUse(topmemory / ONE_KILOBYTE);
    bundle.setPassPoint(pass_point);
    bundle.setPassRate(pass_rate / num_of_test);
    bundle.setSim(sim);
    bundle.setSimSource(sim_s_id);
    //webSocket << ws_send(solution_id, ALL_TEST_MODE ? finalACflg : ACflg, FINISHED, usedtime,
     //                    topmemory / ONE_KILOBYTE,
     //                    pass_point,
     //                    pass_rate / num_of_test, "", "", sim, sim_s_id);
     webSocket << bundle.toJSONString();
    string sql = "UPDATE solution set pass_point=" + to_string(pass_point) + " WHERE solution_id=" +
                 to_string(solution_id);
    conn.query(conn, sql.c_str(), sql.length());

    if (ALL_TEST_MODE) {
        if (num_of_test > 0) {
            pass_rate /= num_of_test;
        }
        if (DEBUG) {
            cout << "Write Usedtime: " << usedtime << endl;
        }
        update_solution(solution_id, finalACflg, usedtime, topmemory / ONE_KILOBYTE, sim,
                        sim_s_id, pass_rate);
    } else {
        update_solution(solution_id, ACflg, usedtime, topmemory / ONE_KILOBYTE, sim,
                        sim_s_id, ZERO_PASSRATE);
    }
    if ((ALL_TEST_MODE && (SPECIAL_JUDGE || finalACflg == WRONG_ANSWER || finalACflg == PRESENTATION_ERROR)) ||
        (ACflg == WRONG_ANSWER || ACflg == PRESENTATION_ERROR)) {
        if (DEBUG)
            printf("add diff info of %d..... \n", solution_id);
        // if (!SPECIAL_JUDGE)
        adddiffinfo(solution_id);
    }
    update_user(user_id);
    update_problem(p_id);
    clean_workdir(work_dir);

    if (DEBUG)
        write_log(oj_home, "result=%d", ALL_TEST_MODE ? finalACflg : ACflg);
    //mysql_close(conn);
    if (record_call) {
        print_call_array();
    }
    closedir(dp);
    return 0;
}
