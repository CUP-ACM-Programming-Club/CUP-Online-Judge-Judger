//
// Created by Ryan on 2018-12-18.
//

#ifndef JUDGE_CLIENT_BUNDLE_H
#define JUDGE_CLIENT_BUNDLE_H

#include <string>
#include <algorithm>
#include <unordered_map>

using std::unordered_map;
using std::string;
using std::to_string;



class Bundle {
public:
    Bundle() = default;

    ~Bundle() = default;

private:
    unordered_map<string, string> _map;

    template<class T>
    bool setValue(const string key,const string val);
    bool setSolutionID(int);
    bool setResult(int);
    bool setFinished(int);
    bool setUsedTime(double);
    bool setMemoryUse(int);
    bool setPassPoint(int);
    bool setPassRate(double);
    bool setTestRunResult(string&);
    bool setCompileInfo(string&);
    bool setSim(int);
    bool setSimSource(int);
    bool setTotalPoint(int);
};


#endif //JUDGE_CLIENT_BUNDLE_H
