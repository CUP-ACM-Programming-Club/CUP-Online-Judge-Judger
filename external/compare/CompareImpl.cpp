//
// Created by Haoyuan Li on 2020/3/26.
//

#include "CompareImpl.h"
#include "utils/utils.h"
#include <iostream>
#include <fstream>
using namespace std;


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


bool is_not_character(int c) {
    return (iscntrl(c) || isspace(c));
}

bool is_number(const string &s) {
    for (auto c:s) {
        if (!isdigit(c))return false;
    }
    return true;
}

void move_to_next_nonspace_character(int &c, FILE *&f, int &ret) {
    while (isspace(c) || iscntrl(c)) {
        do {
            c = fgetc(f);
        } while (isspace(c) || iscntrl(c));
    }
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


/*
 * translated from ZOJ judger r367
 * http://code.google.com/p/zoj/source/browse/trunk/judge_client/client/text_checker.cc#25
 *
 */
int choose = 1;

void find_next_nonspace(int &c1, int &c2, FILE *&f1, FILE *&f2, int &ret, int DEBUG) {
    // Find the next non-space character or \n.
    while ((isspace(c1)) || (isspace(c2))) {
        if (c1 != c2) {
            if (c2 == EOF) {
                do {
                    c1 = fgetc(f1);
                } while (isspace(c1));
                continue;
            } else if (c1 == EOF) {
                do {
                    c2 = fgetc(f2);
                } while (isspace(c2));
                continue;
#ifdef IGNORE_ESOL
                } else if (isspace(c1) && isspace(c2)) {
                while (c2 == '\n' && isspace(c1) && c1 != '\n')
                    c1 = fgetc(f1);
                while (c1 == '\n' && isspace(c2) && c2 != '\n')
                    c2 = fgetc(f2);

#else
            } else if ((c1 == '\r' && c2 == '\n')) {
                c1 = fgetc(f1);
            } else if ((c2 == '\r' && c1 == '\n')) {
                c2 = fgetc(f2);
#endif
            } else {
                if (DEBUG)
                    printf("%d=%c\t%d=%c", c1, c1, c2, c2);
                ret = PRESENTATION_ERROR;
            }
        }
        if (isspace(c1)) {
            c1 = fgetc(f1);
        }
        if (isspace(c2)) {
            c2 = fgetc(f2);
        }
    }
}

int compare_zoj(const char *file1, const char *file2, int DEBUG, int full_diff) {
    if (DEBUG && choose) {
        do {
            fstream user_out(file1), error_file(file2);
            string tmp;
            cout << "Display user file?(0/1)" << endl;
            cin >> choose;
            if (choose) {
                cout << "user running file" << endl;
                while (getline(user_out, tmp))
                    cout << tmp << endl;
            } else break;
            cout << "Display error file?(0/1)" << endl;
            cin >> choose;
            if (choose) {
                cout << "error msg" << endl;
                while (getline(error_file, tmp))
                    cerr << tmp << endl;
            } else break;
        } while (0);
    }
    int ret = ACCEPT;
    int c1, c2;
    FILE *f1, *f2;
    f1 = fopen(file1, "re");
    f2 = fopen(file2, "re");
    auto judge_file_end_or_detect_presentation_error = [&](int c1, int c2, FILE *&f1, FILE *&f2, int &ret) -> void {
        if (is_not_character(c1) && c1 != EOF) {
            auto temp_ret = ret;
            if (c2 != EOF)
                ret = PRESENTATION_ERROR;
            move_to_next_nonspace_character(c1, f1, ret);
            if (c1 == EOF) {
                if (is_not_character(c2)) {
                    move_to_next_nonspace_character(c2, f2, ret);
                }
                if (c2 == EOF) {
                    ret = temp_ret;
                }
            }
        }
    };
    if (!f1 || !f2) {
        ret = RUNTIME_ERROR;
    } else
        for (;;) {
            // Find the first non-space character at the beginning of line.
            // Blank lines are skipped.
            c1 = fgetc(f1);
            c2 = fgetc(f2);
            find_next_nonspace(c1, c2, f1, f2, ret, DEBUG);
            // Compare the current line.
            for (;;) {
                // Read until 2 files return a space or 0 together.
                while ((!isspace(c1) && c1) || (!isspace(c2) && c2)) {
                    if (c1 == EOF && c2 == EOF) {
                        goto end;
                    }
                    if (c1 == EOF || c2 == EOF) {
                        break;
                    }

                    if (c1 != c2) {
                        if (DEBUG) {
                            cerr << "c1:" << (char) c1 << " c2:" << (char) c2 << endl;
                        }
                        // Consecutive non-space characters should be all exactly the same
                        ret = WRONG_ANSWER;
                        goto end;
                    }
                    c1 = fgetc(f1);
                    c2 = fgetc(f2);
                    while (c1 == '\r')c1 = fgetc(f1);
                    //while(c2 == '\r')c2 = fgetc(f2);
                    if (c1 != c2) {
                        cerr << "c1:" << c1 << " c2:" << c2 << endl;
                        judge_file_end_or_detect_presentation_error(c1, c2, f1, f2, ret);
                        judge_file_end_or_detect_presentation_error(c2, c1, f2, f1, ret);
                    }
                }
                find_next_nonspace(c1, c2, f1, f2, ret, DEBUG);
                if (c1 == EOF && c2 == EOF) {
                    goto end;
                }
                if (c1 == EOF || c2 == EOF) {
                    ret = WRONG_ANSWER;
                    goto end;
                }

                if ((c1 == '\n' || !c1) && (c2 == '\n' || !c2)) {
                    break;
                }
            }
        }
    end:
    if (ret == PRESENTATION_ERROR) {
        if (!check_valid_presentation_error(file1, file2)) {
            ret = WRONG_ANSWER;
        }
    }
    if (ret == WRONG_ANSWER || ret == PRESENTATION_ERROR) {
        if (full_diff)
            make_diff_out_full(f1, f2, c1, c2, file1);
        else
            make_diff_out_simple(f1, f2, c1, c2, file1);
    }
    if (f1)
        fclose(f1);
    if (f2)
        fclose(f2);
    return ret;
}


int CompareImpl::compare(const char *stdAnswerFile, const char *userOutputFile) {
    return compare_zoj(stdAnswerFile, userOutputFile, DEBUG, full_diff);
}

extern "C" Compare::Compare* createInstance() {
    return new CompareImpl;
}

extern "C" void destroyInstance(Compare::Compare* compare) {
    delete compare;
}
