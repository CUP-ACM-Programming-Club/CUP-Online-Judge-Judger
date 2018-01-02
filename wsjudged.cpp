#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/resource.h>

#define STD_MB 1<<20
using namespace std;

static int DEBUG = 0;

void run_client(int runid, int clientid, const string &oj_home = "") {
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
	const char *rid = to_string(runid).c_str();
	const char *cid = to_string(clientid).c_str();
	if (!DEBUG)
		execl("/usr/bin/judge_client", "/usr/bin/judge_client", rid, cid,
		      oj_home.c_str(), (char *) nullptr);
	else
		execl("/usr/bin/judge_client", "/usr/bin/judge_client", rid, cid,
		      oj_home.c_str(), "debug", (char *) nullptr);
}

int main(int argc, char **argv) {
	if (argc < 4) {
		cerr << "arguments format is invalid!" << endl;
		cerr << "Please run judge client as format like:" << endl;
		cerr << argv[0] << " ${solution_id} ${judger_id} ${oj_home_dir}" << endl;
		return 1;
	}
	if (argc == 5) {
		DEBUG = 1;
	}
	int solution_id = atoi(argv[1]);
	int judger_id = atoi(argv[2]);
	string oj_home_dir = argv[3];
	run_client(solution_id, judger_id, oj_home_dir);
	return 0;
}
