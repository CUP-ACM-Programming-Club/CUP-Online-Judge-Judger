#include <iostream>
#include <unistd.h>
#include <sys/resource.h>

using namespace std;
#define STD_MB 1<<20

int main(int argc, const char **argv) {
    int DEBUG = 0;
    if (argc < 4) {
        cerr << "arguments format is invalid!" << endl;
        cerr << "Please run judge client as format like:" << endl;
        cerr << argv[0] << " ${solution_id} ${judger_id} ${oj_home_dir}" << endl;
        return 1;
    }
    if (argc == 5) {
        DEBUG = 1;
    }

    struct rlimit LIM{};
    LIM.rlim_max = 800;
    LIM.rlim_cur = 800;
    setrlimit(RLIMIT_CPU, &LIM);
    LIM.rlim_max = 180 * STD_MB;
    LIM.rlim_cur = 180 * STD_MB;
    setrlimit(RLIMIT_FSIZE, &LIM);
    LIM.rlim_max = static_cast<rlim_t>(STD_MB << 11);
    LIM.rlim_cur = static_cast<rlim_t>(STD_MB << 11);
    setrlimit(RLIMIT_AS, &LIM);
    LIM.rlim_cur = LIM.rlim_max = 200;
    setrlimit(RLIMIT_NPROC, &LIM);
    const char *args[argc + 5];
    args[0] = "/usr/bin/judge_client";
    for (int i = 1; i < argc; ++i) {
        args[i] = argv[i];
    }
    if (DEBUG) {
        args[argc++] = "DEBUG";
    }
    args[argc] = nullptr;
    execvp(args[0], (char *const *) args);
    return 0;
}

