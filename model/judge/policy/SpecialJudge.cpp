//
// Created by Haoyuan Li on 2019/12/24.
//

#include "SpecialJudge.h"
#include <iostream>
#include <fstream>
#include "unistd.h"
#include "../../../header/static_var.h"
#include <sys/resource.h>
#include <sys/wait.h>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::fstream;

int SpecialJudge::run(char *oj_home, int problem_id, char *infile, char *outfile, char *userfile, char *usercode, string &global_work_dir) {
    pid_t pid;
    int fd[2];
    int *write_fd = &fd[1];
    int *read_fd = &fd[0];
    char buffer[1 << 7];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 16;
    }
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
        close(*read_fd);
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
        if (WIFEXITED(ret)) {
            cout << "WIFEXITED:" << WIFEXITED(ret) << endl;
        } else {
            cout << "ERROR WIFEXITED:" << WIFEXITED(ret) << endl;
        }
        cout << "spj1=" << ret << '\n';
        if (ret) {
            ret = WEXITSTATUS(ret);
            cout << "ret = " << ret << endl;
        }
        if (ret && ret < ACCEPT) {
            ret = WRONG_ANSWER;
        }
        sprintf(buffer, "%d", ret);
        write(*write_fd, buffer, strlen(buffer));
        close(*write_fd);
        exit(0);
    } else {
        close(*write_fd);
        read(*read_fd, buffer, sizeof(buffer) - 1);
        cout << "read buffer: " << buffer << endl;
        sscanf(buffer, "%d", &ret);
        int status;
        cout << "fork pid: " << pid << endl;
        waitpid(pid, &status, 0);
        cout << "status:" << status << '\n';
        cout << "spj2=" << ret << '\n';
        close(*read_fd);
    }
    return compatibleParse(ret);
}

SpecialJudge& SpecialJudge::setDebug(bool debug) {
    this->DEBUG = debug;
    return *this;
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
