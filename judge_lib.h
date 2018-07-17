//
// Created by Ryan Lee on 2018/7/17.
//

#ifndef JUDGE_CLIENT_JUDGE_LIB_H
#define JUDGE_CLIENT_JUDGE_LIB_H
#include <sys/stat.h>
#include <cctype>
#include <iostream>
#include "static_var.h"
using namespace std;
using json = nlohmann::json;
// urlencoded function copied from http://www.geekhideout.com/urlcode.shtml
/* Converts a hex character to its integer value */
char from_hex(char ch) {
    return static_cast<char>(isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10);
}

/* Converts an integer value to its hex character*/
char to_hex(char code) {
    static char hex[] = "0123456789abcdef";
    return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(char *str) {
    char *pstr = str, *buf = (char *) malloc(strlen(str) * 3 + 1), *pbuf = buf;
    while (*pstr) {
        if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.'
            || *pstr == '~')
            *pbuf++ = *pstr;
        else if (*pstr == ' ')
            *pbuf++ = '+';
        else
            *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(
                    static_cast<char>(*pstr & 15));
        pstr++;
    }
    *pbuf = '\0';
    return buf;
}


long get_file_size(const char *filename) {
    struct stat f_stat{};

    if (stat(filename, &f_stat) == -1) {
        return 0;
    }

    return (long) f_stat.st_size;
}

bool utf8_check_is_valid(const string &string) {
    int c, i, ix, n, j;
    for (i = 0, ix = static_cast<int>(string.length()); i < ix; i++) {
        c = (unsigned char) string[i];
        //if (c==0x09 || c==0x0a || c==0x0d || (0x20 <= c && c <= 0x7e) ) n = 0; // is_printable_ascii
        if (0x00 <= c && c <= 0x7f) n = 0; // 0bbbbbbb
        else if ((c & 0xE0) == 0xC0) n = 1; // 110bbbbb
        else if (c == 0xed && i < (ix - 1) && ((unsigned char) string[i + 1] & 0xa0) == 0xa0)
            return false; //U+d800 to U+dfff
        else if ((c & 0xF0) == 0xE0) n = 2; // 1110bbbb
        else if ((c & 0xF8) == 0xF0) n = 3; // 11110bbb
            //else if (($c & 0xFC) == 0xF8) n=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
            //else if (($c & 0xFE) == 0xFC) n=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
        else return false;
        for (j = 0; j < n && i < ix; j++) { // n bytes matching 10bbbbbb follow ?
            if ((++i == ix) || (((unsigned char) string[i] & 0xC0) != 0x80))
                return false;
        }
    }
    return true;
}


string ws_send(const int &solution_id, const int &state, const int &finished, const double &time,
               const int &memory, const int &pass_point, const double &pass_rate, const string &test_run_result = "",
               const string &compile_info = "", const int sim = 0, const int sim_s_id = 0) {
    json send_msg;
    send_msg["type"] = "judger";
    send_msg["value"]["solution_id"] = solution_id;
    send_msg["value"]["state"] = state;
    send_msg["value"]["finish"] = finished;
    send_msg["value"]["time"] = time;
    send_msg["value"]["memory"] = memory;
    send_msg["value"]["pass_rate"] = pass_rate;
    send_msg["value"]["pass_point"] = pass_point;
    send_msg["value"]["sim"] = sim;
    send_msg["value"]["sim_s_id"] = sim_s_id;
    if (test_run_result.length()) {
        if (utf8_check_is_valid(test_run_result)) {
            send_msg["value"]["test_run_result"] = test_run_result;
        } else {
            send_msg["value"]["test_run_result"] = string("检测到非法UTF-8输出");
        }
    }
    if (compile_info.length()) {
        if (utf8_check_is_valid(compile_info)) {
            send_msg["value"]["compile_info"] = compile_info;
        }
        else {
            send_msg["value"]["compile_info"] = string("检测到非法UTF-8输出");
        }
    }
    return send_msg.dump();
    /*
    string s="{\"solution_id\":"+to_string(solution_id)+",\"state\":"+to_string(state)+",\"finish\":"+to_string(finished);
    s+=",\"time\":"+to_string(time)+",\"memory\":"+to_string(memory)+",\"pass_point\":"+to_string(pass_point)+"";
    s+="}";
    return s;
     */
}

int execute_cmd(const char *fmt, ...) {
    char cmd[BUFFER_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(cmd, fmt, ap);
    int ret = system(cmd);
    va_end(ap);
    return ret;
}

int after_equal(const char *c) {
    int i = 0;
    for (; c[i] && c[i] != '='; ++i);
    return ++i;
}


void trim(char *c) {
    char buf[BUFFER_SIZE];
    char *start, *end;
    strcpy(buf, c);
    start = buf;
    while (isspace(*start))
        ++start;
    end = start;
    while (!isspace(*end))
        ++end;
    *end = '\0';
    strcpy(c, start);
}


bool read_buf(char *buf, const char *key, char *value) {
    if (strncmp(buf, key, strlen(key)) == 0) {
        strcpy(value, buf + after_equal(buf));
        trim(value);
        if (DEBUG)
            printf("%s\n", value);
        return true;
    }
    return false;
}

void read_int(char *buf, const char *key, int &value) {
    char buf2[BUFFER_SIZE];
    if (read_buf(buf, key, buf2))
        value = atoi(buf2);
}

const char *getFileNameFromPath(const char *path) {
    for (auto i = static_cast<int>(strlen(path)); i >= 0; i--) {
        if (path[i] == '/')
            return &path[i + 1];
    }
    return path;
}


void make_diff_out_full(FILE *f1, FILE *f2, int c1, int c2, const char *path) {

    execute_cmd("echo '========[%s]========='>>diff.out", getFileNameFromPath(path));
    execute_cmd("echo '------test in top 100 lines------'>>diff.out");
    execute_cmd("head -100 data.in>>diff.out");
    execute_cmd("echo '------test out top 100 lines-----'>>diff.out");
    execute_cmd("head -100 '%s'>>diff.out", path);
    execute_cmd("echo '------user out top 100 lines-----'>>diff.out");
    execute_cmd("head -100 user.out>>diff.out");
    execute_cmd("echo '------diff out 200 lines-----'>>diff.out");
    execute_cmd("diff '%s' user.out -y|head -200>>diff.out", path);
    execute_cmd("echo '=============================='>>diff.out");

}

void make_diff_out_simple(FILE *f1, FILE *f2, int c1, int c2, const char *path) {
    execute_cmd("echo '========[%s]========='>>diff.out", getFileNameFromPath(path));
    execute_cmd("echo '=======diff out 100 lines====='>>diff.out");
    execute_cmd("diff '%s' user.out -y|head -100>>diff.out", path);
    execute_cmd("echo '=============================='>>diff.out");
}


void delnextline(char s[]) {
    auto L = static_cast<int>(strlen(s));
    while (L > 0 && (s[L - 1] == '\n' || s[L - 1] == '\r'))
        s[--L] = 0;
}


FILE *read_cmd_output(const char *fmt, ...) {
    char cmd[BUFFER_SIZE];

    FILE *ret = nullptr;
    va_list ap;

    va_start(ap, fmt);
    vsprintf(cmd, fmt, ap);
    va_end(ap);
    if (DEBUG)
        printf("%s\n", cmd);
    ret = popen(cmd, "r");

    return ret;
}

#endif //JUDGE_CLIENT_JUDGE_LIB_H