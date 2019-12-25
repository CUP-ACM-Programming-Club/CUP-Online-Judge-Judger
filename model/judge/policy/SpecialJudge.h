//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_SPECIALJUDGE_H
#define JUDGE_CLIENT_SPECIALJUDGE_H
#include <string>
using std::string;

class SpecialJudge {
public:
    int run(char*, int, char*, char*, char*, char*, string&);
    SpecialJudge& setDebug(bool);
    static SpecialJudge newInstance();
private:
    int compatibleParse(int);
    bool DEBUG = false;
};


#endif //JUDGE_CLIENT_SPECIALJUDGE_H
