//
// Created by Ryan on 2018/10/18.
//

#include "CompilerArgsReader.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <iostream>
#include <fstream>

using std::fstream;
using rapidjson::Document;
using rapidjson::Value;
using std::cout;
using namespace std;
CompilerArgsReader::CompilerArgsReader(const string &filePath) {
    loadFile(filePath);
}

bool CompilerArgsReader::loadFile(const string &filePath) {
    fstream JSON_FILE(filePath.c_str());
    string s, line;
    while (getline(JSON_FILE, line)) {
        s += line;
    }
    JSON_FILE.close();
    return isLoad = !document.ParseInsitu((char *) s.c_str()).HasParseError();
}

bool CompilerArgsReader::loadJSON(string jsonPlainText) {
    return !document.ParseInsitu((char *) jsonPlainText.c_str()).HasParseError();
}

vector<string> CompilerArgsReader::Get(string key) {
    if (!isLoad) {
        throw "No JSON file is loaded";
    }

    vector<string> vec;
    auto findMember = document.FindMember(key.c_str());
    if(findMember == document.MemberEnd()) {
        return vec;
    }
    const Value& val = document[key.c_str()];
    for(Value::ConstValueIterator iter = val.Begin();iter != val.End();++iter) {
        vec.emplace_back(iter->GetString());
    }
    return vec;
}

