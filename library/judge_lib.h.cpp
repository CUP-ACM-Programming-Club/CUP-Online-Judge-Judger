//
// Created by Ryan Lee on 2018/7/17.
//

#include <dirent.h>
#include <dlfcn.h>
#include "judge_lib.h"
#include "../header/static_var.h"
#include "../model/submission/SubmissionInfo.h"

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
        case L_C11:
        case L_C99:
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
        case L_CPP17:
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

int detectArgType(const char *argument) {
    if (argument[0] == '-') {
        if (!strcmp(argument + 1, "language")) {
            return _LANG_NAME;
        } else if (!strcmp(argument + 1, "no_record")) {
            return _NO_RECORD;
        } else if (!strcmp(argument + 1, "dir")) {
            return _DIR;
        } else if (!strcmp(argument + 1, "record")) {
            return _RECORD_CALL;
        } else if (!strcmp(argument + 1, "solution_id")) {
            return _SOLUTION_ID;
        } else if (!strcmp(argument + 1, "runner_id")) {
            return _RUNNER_ID;
        } else if (!strcmp(argument + 1, "admin")) {
            return _ADMIN;
        } else if (!strcmp(argument + 1, "no-sim")) {
            return _NO_SIM;
        } else if (!strcmp(argument + 1, "no-mysql")) {
            return _NO_MYSQL;
        } else if (!strcmp(argument + 1, "judger_id")) {
            return _JUDGER_ID;
        }
        else {
            return _ERROR;
        }
    } else {
        if (!strcmp(argument, "DEBUG")) {
            return _DEBUG;
        } else {
            return _ERROR;
        }
    }
}

void write_log(const char *oj_home, const char *_fmt, ...) {
    va_list ap;
    char fmt[FOUR * ONE_KILOBYTE];
    strncpy(fmt, _fmt, FOUR * ONE_KILOBYTE);
    char buffer[FOUR * ONE_KILOBYTE];
    //      time_t          t = time(NULL);
    //int l;
    //sprintf(buffer, "%s/log/client.log", oj_home);
    //FILE *fp = fopen(buffer, "ae+");
    //if (fp == nullptr) {
    //    fprintf(stderr, "openfile error!\n");
    //    system("pwd");
    //}
    va_start(ap, _fmt);
    //l =
    vsprintf(buffer, fmt, ap);
    //fprintf(fp, "%s\n", buffer);
    //if (DEBUG) {
    cerr << buffer << endl;
    //printf("%s\n", buffer);
    //}
    va_end(ap);
    //fclose(fp);
}

bool initWebSocketConnection(string &ip, int port) {
    webSocket.connect("ws://" + ip + ":" + to_string(port));
    return webSocket.isConnected();
}

bool initWebSocketConnection(string &&ip, int port) {
    webSocket.connect("ws://" + ip + ":" + to_string(port));
    return webSocket.isConnected();
}

void get_solution_info(int solution_id, int &p_id, char *user_id,
                       int &lang) {
    get_solution_info_from_mysql(solution_id, p_id, user_id, lang);
}

