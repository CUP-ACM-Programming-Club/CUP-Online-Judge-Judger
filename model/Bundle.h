//
// Created by Ryan on 2018-12-18.
//

#ifndef JUDGE_CLIENT_BUNDLE_H
#define JUDGE_CLIENT_BUNDLE_H

#include <string>
#include <algorithm>
#include <unordered_map>
#include "../library/judge_lib.h"

using std::unordered_map;
using std::string;
using std::to_string;

enum {
    INT,
    DOUBLE,
    STRING,
    UNDEFINED
};

class Pack {
public:
    Pack(){
        type = UNDEFINED;
    }
    Pack(int num){
        type = INT;
        this->num = num;
    }
    Pack(double num) {
        type = DOUBLE;
        this->f = num;
    }
    Pack(string str) {
        type = STRING;
        this->str = str;
    }
    int getInt() {
       return num;
    }

    double getFloat() {
        return f;
    }

    string getString() {
        return str;
    }

    int getType() {
        return type;
    }
private:
    int num;
    double f;
    string str;
    int type;
};

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
    bool setJudger(char*);
    bool setJudger(string&);
    bool setTestRunResult(string&);
    bool setCompileInfo(string&);
    bool setSim(int);
    bool setSimSource(int);
    bool setTotalPoint(int);
    void clear();
    string toJSONString();
    operator string();
private:
    unordered_map<string, Pack> _map;
    void init();
    bool setValue(string key, Pack val);
    string trim(string&);
    string checkUTF8Valid(string&);
    const int CODE_LENGTH_LIMIT = 1 << 13;
};


#endif //JUDGE_CLIENT_BUNDLE_H
