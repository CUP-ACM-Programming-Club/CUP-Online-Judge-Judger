//
// Created by Ryan Lee on 2018/7/17.
//

#ifndef JUDGE_CLIENT_JUDGE_LIB_H
#define JUDGE_CLIENT_JUDGE_LIB_H

#include <sys/stat.h>
#include <cctype>
#include <iostream>
#include <fstream>
#include "../header/static_var.h"
#include "../header/json.hpp"
#include <cstdarg>

using namespace std;
using json = nlohmann::json;
// urlencoded function copied from http://www.geekhideout.com/urlcode.shtml
/* Converts a hex character to its integer value */
extern char from_hex(char ch);

/* Converts an integer value to its hex character*/
extern char to_hex(char code);

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
extern char *url_encode(char *str);


extern long get_file_size(const char *filename);

extern bool utf8_check_is_valid(const string &string);


extern string ws_send(const int &solution_id, const int &state, const int &finished, const double &time,
               const int &memory, const int &pass_point, const double &pass_rate, const string &test_run_result = "",
               const string &compile_info = "", const int sim = 0, const int sim_s_id = 0);

extern int execute_cmd(const char *fmt, ...);

extern int after_equal(const char *c);


extern void trim(char *c);


extern bool read_buf(char *buf, const char *key, char *value);

extern void read_int(char *buf, const char *key, int &value);

extern const char *getFileNameFromPath(const char *path);

extern bool isJava(const int lang);

extern bool is_number(const string &s);

extern bool check_valid_presentation_error(const char *ansfile, const char *userfile);


extern void make_diff_out_full(FILE *f1, FILE *f2, int c1, int c2, const char *path);

extern void make_diff_out_simple(FILE *f1, FILE *f2, int c1, int c2, const char *path);


extern void delnextline(char s[]);


extern FILE *read_cmd_output(const char *fmt, ...);


extern void copy_shell_runtime(char *work_dir);

extern void copy_objc_runtime(char *work_dir);

extern void copy_bash_runtime(char *work_dir);

extern void copy_ruby_runtime(char *work_dir);

extern void copy_guile_runtime(char *work_dir);

extern void copy_python_runtime(char *work_dir);


extern void copy_pypy_runtime(char *work_dir);


extern void copy_pypy3_runtime(char *work_dir);

extern void copy_php_runtime(char *work_dir);

extern void copy_perl_runtime(char *work_dir);

extern void copy_freebasic_runtime(char *work_dir);

extern void copy_mono_runtime(char *work_dir);

extern void copy_lua_runtime(char *work_dir);

extern void copy_js_runtime(char *work_dir);
extern int isInFile(const char fname[]);

extern void move_to_next_nonspace_character(int &c, FILE *&f, int &ret);

extern bool is_not_character(int c);


extern void umount(char *work_dir);


extern char *escape(char s[], const char t[]);

extern int fix_python_mis_judge(char *work_dir, int &ACflg, int &topmemory,
                         int mem_lmt);

extern int fix_java_mis_judge(char *work_dir, int &ACflg, int &topmemory,
                       int mem_lmt);


extern void clean_workdir(char *work_dir);

extern int detectArgType(const char* argument);

extern void write_log(const char* oj_home, const char *_fmt, ...);

#endif //JUDGE_CLIENT_JUDGE_LIB_H