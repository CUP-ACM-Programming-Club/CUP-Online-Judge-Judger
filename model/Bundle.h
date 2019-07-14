//
// Created by Ryan on 2018-12-18.
//

#ifndef JUDGE_CLIENT_BUNDLE_H
#define JUDGE_CLIENT_BUNDLE_H

#include <string>
#include <algorithm>
#include <unordered_map>
#include "Pack.h"

using std::unordered_map;
using std::string;
using std::to_string;


class Bundle {
public:
    Bundle();

    ~Bundle() = default;

    bool setSolutionID(int);

    bool setResult(int);

    bool setFinished(int);

    bool setUsedTime(double);

    bool setMemoryUse(int);

    bool setPassPoint(int);

    bool setPassRate(double);

    bool setJudger(char *);

    bool setJudger(string &);

    bool setTestRunResult(string &);

    bool setCompileInfo(string &);

    bool setSim(int);

    bool setSimSource(int);

    bool setTotalPoint(int);

    void clear();

    string toJSONString();

    operator string();

private:
    unordered_map<string, Pack> _map;

    void init();

    bool setValue(const string& key, Pack val);

    string trim(string &);

    string checkUTF8Valid(string &);

    Pack &get(const string&);

    bool has(const string&);

    const unsigned CODE_LENGTH_LIMIT = 1 << 13;
};


#endif //JUDGE_CLIENT_BUNDLE_H