void get_solution_info_from_mysql(int solution_id, int& p_id, char *user_id, int& lang) {
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

void getSolutionInfoFromSubmissionInfo(SubmissionInfo& submissionInfo, int& p_id, char* user_id, int& lang) {
    p_id = submissionInfo.getProblemId();
    sprintf(user_id, "%s", submissionInfo.getUserId().c_str());
    lang = submissionInfo.getLanguage();
}

void buildSubmissionInfo(SubmissionInfo& submissionInfo, string& judgerId) {
    string filePath = string(oj_home) + "/submission/" + judgerId + ".json";
    submissionInfo.readFromFile(filePath);
}

void removeSubmissionInfo(string& uuid) {
    execute_cmd("/bin/rm -rf %s/submission/%s.json", oj_home, uuid.c_str());
}

vector<pair<string, int> >getFileList(const string& path, function<int(const char*)> func) {
    auto* dp = opendir(path.c_str());
    dirent* dirp;
    if (dp == nullptr) {
        write_log(oj_home, "No such dir:%s!\n", path.c_str());
        exit(-1);
    }
    vector<pair<string, int> >inFileList;
    while((dirp = readdir(dp)) != nullptr) {
        auto fileLen = func(dirp->d_name);
        if(fileLen) {
            inFileList.emplace_back(dirp->d_name, fileLen);
        }
    }
    return inFileList;
}

vector<pair<string, int> >getFileList(const string& path) {
    auto func = [&](const char*) -> int{return 1;};
    return getFileList(path, func);
}


int get_sim(int solution_id, int lang, int pid, int &sim_s_id) {
    if (no_sim) {
        return 0;
    }
    char src_pth[BUFFER_SIZE];
    sprintf(src_pth, "Main.%s", lang_ext[lang]);
    if (DEBUG) {
        cout << "get sim: " << src_pth << endl;
    }
    int sim = 0;
    if (!admin) {
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
        if (isC(lang)) {
            execute_cmd("/bin/ln -s ../data/%d/ac/%d.%s ../data/%d/ac/%d.%s", pid,
                        solution_id, lang_ext[0], pid, solution_id,
                        lang_ext[1]);
        }
        else if (isCPP(lang)) {
            execute_cmd("/bin/ln -s ../data/%d/ac/%d.%s ../data/%d/ac/%d.%s", pid,
                        solution_id, lang_ext[1], pid, solution_id,
                        lang_ext[0]);
        }

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
    return sim;
}

string getFileContent(const string& file) {
    string content, tmp;
    fstream fileStream(file.c_str());
    while (getline(fileStream, tmp)) {
        content += tmp + "\n";
    }
    return content;
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

void make_workdir(char* work_dir) {
    char shm_path[BUFFER_SIZE];
    execute_cmd("/bin/mkdir -p %s", work_dir);
    execute_cmd("/bin/chown -R judge %s ", work_dir);
    execute_cmd("/bin/chmod 755 %s", work_dir);
    sprintf(shm_path, "/dev/shm/hustoj/%s/", oj_home);
    execute_cmd("/bin/ln -s %s/data %s", oj_home, shm_path);
}

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


void fix_python_syntax_error_response(int &ACflg, int lang) {
    if (ACflg != ACCEPT) return;
    if (lang == PYTHON2 || lang == PYTHON3 || lang == PYPY || lang == PYPY3) {
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

void print_runtimeerror(const char *err) {
    FILE *ferr = fopen("error.out", "a+");
    fprintf(ferr, "Runtime Error:%s\n", err);
    fclose(ferr);
}


void getProblemInfoFromSubmissionInfo(SubmissionInfo& submissionInfo, double& time_lmt, int& mem_lmt, int& isspj) {
    time_lmt = submissionInfo.getTimeLimit();
    mem_lmt = int(submissionInfo.getMemoryLimit());
    isspj = int(submissionInfo.getSpecialJudge());
    if (time_lmt <= 0) {
        time_lmt = 1;
    }
}


void getCustomInputFromSubmissionInfo(SubmissionInfo& submissionInfo) {
    if (DEBUG) {
        cout << submissionInfo.getCustomInput() << endl;
    }
    char src_pth[BUFFER_SIZE];
    sprintf(src_pth, "data.in");
    FILE *fp_src = fopen(src_pth, "w");
    fprintf(fp_src, "%s", submissionInfo.getCustomInput().c_str());
    fclose(fp_src);
}


void getSolutionFromSubmissionInfo(SubmissionInfo& submissionInfo, char* usercode) {
    char src_pth[BUFFER_SIZE];
    sprintf(usercode, "%s", submissionInfo.getSource().c_str());
    sprintf(src_pth, "Main.%s", lang_ext[submissionInfo.getLanguage()]);
    if (DEBUG) {
        printf("Main=%s", src_pth);
        cout << usercode << endl;
    }
    FILE *fp_src = fopen(src_pth, "we");
    fprintf(fp_src, "%s", usercode);
    fclose(fp_src);
}


string getRuntimeInfoContents(const string& filename) {
    char buffer[4096];
    string runtimeInfo;
    FILE *fp = fopen(filename.c_str(), "re");
    while (fgets(buffer, 1024, fp)) {
        runtimeInfo += buffer;
        if (runtimeInfo.length() > 4096) {
            break;
        }
    }
    return runtimeInfo;
}

Language* getLanguageModel(int language) {
    string languageName = languageNameReader.GetString(to_string(language));
    void* languageHandler = dlopen(("/usr/lib/cupjudge/lib" + languageName + ".so").c_str(), RTLD_LAZY);
    if (!languageHandler) {
        cerr << "Cannot load library: " << dlerror() << endl;
        exit(1);
    }
    dlerror();
    createLanguageInstance* createInstance = (createLanguageInstance*) dlsym(languageHandler, (string("createInstance") + languageName).c_str());
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << endl;
        exit(1);
    }
//    destroyLanguageInstance* destroyInstance = (destroyLanguageInstance*) dlsym(languageHandler, (string("destroyInstance") + languageName).c_str());
//    dlsym_error = dlerror();
//    if (dlsym_error) {
//        cerr << "Cannot load symbol create: " << dlsym_error << endl;
//        exit(1);
//    }

    Language* languageInstance = createInstance();
    return languageInstance;
}

bool isPython(int language) {
    switch (language) {
        case PYTHON2:
        case PYTHON3:
        case PYPY:
        case PYPY3:
            return true;
        default:
            return false;
    }
    return false;
}

