//
// Created by Ryan Lee on 2018/7/17.
//
#include "judge_lib.h"
#include "../header/static_var.h"
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>

using namespace std;
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
        } else if (!strcmp(argument + 1, "stdin")) {
            return _STDIN;
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

void getSolutionInfoFromSubmissionInfo(SubmissionInfo& submissionInfo, int& p_id, char* user_id, int& lang) {
    p_id = submissionInfo.getProblemId();
    sprintf(user_id, "%s", submissionInfo.getUserId().c_str());
    lang = submissionInfo.getLanguage();
}

void buildSubmissionInfo(SubmissionInfo& submissionInfo, string& judgerId) {
    if (READ_FROM_STDIN) {
        string jsonFromStdin;
        std::getline(cin, jsonFromStdin);
        submissionInfo.readJSON(jsonFromStdin);
    }
    else {
        string filePath = string(oj_home) + "/submission/" + judgerId + ".json";
        submissionInfo.readFromFile(filePath);
    }
}

void removeSubmissionInfo(string& uuid) {
    execute_cmd("/bin/rm -rf %s/submission/%s.json", oj_home, uuid.c_str());
}

vector<pair<string, int> >getFileList(const string& path, const function<int(const char*)>& func) {
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
    shared_ptr<Language> languageModel(getLanguageModel(lang));
    char src_pth[BUFFER_SIZE];
    sprintf(src_pth, "Main.%s", languageModel->getFileSuffix().c_str());
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
                    languageModel->getFileSuffix().c_str());
        //c cpp will
        string suffix = languageModel->getFileSuffix();
        if (suffix == "c") {
            execute_cmd("/bin/ln -s %s/data/%d/ac/%d.%s %s/data/%d/ac/%d.%s", oj_home, pid,
                        solution_id, "c", oj_home, pid, solution_id,
                        "cc");
        }
        else if (suffix == "cc") {
            execute_cmd("/bin/ln -s %s/data/%d/ac/%d.%s %s/data/%d/ac/%d.%s", oj_home, pid,
                        solution_id, "cc", oj_home, pid, solution_id,
                        "c");
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
    stringstream ss;
    ss << fileStream.rdbuf();
    return ss.str();
}

void mk_shm_workdir(char *work_dir) {
    char shm_path[BUFFER_SIZE];
    sprintf(shm_path, "%s/submission", oj_home);
    execute_cmd("/bin/chown -R judge %s", shm_path);
    execute_cmd("/bin/chgrp -R judge %s", shm_path);
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
    sprintf(shm_path, "%s/submission", oj_home);
    execute_cmd("/bin/chown -R judge %s", shm_path);
    execute_cmd("/bin/chgrp -R judge %s", shm_path);
    execute_cmd("/bin/mkdir -p %s", work_dir);
    execute_cmd("/bin/chown -R judge %s ", work_dir);
    execute_cmd("/bin/chgrp -R judge %s ", work_dir);
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

void prepare_files_with_id(const char *filename, int namelen, char *infile, int &p_id,
                   char *work_dir, char *outfile, char *userfile, int runner_id, int file_id) {
    //              printf("ACflg=%d %d check a file!\n",ACflg,solution_id);

    char fname0[BUFFER_SIZE];
    char fname[BUFFER_SIZE];
    strncpy(fname0, filename, static_cast<size_t>(namelen));
    fname0[namelen] = 0;
    escape(fname, fname0);
    printf("%s\n%s\n", fname0, fname);
    sprintf(infile, "%s/data/%d/%s.in", oj_home, p_id, fname);
    execute_cmd("/bin/cp '%s' %s/data%d.in", infile, work_dir, file_id);
    execute_cmd("/bin/cp %s/data/%d/*.dic %s/", oj_home, p_id, work_dir);
    sprintf(outfile, "%s/data/%d/%s.out", oj_home, p_id, fname0);
    sprintf(userfile, "%s/run%d/user%d.out", oj_home, runner_id, file_id);
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
    shared_ptr<Language> languageModel(getLanguageModel(submissionInfo.getLanguage()));
    sprintf(usercode, "%s", submissionInfo.getSource().c_str());
    sprintf(src_pth, "Main.%s", languageModel->getFileSuffix().c_str());
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

template<class Instance>
Instance* getDynamicLibraryInstance (const char* dynamicLibraryPath, const char* createInstanceMethodName) {
    void* handler = dlopen(dynamicLibraryPath, RTLD_LAZY);
    if (!handler) {
        cerr << "Cannot load library: " << dynamicLibraryPath << ": " << dlerror() << endl;
        exit(1);
    }
    dlerror();
    Instance* (*createInstance)();
    createInstance = (Instance *(*)())(dlsym(handler, createInstanceMethodName));
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol " << createInstanceMethodName << " create: " << dlsym_error << endl;
        exit(1);
    }
    return createInstance();
}


Language* getLanguageModel(int language) {
    string languageName = languageNameReader.GetString(to_string(language));
    return getDynamicLibraryInstance<Language>(("/usr/lib/cupjudge/lib" + languageName + ".so").c_str(), (string("createInstance") + languageName).c_str());
}

MySQLSubmissionAdapter* getAdapter() {
    return getDynamicLibraryInstance<MySQLSubmissionAdapter>("/usr/lib/cupjudge/libmysql.so", "createInstance");
}

Compare::Compare* getCompareModel() {
    return getDynamicLibraryInstance<Compare::Compare>("/usr/lib/cupjudge/libcompare.so", "createInstance");
}

void setRunUser () {
    while (setgid(1536) != 0)
        sleep(1);
    while (setuid(1536) != 0)
        sleep(1);
    while (setresuid(1536, 1536, 1536) != 0)
        sleep(1);
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

