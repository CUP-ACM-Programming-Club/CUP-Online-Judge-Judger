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

    Bundle& setSolutionID(int);

    Bundle& setResult(int);

    Bundle& setFinished(int);

    Bundle& setUsedTime(double);

    Bundle& setMemoryUse(int);

    Bundle& setPassPoint(int);

    Bundle& setPassRate(double);

    Bundle& setJudger(char *);

    Bundle& setJudger(string &);

    Bundle& setTestRunResult(string &);

    Bundle& setCompileInfo(string &);

    Bundle& setSim(int);

    Bundle& setSimSource(int);

    Bundle& setTotalPoint(int);

    Bundle& setRuntimeInfo(string &);

    Bundle& clear();

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
