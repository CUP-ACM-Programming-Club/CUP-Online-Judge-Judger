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

#include "model/websocket/WebSocketSender.h"
#include "header/static_var.h"

#include "model/base/JSONVectorReader.h"
#include "model/base/Bundle.h"

#include "library/judge_lib.h"
#include "model/submission/SubmissionInfo.h"
#include "model/judge/policy/SpecialJudge.h"
#include "manager/syscall/InitManager.h"
#include "external/mysql/MySQLSubmissionAdapter.h"

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

//static int sleep_tmp;
#define ZOJ_COM

int call_counter[call_array_size];
static char LANG_NAME[BUFFER_SIZE];

// read the configue file
void init_mysql_conf() {
    strcpy(java_xms, "-Xms32m");
    strcpy(java_xmx, "-Xmx256m");
    host_name[0] = 0;
    user_name[0] = 0;
    password[0] = 0;
    db_name[0] = 0;
    database_port = 3306;
    string configDIR = oj_home;
    configDIR += "/etc/config.json";
    ConfigReader configReader(configDIR);
    strcpy(host_name, configReader.GetString("hostname").c_str());
    strcpy(user_name, configReader.GetString("username").c_str());
    strcpy(password, configReader.GetString("password").c_str());
    strcpy(db_name, configReader.GetString("db_name").c_str());
    database_port = configReader.GetInt("port");
    javaTimeBonus = configReader.GetInt("java_time_bonus");
    java_memory_bonus = configReader.GetInt("java_memory_bonus");
    strcpy(java_xms, configReader.GetString("java_xms").c_str());
    strcpy(java_xmx, configReader.GetString("java_xmx").c_str());
    sim_enable = configReader.GetInt("sim_enable");
    full_diff = configReader.GetInt("full_diff");
    strcpy(http_username, configReader.GetString("judger_name").c_str());
    SHARE_MEMORY_RUN = configReader.GetInt("shm_run");
    use_max_time = configReader.GetInt("use_max_time");
    use_ptrace = configReader.GetInt("use_ptrace");
    ALL_TEST_MODE = configReader.GetInt("all_test_mode");
    //  fclose(fp);
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
int compile(int lang, char *work_dir) {
    int pid;
    //webSocket << ws_send(solution_id, 2, NOT_FINISHED, ZERO_TIME, ZERO_MEMORY, ZERO_PASSPOINT, ZERO_PASSRATE);
    string configJSONDir = oj_home;
    configJSONDir += "/etc/compile.json";
    CompilerArgsReader compilerArgsReader(configJSONDir);
    shared_ptr<Language> languageModel(getLanguageModel(lang));
    pid = fork();
    if (pid == CHILD_PROCESS) {
        languageModel->setCompileProcessLimit();
        languageModel->setCompileExtraConfig();
        languageModel->setCompileMount(work_dir);
        while (setgid(1536) != 0)
            sleep(1);
        while (setuid(1536) != 0)
            sleep(1);
        while (setresuid(1536, 1536, 1536) != 0)
            sleep(1);
        if (DEBUG)
            cout << "Lang:" << lang << endl;
        auto args = compilerArgsReader.GetArray(to_string(lang));
        languageModel->compile(args, java_xms, java_xmx);
        if (DEBUG) {
            cout << "Copilation end!\n" << endl;
        }
        //exit(!system("cat ce.txt"));
        exit(0);
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        status = languageModel->getCompileResult(status);
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

void run_solution(int &lang, char *work_dir, double &time_lmt, double &usedtime,
                  int &mem_lmt) {
    shared_ptr<Language> languageModel(getLanguageModel(lang));
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
    if (use_ptrace) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    }
    // run me
    languageModel->buildChrootSandbox(work_dir);

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
    languageModel->setProcessLimit();

    // set the stack
    LIM.rlim_cur = static_cast<rlim_t>(STD_MB << 7);
    LIM.rlim_max = static_cast<rlim_t>(STD_MB << 7);
    setrlimit(RLIMIT_STACK, &LIM);
    // set the memory
    LIM.rlim_cur = static_cast<rlim_t>(STD_MB * mem_lmt / 2 * 3);
    LIM.rlim_max = static_cast<rlim_t>(STD_MB * mem_lmt * 2);
    languageModel->runMemoryLimit(LIM);
    if (!use_ptrace) {
        languageModel->buildSeccompSandbox();
    }
    languageModel->run(mem_lmt);
    //sleep(1);
    fflush(stderr);
    exit(0);
}

void judge_solution(int &ACflg, double &usedtime, double time_lmt, int isspj,
                    int p_id, char *infile, char *outfile, char *userfile, char *usercode, int &PEflg,
                    int lang, char *work_dir, int &topmemory, int mem_lmt,
                    int solution_id, int num_of_test, string& global_work_dir) {
    //usedtime-=1000;
    shared_ptr<Language> languageModel(getLanguageModel(lang));
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
    languageModel->fixACFlag(ACflg);
    // compare
    if (ACflg == ACCEPT) {
        if (isspj) {
            comp_res = SpecialJudge::newInstance().setDebug(DEBUG).run(oj_home, p_id, infile, outfile, userfile, usercode, global_work_dir);
        } else {
            shared_ptr<Compare::Compare> compare(getCompareModel());
            compare->setDebug(DEBUG);
            comp_res = compare->compare(outfile, userfile);

        if (comp_res == WRONG_ANSWER) {
            ACflg = WRONG_ANSWER;
            if (DEBUG)
                printf("fail test %s\n", infile);
        } else if (comp_res == PRESENTATION_ERROR)
            PEflg = PRESENTATION_ERROR;
        ACflg = comp_res;
    }
    //jvm popup messages, if don't consider them will get miss-WrongAnswer
    languageModel->fixFlagWithVMIssue(work_dir, ACflg, topmemory, mem_lmt);
}

void watch_solution(pid_t pidApp, char *infile, int &ACflg, int isspj,
                    char *userfile, char *outfile, int solution_id, int lang,
                    int &topmemory, int mem_lmt, double &usedtime, double time_lmt, int &p_id,
                    int &PEflg, char *work_dir) {
    // parent
    int tempmemory;
    shared_ptr<Language> languageModel(getLanguageModel(lang));
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
        tempmemory = languageModel->getMemory(ruse, pidApp);
        topmemory = max(tempmemory, topmemory);
        if (topmemory > mem_lmt * STD_MB) {
            if (DEBUG)
                printf("out of memory %d\n", topmemory);
            if (ACflg == ACCEPT)
                ACflg = MEMORY_LIMIT_EXCEEDED;
            if (use_ptrace) {
                ptrace(PTRACE_KILL, pidApp, NULL, NULL);
            }
            break;
        }
        //sig = status >> 8;/*status >> 8 EXITCODE*/

        if (WIFEXITED(status))
            break;
        bool has_error = get_file_size("error.out") > 0;
        if (has_error) {
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
            if (contents.find("Killed") != std::string::npos) {
                write_log(oj_home, contents.c_str());
                print_runtimeerror(contents.c_str());
                //ptrace(PTRACE_KILL, pidApp, NULL, NULL);
                //print_runtimeerror(contents.c_str());
                break;
            }
        }
        if (languageModel->gotErrorWhileRunning(has_error) && !ALL_TEST_MODE) {
            ACflg = RUNTIME_ERROR;
            //addreinfo(solution_id);
            if (use_ptrace) {
                ptrace(PTRACE_KILL, pidApp, NULL, NULL);
            }
            break;
        }


        if (!isspj
            && get_file_size(userfile)
               > get_file_size(outfile) * 2 + 1024) {
            ACflg = OUTPUT_LIMIT_EXCEEDED;
            if (use_ptrace) {
                ptrace(PTRACE_KILL, pidApp, NULL, NULL);
            }
            break;
        }

        exitcode = WEXITSTATUS(status);
        /*exitcode == 5 waiting for next CPU allocation          * ruby using system to run,exit 17 ok
         *  */
        if (languageModel->isValidExitCode(exitcode))
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
            if (use_ptrace) {
                ptrace(PTRACE_KILL, pidApp, NULL, NULL);
            }
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
        if (use_ptrace) {
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
    }

    usedtime += (ruse.ru_utime.tv_sec * 1000 + ruse.ru_utime.tv_usec / 1000);
    usedtime += (ruse.ru_stime.tv_sec * 1000 + ruse.ru_stime.tv_usec / 1000);

    //clean_session(pidApp);
}


void init_parameters(int argc, char **argv, int &solution_id,
                     int &runner_id, string& judgerId) {
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
        } else if (argType == _NO_SIM) {
            no_sim = true;
        } else if (argType == _NO_MYSQL) {
            MYSQL_MODE = false;
        } else if (argType == _STDIN) {
            READ_FROM_STDIN = true;
        }
        else {
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
                case _JUDGER_ID:
                    judgerId = string(argv[i]);
                    break;
                default:
                    error = true;
                    break;
            }
        }
    }
    if (!error) {
        chdir(oj_home);
        return;
    }
    else {// old parameter pass way
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
    string judgerId;
    char work_dir[BUFFER_SIZE];
    char usercode[CODESIZE];
    char user_id[BUFFER_SIZE];
    SubmissionInfo submissionInfo;
    shared_ptr<MySQLSubmissionAdapter> adapter;
    solution_id = DEFAULT_SOLUTION_ID;
    int runner_id = 0;
    int p_id, memoryLimit, lang, SPECIAL_JUDGE, sim, sim_s_id = ZERO_SIM;
    double max_case_time = ZERO_TIME;
    double timeLimit;
    init_parameters(argc, argv, solution_id, runner_id, judgerId);
    init_mysql_conf();
    initWebSocketConnection("localhost", 5100);
    bundle.setJudgerId(judgerId);
    //set work directory to start running & judging
    sprintf(work_dir, "%s/run%d/", oj_home, runner_id);
    string global_work_dir = string(work_dir);
    clean_workdir(work_dir);
    if (SHARE_MEMORY_RUN)
        mk_shm_workdir(work_dir);
    else {
        make_workdir(work_dir);
    }
    languageNameReader.loadFile(string(oj_home) + "/etc/language.json");
    chdir(work_dir);
    shared_ptr<Language> languageModel;

    if (MYSQL_MODE) {
        adapter = shared_ptr<MySQLSubmissionAdapter>(getAdapter());
        adapter->setPort(database_port)
        .setDBName(db_name)
        .setUserName(user_name)
        .setHostName(host_name)
        .setPassword(password);
        if (!adapter->start()) {
            cerr << "Failed to create a MYSQL connection." << endl;
            exit(1); //exit if mysql is down
        }
        adapter->getSolutionInfo(solution_id, p_id, user_id, lang);
        languageModel = shared_ptr<Language>(getLanguageModel(lang));
        adapter->getProblemInfo(abs(p_id), timeLimit, memoryLimit, SPECIAL_JUDGE);
        adapter->getSolution(solution_id, work_dir, lang, usercode, languageModel->getFileSuffix().c_str(), DEBUG);
    }
    else {
        buildSubmissionInfo(submissionInfo, judgerId);
        getSolutionInfoFromSubmissionInfo(submissionInfo, p_id, user_id, lang);
        languageModel = shared_ptr<Language>(getLanguageModel(lang));
        getProblemInfoFromSubmissionInfo(submissionInfo, timeLimit, memoryLimit, SPECIAL_JUDGE);
        getSolutionFromSubmissionInfo(submissionInfo, usercode);
    }
    timeLimit = languageModel->buildTimeLimit(timeLimit, javaTimeBonus);
    memoryLimit = languageModel->buildMemoryLimit(memoryLimit, java_memory_bonus);
    languageModel->setExtraPolicy(oj_home, work_dir);
/*
*/
    //get the limit
    if (p_id <= TEST_RUN_SUBMIT) {//Is custom input
        SPECIAL_JUDGE = NONE_SPECIAL_JUDGE;
    }
    //copy source file
    //java is lucky

    //never bigger than judged set value;
    if (timeLimit > 300 * SECOND || timeLimit < ZERO) {
        timeLimit = 300 * SECOND;
    }
    if (memoryLimit > ONE_KILOBYTE || memoryLimit < ONE) {
        memoryLimit = ONE_KILOBYTE;//ONE_KILOBYTE MB
    }
    if (DEBUG) {
        printf("time: %f mem: %d\n", timeLimit, memoryLimit);
    }

    bundle.clear();
    bundle.setJudger(http_username);
    bundle.setSolutionID(solution_id);
    bundle.setResult(COMPILING);
    bundle.setFinished(NOT_FINISHED);
    bundle.setUsedTime(ZERO_TIME);
    bundle.setMemoryUse(ZERO_MEMORY);
    bundle.setPassPoint(ZERO_PASSPOINT);
    bundle.setPassRate(ZERO_PASSRATE);
    webSocket << bundle.toJSONString();
    if (compile(lang, work_dir) != COMPILED) {
        string _compile_info = getFileContent("ce.txt");
        bundle.setJudger(http_username);
        bundle.setSolutionID(solution_id);
        bundle.setResult(COMPILE_ERROR);
        bundle.setFinished(FINISHED);
        bundle.setCompileInfo(_compile_info);
        webSocket << bundle.toJSONString();
        clean_workdir(work_dir);
        removeSubmissionInfo(judgerId);
        write_log(oj_home, "compile error");
        exit(0);
    } else {
        umount(work_dir);
    }
    char fullpath[BUFFER_SIZE];
    char infile[BUFFER_SIZE];
    char outfile[BUFFER_SIZE];
    char userfile[BUFFER_SIZE];
    sprintf(fullpath, "%s/data/%d", oj_home, p_id); // the fullpath of data dir
    int ACflg, PEflg;
    ACflg = PEflg = ACCEPT;
    int topmemory = ZERO_MEMORY;
    double usedtime = ZERO_TIME;

    //create chroot for ruby bash python
    languageModel->buildRuntime(work_dir);
    // read files and run
    double pass_rate = ZERO_PASSRATE;
    int num_of_test = 0;
    int finalACflg = ACflg;
    if (p_id <= TEST_RUN_PROBLEM) {  //custom input running
        printf("running a custom input...\n");
        if (MYSQL_MODE) {
            adapter->getCustomInput(solution_id, work_dir);
        }
        else {
            getCustomInputFromSubmissionInfo(submissionInfo);
        }
        InitManager::initSyscallLimits(lang, call_counter, record_call, call_array_size);
        bundle.setJudger(http_username);
        bundle.setSolutionID(solution_id);
        bundle.setResult(RUNNING_JUDGING);
        webSocket << bundle.toJSONString();
        pid_t pidApp = fork();

        if (pidApp == CHILD_PROCESS) {
            run_solution(lang, work_dir, timeLimit, usedtime, memoryLimit);
        } else {
            watch_solution(pidApp, infile, ACflg, SPECIAL_JUDGE, userfile, outfile,
                           solution_id, lang, topmemory, memoryLimit, usedtime, timeLimit,
                           p_id, PEflg, work_dir);

        }
        ACflg = languageModel->fixACStatus(ACflg);
        string error_message;
        if (ACflg == TIME_LIMIT_EXCEEDED) {
            usedtime = timeLimit * 1000;
            error_message = "Time Limit Exceeded.Kill Process.\n";
        } else if (ACflg == RUNTIME_ERROR) {
            if (DEBUG)
                printf("add RE info of %d..... \n", solution_id);
            error_message = "Runtime Error. Kill Process.\n";
        } else if (ACflg == MEMORY_LIMIT_EXCEEDED) {
            error_message = "Memory Limit Exceeded.Kill Process.\n";
        }
        string test_run_out;
        if (ACflg == ACCEPT) {
            test_run_out = getRuntimeInfoContents("user.out");
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

        if(usedtime == timeLimit * 1000) {
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
        clean_workdir(work_dir);
        removeSubmissionInfo(judgerId);
        exit(0);
    }
    int total_point = 0;
    int pass_point = ZERO_PASSPOINT;
    vector<pair<string, int> >inFileList = getFileList(fullpath, isInFile);
    total_point = inFileList.size();
    bundle.setJudger(http_username);
    bundle.setSolutionID(solution_id);
    bundle.setResult(RUNNING_JUDGING);
    bundle.setTotalPoint(total_point);
    webSocket << bundle.toJSONString();
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
            InitManager::initSyscallLimits(lang, call_counter, record_call, call_array_size);
            pid_t pidApp = fork();

            if (pidApp == CHILD_PROCESS) {
                if (DEBUG) {
                    printf("Running solution\n");
                    cout << "Time limit ALL_TEST_MODE:" << (timeLimit + 1) << endl;
                    cout << "Time limit NORMAL:" << ((timeLimit - usedtime / 1000) + 1) << endl;
                }
                run_solution(lang, work_dir, timeLimit, usedtime, memoryLimit);
            } else {

                if (DEBUG) {
                    cout << "Run test point:" << num_of_test << endl;
                }
                watch_solution(pidApp, infile, ACflg, SPECIAL_JUDGE, userfile, outfile,
                               solution_id, lang, topmemory, memoryLimit, usedtime, timeLimit,
                               p_id, PEflg, work_dir);
                judge_solution(ACflg, usedtime, timeLimit, SPECIAL_JUDGE, p_id, infile,
                               outfile, userfile, usercode, PEflg, lang, work_dir, topmemory,
                               memoryLimit, solution_id, num_of_test, global_work_dir);
                if (use_max_time) {
                    max_case_time = max(usedtime, max_case_time);
                    usedtime = ZERO_TIME;
                }
            }

            if (usedtime > timeLimit * 1000 || ACflg == TIME_LIMIT_EXCEEDED) {
                cout << "Time Limit Exceeded" << endl;
                ACflg = TIME_LIMIT_EXCEEDED;
                usedtime = timeLimit * 1000;
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
        bundle.setUsedTime(min(usedtime, timeLimit * 1000));
        bundle.setMemoryUse(min(topmemory / ONE_KILOBYTE, memoryLimit * STD_MB / ONE_KILOBYTE));
        bundle.setPassPoint(pass_point);
        bundle.setPassRate(pass_rate / num_of_test);
        webSocket << bundle.toJSONString();
    }
    if (ALL_TEST_MODE) {
        ACflg = finalACflg;
    }
    if (ACflg == ACCEPT && PEflg == PRESENTATION_ERROR) {
        ACflg = PRESENTATION_ERROR;
    }
    if (sim_enable && ACflg == ACCEPT && (!ALL_TEST_MODE || finalACflg == ACCEPT)
        && (languageModel->enableSim())) { //bash don't supported
        sim = get_sim(solution_id, lang, p_id, sim_s_id);
    } else {
        sim = ZERO_SIM;
    }
    //if(ACflg == OJ_RE)addreinfo(solution_id);
    string runtimeInfo;
    if ((ALL_TEST_MODE && finalACflg == RUNTIME_ERROR) || ACflg == RUNTIME_ERROR) {
        runtimeInfo = getRuntimeInfoContents("error.out");
        if (DEBUG)
            printf("add RE info of %d..... \n", solution_id);
    }
    if (use_max_time) {
        usedtime = max_case_time;
    }
    if (ACflg == TIME_LIMIT_EXCEEDED || (ALL_TEST_MODE && finalACflg == TIME_LIMIT_EXCEEDED)) {
        usedtime = timeLimit * 1000;
    }
    if ((ACflg == WRONG_ANSWER || ACflg == PRESENTATION_ERROR)) {
        runtimeInfo = getRuntimeInfoContents("diff.out");
        if (DEBUG)
            printf("add diff info of %d..... \n", solution_id);
    }
    bundle.setResult(ALL_TEST_MODE ? finalACflg : ACflg);
    bundle.setFinished(FINISHED);
    bundle.setRuntimeInfo(runtimeInfo);
    bundle.setUsedTime(usedtime);
    bundle.setMemoryUse(topmemory / ONE_KILOBYTE);
    bundle.setPassPoint(pass_point);
    bundle.setPassRate(pass_rate / num_of_test);
    bundle.setSim(sim);
    bundle.setSimSource(sim_s_id);
    webSocket << bundle.toJSONString();
    clean_workdir(work_dir);
    removeSubmissionInfo(judgerId);
    if (DEBUG) {
        write_log(oj_home, "result=%d", ALL_TEST_MODE ? finalACflg : ACflg);
    }
    if (record_call) {
        print_call_array();
    }
    return 0;
}
