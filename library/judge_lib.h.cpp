//
// Created by Ryan Lee on 2018/7/17.
//

#include "judge_lib.h"

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
                      const int &memory, const int &pass_point, const double &pass_rate, const string &test_run_result,
                      const string &compile_info, const int sim, const int sim_s_id) {
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
        } else {
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

bool isJava(const int lang) {
    switch (lang) {
        case JAVA:
        case JAVA6:
        case JAVA7:
        case JAVA8:
            return true;
        default:
            return false;
    }
}

bool isC(const int lang) {
    switch (lang) {
        case C11:
        case C99:
        case CLANG:
        case CLANG11:
            return true;
        default:
            return false;
    }
}

bool isCPP(const int lang) {
    switch (lang) {
        case CPP11:
        case CPP17:
        case CPP98:
        case CLANGPP:
        case CLANGPP17:
            return true;
        default:
            return false;
    }
}

bool isCOrCPP(const int lang) {
    return isC(lang) || isCPP(lang);
}


bool is_number(const string &s) {
    for (auto c:s) {
        if (!isdigit(c))return false;
    }
    return true;
}

bool check_valid_presentation_error(const char *ansfile, const char *userfile) {
    fstream user(userfile), ans(ansfile);
    string u, a;
    while (user >> u) {
        ans >> a;
        if (is_number(a)) {
            if (a != u) {
                return false;
            }
        } else {
            break;
        }
    }
    return true;
}


void make_diff_out_full(FILE *f1, FILE *f2, int c1, int c2, const char *path) {

    execute_cmd("echo '========[%s]========='>>diff.out", getFileNameFromPath(path));
    execute_cmd("echo '------测试输入前100行------'>>diff.out");
    execute_cmd("head -100 data.in>>diff.out");
    execute_cmd("echo '\n------测试输出前100行-----'>>diff.out");
    execute_cmd("head -100 '%s'>>diff.out", path);
    execute_cmd("echo '\n------用户输出前100行-----'>>diff.out");
    execute_cmd("head -100 user.out>>diff.out");
    execute_cmd("echo '\n------测试输出(左)与用户输出(右)前200行的区别-----'>>diff.out");
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


void copy_shell_runtime(char *work_dir) {
    cout << "copy shell runtime start" << endl;
    execute_cmd("/bin/mkdir %s/lib", work_dir);
    execute_cmd("/bin/mkdir %s/lib64", work_dir);
    execute_cmd("/bin/mkdir %s/bin", work_dir);
    //  execute_cmd("/bin/cp /lib/* %s/lib/", work_dir);
    //  execute_cmd("/bin/cp -a /lib/i386-linux-gnu %s/lib/", work_dir);
    //  execute_cmd("/bin/cp -a /usr/lib/i386-linux-gnu %s/lib/", work_dir);
    execute_cmd("/bin/cp -a /lib/x86_64-linux-gnu %s/lib/", work_dir);
    execute_cmd("/bin/cp /lib64/* %s/lib64/", work_dir);
    //  execute_cmd("/bin/cp /lib32 %s/", work_dir);
    //execute_cmd("/bin/cp /bin/busybox %s/bin/", work_dir);
    //execute_cmd("/bin/ln -s /bin/busybox %s/bin/sh", work_dir);
    //execute_cmd("/bin/cp /bin/bash %s/bin/bash", work_dir);
    //execute_cmd("/bin/cp /bin/ldd %s/bin", work_dir);
    //execute_cmd("/bin/cp /bin/ls %s/bin", work_dir);
    cout << "copy shell runtime finish" << endl;
}

void copy_objc_runtime(char *work_dir) {
    copy_shell_runtime(work_dir);
    execute_cmd("/bin/mkdir -p %s/proc", work_dir);
    execute_cmd("/bin/mount -o bind /proc %s/proc", work_dir);
    execute_cmd("/bin/mkdir -p %s/lib/", work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libdbus-1.so.3                          %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libgcc_s.so.1                           %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libgcrypt.so.11                         %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libgpg-error.so.0                       %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libz.so.1                               %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libc.so.6                 %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libdl.so.2                %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libm.so.6                 %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libnsl.so.1               %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libpthread.so.0           %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/librt.so.1                %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libavahi-client.so.3                %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libavahi-common.so.3                %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libdns_sd.so.1                      %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libffi.so.5                         %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libgnustep-base.so.1.19             %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libgnutls.so.26                     %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libobjc.so.2                        %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libtasn1.so.3                       %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libxml2.so.2                        %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libxslt.so.1                        %s/lib/ ",
            work_dir);

}

void copy_bash_runtime(char *work_dir) {
    //char cmd[BUFFER_SIZE];
    //const char * ruby_run="/usr/bin/ruby";
    copy_shell_runtime(work_dir);
    execute_cmd("/bin/cp `which bc`  %s/bin/", work_dir);
    execute_cmd("busybox dos2unix Main.sh", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/grep", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/awk", work_dir);
    execute_cmd("/bin/cp /bin/sed %s/bin/sed", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/cut", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/sort", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/join", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/wc", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/tr", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/dc", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/dd", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/cat", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/tail", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/head", work_dir);
    execute_cmd("/bin/ln -s /bin/busybox %s/bin/xargs", work_dir);
    execute_cmd("chmod +rx %s/Main.sh", work_dir);

}

void copy_ruby_runtime(char *work_dir) {

    copy_shell_runtime(work_dir);
    execute_cmd("mkdir -p %s/usr", work_dir);
    execute_cmd("mkdir -p %s/usr/lib", work_dir);
    execute_cmd("mkdir -p %s/usr/lib64", work_dir);
    execute_cmd("cp -a /usr/lib/libruby* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib/ruby* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib64/ruby* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libruby* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/bin/ruby* %s/", work_dir);

}

void copy_guile_runtime(char *work_dir) {

    copy_shell_runtime(work_dir);
    execute_cmd("/bin/mkdir -p %s/proc", work_dir);
    execute_cmd("/bin/mount -o bind /proc %s/proc", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/lib", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/share", work_dir);
    execute_cmd("/bin/cp -a /usr/share/guile %s/usr/share/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libguile* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libgc* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/i386-linux-gnu/libffi* %s/usr/lib/",
                work_dir);
    execute_cmd("/bin/cp /usr/lib/i386-linux-gnu/libunistring* %s/usr/lib/",
                work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libgmp* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libgmp* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libltdl* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libltdl* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/bin/guile* %s/", work_dir);

}

void copy_python_runtime(char *work_dir) {

    copy_shell_runtime(work_dir);
    cout << "copy python runtime start" << endl;
    execute_cmd("mkdir -p %s/usr/include", work_dir);
    execute_cmd("mkdir -p %s/dev", work_dir);
    execute_cmd("mkdir -p %s/usr/lib", work_dir);
    execute_cmd("mkdir -p %s/usr/lib64", work_dir);
    execute_cmd("mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("cp /usr/bin/python* %s/", work_dir);
    execute_cmd("cp /usr/local/bin/python* %s/", work_dir);
    execute_cmd("cp -a /usr/lib/python* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/local/lib/python* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib64/python* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/local/lib64/python* %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/local/lib/python* %s/usr/local/lib/", work_dir);
    execute_cmd("cp -a /usr/include/python* %s/usr/include/", work_dir);
    execute_cmd("cp -a /usr/local/include/python* %s/usr/include/", work_dir);
    execute_cmd("cp -a /usr/lib/libpython* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/local/lib/libpython* %s/usr/lib/", work_dir);
    execute_cmd("/bin/mkdir -p %s/home/judge", work_dir);
    execute_cmd("/bin/chown judge %s", work_dir);
    execute_cmd("/bin/mkdir -p %s/etc", work_dir);
    execute_cmd("/bin/grep judge /etc/passwd>%s/etc/passwd", work_dir);
    execute_cmd("/bin/mount -o bind /dev %s/dev", work_dir);
    cout << "copy python runtime finish" << endl;
}


void copy_pypy_runtime(char *work_dir) {
    copy_shell_runtime(work_dir);
    execute_cmd("mkdir -p %s/usr/include", work_dir);
    execute_cmd("mkdir -p %s/dev", work_dir);
    execute_cmd("mkdir -p %s/usr/lib", work_dir);
    execute_cmd("mkdir -p %s/usr/lib64", work_dir);
    execute_cmd("mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("cp -a /usr/local/pypy %s/", work_dir);
    // execute_cmd("cp -a /usr/include/pypy* %s/usr/include/", work_dir);
    // execute_cmd("cp -a /usr/lib/libpython* %s/usr/lib/", work_dir);
    execute_cmd("cp -a /usr/lib64/libpthread.so.0 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libc.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libutil.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libdl.so.2 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libbz2.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libz.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libm.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/librt.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libcrypt.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libgcc_s.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libfreebl3.so %s/usr/lib64/", work_dir);
    execute_cmd("/bin/mkdir -p %s/home/judge", work_dir);
    execute_cmd("/bin/chown judge %s", work_dir);
    execute_cmd("/bin/mkdir -p %s/etc", work_dir);
    execute_cmd("/bin/grep judge /etc/passwd>%s/etc/passwd", work_dir);
    execute_cmd("/bin/mount -o bind /dev %s/dev", work_dir);
}


void copy_pypy3_runtime(char *work_dir) {
    copy_shell_runtime(work_dir);
    execute_cmd("mkdir -p %s/usr/include", work_dir);
    execute_cmd("mkdir -p %s/dev", work_dir);
    execute_cmd("mkdir -p %s/usr/lib", work_dir);
    execute_cmd("mkdir -p %s/usr/lib64", work_dir);
    execute_cmd("mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("mkdir -p %s/proc", work_dir);
    execute_cmd("cp -a /proc/cpuinfo %s/proc/", work_dir);
    execute_cmd("cp -a /usr/local/pypy3 %s/pypy3", work_dir);
    execute_cmd("cp -a /usr/local/pypy3/bin/libpypy3-c.so %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/local/pypy3/lib %s/usr/lib64", work_dir);
    execute_cmd("cp -a /usr/local/pypy3/lib %s/usr/lib", work_dir);
    execute_cmd("cp -a /usr/lib64/linux-vdso.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libpthread.so.0 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libc.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libutil.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libdl.so.2 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libbz2.so.1.0 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libbz2.so.1.0.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libbz2.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libexpat.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libm.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libz.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/librt.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libcrypt.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libffi.so.6 %s/usr/lib64/", work_dir);
    //execute_cmd("cp -a /usr/lib64/libncursesw.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libtinfow.so.6 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libgcc_s.so.1 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/ld-linux-x86-64.so.2 %s/usr/lib64/", work_dir);
    execute_cmd("cp -a /usr/lib64/libfreebl3.so %s/usr/lib64/", work_dir);

    execute_cmd("/bin/mkdir -p %s/home/judge", work_dir);
    execute_cmd("/bin/chown -R judge %s", work_dir);
    execute_cmd("/bin/mkdir -p %s/etc", work_dir);
    execute_cmd("/bin/grep judge /etc/passwd>%s/etc/passwd", work_dir);
    execute_cmd("/bin/mount -o bind /dev %s/dev", work_dir);
}

void copy_php_runtime(char *work_dir) {

    copy_shell_runtime(work_dir);
    execute_cmd("/bin/mkdir %s/usr", work_dir);
    execute_cmd("/bin/mkdir %s/usr/lib", work_dir);
    execute_cmd("/bin/cp /usr/lib/libedit* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libdb* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libgssapi_krb5* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libkrb5* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libk5crypto* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libedit* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libdb* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libgssapi_krb5* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libkrb5* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/*/libk5crypto* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/libxml2* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/lib/x86_64-linux-gnu/libxml2.so* %s/usr/lib/",
                work_dir);
    execute_cmd("/bin/cp /usr/bin/php* %s/", work_dir);
    execute_cmd("chmod +rx %s/Main.php", work_dir);

}

void copy_perl_runtime(char *work_dir) {

    copy_shell_runtime(work_dir);
    execute_cmd("/bin/mkdir %s/usr", work_dir);
    execute_cmd("/bin/mkdir %s/usr/lib", work_dir);
    execute_cmd("/bin/cp /usr/lib/libperl* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/bin/perl* %s/", work_dir);

}

void copy_freebasic_runtime(char *work_dir) {

    copy_shell_runtime(work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/bin", work_dir);
    execute_cmd("/bin/cp /usr/local/lib/freebasic %s/usr/local/lib/", work_dir);
    execute_cmd("/bin/cp /usr/local/bin/fbc %s/", work_dir);
    execute_cmd("/bin/cp -a /lib32/* %s/lib/", work_dir);

}

void copy_mono_runtime(char *work_dir) {

    copy_shell_runtime(work_dir);
    execute_cmd("/bin/mkdir %s/usr", work_dir);
    execute_cmd("/bin/mkdir %s/proc", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/lib/mono/2.0", work_dir);
    execute_cmd("/bin/cp -a /usr/lib/mono %s/usr/lib/", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/lib64/mono/2.0", work_dir);
    execute_cmd("/bin/cp -a /usr/lib64/mono %s/usr/lib64/", work_dir);

    execute_cmd("/bin/cp /usr/lib/libgthread* %s/usr/lib/", work_dir);

    execute_cmd("/bin/mount -o bind /proc %s/proc", work_dir);
    execute_cmd("/bin/cp /usr/bin/mono* %s/", work_dir);

    execute_cmd("/bin/cp /usr/lib/libgthread* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /lib/libglib* %s/lib/", work_dir);
    execute_cmd("/bin/cp /lib/tls/i686/cmov/lib* %s/lib/tls/i686/cmov/",
                work_dir);
    execute_cmd("/bin/cp /lib/libpcre* %s/lib/", work_dir);
    execute_cmd("/bin/cp /lib/ld-linux* %s/lib/", work_dir);
    execute_cmd("/bin/cp /lib64/ld-linux* %s/lib64/", work_dir);
    execute_cmd("/bin/mkdir -p %s/home/judge", work_dir);
    execute_cmd("/bin/chown judge %s/home/judge", work_dir);
    execute_cmd("/bin/mkdir -p %s/etc", work_dir);
    execute_cmd("/bin/grep judge /etc/passwd>%s/etc/passwd", work_dir);

}

void copy_lua_runtime(char *work_dir) {

    copy_shell_runtime(work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/lib", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/local/bin", work_dir);
    execute_cmd("/bin/cp /usr/bin/lua %s/", work_dir);

}

void copy_js_runtime(char *work_dir) {
    execute_cmd("mkdir -p %s/dev", work_dir);
    execute_cmd("/bin/mount -o bind /dev %s/dev", work_dir);
    execute_cmd("/bin/mkdir -p %s/usr/lib64 %s/lib64/ %s/lib64/", work_dir, work_dir, work_dir);
    execute_cmd("/bin/cp /lib64/libz.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64libuv.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicui18n.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicuuc.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicudata.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libtinfo.so.*  %s/lib64/", work_dir);

    execute_cmd("/bin/cp /usr/lib64/libcares.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libv8.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib/libssl.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libcrypto.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libdl.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/librt.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libstdc++.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libpthread.so.*  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libc.so.6  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libm.so.6  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libgcc_s.so.1  %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/ld-linux.so.*  %s/lib64/", work_dir);

    execute_cmd("/bin/mkdir -p %s/usr/lib %s/lib64/", work_dir, work_dir);

    execute_cmd("/bin/cp /lib64/libz.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libuv.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/librt.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libpthread.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libdl.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libssl.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libcrypto.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicui18n.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicuuc.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libstdc++.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libm.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libgcc_s.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/libc.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /lib64/ld-linux-x86-64.so.* %s/lib64/", work_dir);
    execute_cmd("/bin/cp /usr/lib64/libicudata.so.* %s/lib64/", work_dir);


    execute_cmd("/bin/cp /usr/local/bin/node %s/", work_dir);
}
int isInFile(const char fname[]) {
    auto l = static_cast<int>(strlen(fname));
    if (l <= 3 || strcmp(fname + l - 3, ".in") != 0)
        return 0;
    else
        return l - 3;
}

void move_to_next_nonspace_character(int &c, FILE *&f, int &ret) {
    while (isspace(c) || iscntrl(c)) {
        do {
            c = fgetc(f);
        } while (isspace(c) || iscntrl(c));
    }
}

bool is_not_character(int c) {
    return (iscntrl(c) || isspace(c));
}


void umount(char *work_dir) {
    execute_cmd("/bin/umount -f %s/proc", work_dir);
    execute_cmd("/bin/umount -f %s/dev ", work_dir);
    execute_cmd("/bin/umount -f %s/lib ", work_dir);
    execute_cmd("/bin/umount -f %s/lib64 ", work_dir);
    execute_cmd("/bin/umount -f %s/etc/alternatives ", work_dir);
    execute_cmd("/bin/umount -f %s/usr ", work_dir);
    execute_cmd("/bin/umount -f %s/bin ", work_dir);
    execute_cmd("/bin/umount -f %s/proc ", work_dir);
    execute_cmd("/bin/umount -f bin usr lib lib64 etc/alternatives proc dev ");
    execute_cmd("/bin/umount -f %s/* ", work_dir);
    execute_cmd("/bin/umount -f %s/log/* ", work_dir);
    execute_cmd("/bin/umount -f %s/log/etc/alternatives ", work_dir);
}


char *escape(char s[], const char t[]) {
    int i, j;
    for (i = j = 0; t[i] != '\0'; ++i) {
        if (t[i] == '\'') {
            s[j++] = '\'';
            s[j++] = '\\';
            s[j++] = '\'';
            s[j++] = '\'';
            continue;
        } else {
            s[j++] = t[i];
        }
    }
    s[j] = '\0';
    return s;
}

int fix_python_mis_judge(char *work_dir, int &ACflg, int &topmemory,
                                int mem_lmt) {
    int comp_res = execute_cmd(
            "/bin/grep 'MemoryError'  %s/error.out", work_dir);

    if (!comp_res) {
        printf("Python need more Memory!");
        ACflg = MEMORY_LIMIT_EXCEEDED;
        topmemory = mem_lmt * STD_MB;
    }

    return comp_res;
}

int fix_java_mis_judge(char *work_dir, int &ACflg, int &topmemory,
                              int mem_lmt) {
    int comp_res;
    execute_cmd("chmod 700 %s/error.out", work_dir);
    if (DEBUG)
        execute_cmd("cat %s/error.out", work_dir);
    comp_res = execute_cmd("/bin/grep 'Exception'  %s/error.out", work_dir);
    if (!comp_res) {
        printf("Exception reported\n");
        ACflg = RUNTIME_ERROR;
    }
    execute_cmd("cat %s/error.out", work_dir);

    comp_res = execute_cmd(
            "/bin/grep 'java.lang.OutOfMemoryError'  %s/error.out", work_dir);

    if (!comp_res) {
        printf("JVM need more Memory!");
        ACflg = MEMORY_LIMIT_EXCEEDED;
        topmemory = mem_lmt * STD_MB;
    }

    if (!comp_res) {
        printf("JVM need more Memory or Threads!");
        ACflg = MEMORY_LIMIT_EXCEEDED;
        topmemory = mem_lmt * STD_MB;
    }
    comp_res = execute_cmd("/bin/grep 'Could not create'  %s/error.out",
                           work_dir);
    if (!comp_res) {
        printf("jvm need more resource,tweak -Xmx(OJ_JAVA_BONUS) Settings");
        ACflg = RUNTIME_ERROR;
    }
    return comp_res;
}


void clean_workdir(char *work_dir) {
    umount(work_dir);
    if (DEBUG) {
        execute_cmd("/bin/rm -rf %s/log/*", work_dir);
        execute_cmd("mkdir %s/log/", work_dir);
        execute_cmd("/bin/mv %s/* %s/log/", work_dir, work_dir);
    } else {
        execute_cmd("mkdir %s/log/", work_dir);
        execute_cmd("/bin/mv %s/* %s/log/", work_dir, work_dir);
        execute_cmd("/bin/rm -rf %s/log/*", work_dir);
    }

}

int detectArgType(const char* argument) {
    if (argument[0] == '-') {
        if (!strcmp(argument + 1,"language")) {
            return _LANG_NAME;
        }
        else if(!strcmp(argument + 1,"no_record")) {
            return _NO_RECORD;
        }
        else if(!strcmp(argument + 1, "dir")) {
            return _DIR;
        }
        else if(!strcmp(argument + 1,"record")) {
            return _RECORD_CALL;
        }
        else if(!strcmp(argument + 1,"solution_id")) {
            return _SOLUTION_ID;
        }
        else if(!strcmp(argument + 1,"runner_id")) {
            return _RUNNER_ID;
        }
        else if(!strcmp(argument + 1,"admin")) {
            return _ADMIN;
        }
        else if(!strcmp(argument + 1, "no-sim")) {
            return _NO_SIM;
        }
    }
    else {
        if (!strcmp(argument,"DEBUG")) {
            return _DEBUG;
        }
        else {
            return _ERROR;
        }
    }
}

void write_log(const char* oj_home, const char *_fmt, ...) {
    va_list ap;
    char fmt[FOUR * ONE_KILOBYTE];
    strncpy(fmt, _fmt, FOUR * ONE_KILOBYTE);
    char buffer[FOUR * ONE_KILOBYTE];
    //      time_t          t = time(NULL);
    //int l;
    sprintf(buffer, "%s/log/client.log", oj_home);
    FILE *fp = fopen(buffer, "ae+");
    if (fp == nullptr) {
        fprintf(stderr, "openfile error!\n");
        system("pwd");
    }
    va_start(ap, _fmt);
    //l =
    vsprintf(buffer, fmt, ap);
    fprintf(fp, "%s\n", buffer);
    //if (DEBUG) {
    cerr << buffer << endl;
    //printf("%s\n", buffer);
    //}
    va_end(ap);
    fclose(fp);
}
