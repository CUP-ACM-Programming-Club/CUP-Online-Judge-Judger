//
// Created by Haoyuan Li on 2019/12/24.
//

#include "SpecialJudge.h"
#include <iostream>
#include <fstream>
#include "unistd.h"
#include "../../../header/static_var.h"
#include <sys/resource.h>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::fstream;

int SpecialJudge::run(char *oj_home, int problem_id, char *infile, char *outfile, char *userfile, char *usercode, string &global_work_dir) {
    pid_t pid;
    printf("pid=%d\n", problem_id);
    printf("%s\n", usercode);
    string outfiled(global_work_dir);
    outfiled += "usercode.code";
    ofstream userout(outfiled);
    userout << usercode;
    userout.close();
    fstream users(userfile);
    string tmp;
    freopen("/dev/tty", "w", stdout);
    if (DEBUG) {
        while (getline(users, tmp))
            cout << tmp << endl;
    }
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
        cout << "spj1=" << ret << endl;
        if (ret)
            ret = WEXITSTATUS(ret);
        if (ret && ret < ACCEPT) {
            ret = WRONG_ANSWER;
        }
        exit(ret);
    } else {
        int status;
        cout << "fork pid: " << pid << endl;
        waitpid(pid, &status, 0);
        cout << "status:" << status << '\n';
        ret = WEXITSTATUS(status);
        cout << "spj2=" << ret << '\n';
    }
    return compatibleParse(ret);
}

SpecialJudge& SpecialJudge::setDebug(bool debug) {
    this->DEBUG = debug;
}

int SpecialJudge::compatibleParse(int ret) {
    if (ret < 4) {
        if (ret == 0) {
            return ACCEPT;
        }
        else {
            if (DEBUG) {
                cout << "Fail test " << endl;
            }
            return WRONG_ANSWER;
        }
    }
    return ret;
}

SpecialJudge SpecialJudge::newInstance() {
    return SpecialJudge();
}
