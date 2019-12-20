//
// Created by Haoyuan Li on 2019/12/19.
//

#ifndef JUDGE_CLIENT_SUBMISSIONINFO_H
#define JUDGE_CLIENT_SUBMISSIONINFO_H

#include <string>
using std::string;
class SubmissionInfo {
private:
    string source;
    string customInput;
    double timeLimit;
    double memoryLimit;
    bool specialJudge;
    int problemId;
    string userId;
    int language;

public:
    SubmissionInfo& setSource(string&);
    SubmissionInfo& setCustomInput(string&);
    SubmissionInfo& setTimeLimit(double);
    SubmissionInfo& setMemoryLimit(double);
    SubmissionInfo& setSpecialJudge(bool);
    SubmissionInfo& setProblemId(int);
    SubmissionInfo& setUserId(string&);
    SubmissionInfo& setLanguage(int);
    SubmissionInfo& readFromFile(string&);

    string& getSource();
    string& getCustomInput();
    double getTimeLimit();
    double getMemoryLimit();
    bool getSpecialJudge();
    int getProblemId();
    string& getUserId();
    int getLanguage();
};


#endif //JUDGE_CLIENT_SUBMISSIONINFO_H
