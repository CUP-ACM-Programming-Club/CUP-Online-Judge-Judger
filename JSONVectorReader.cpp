//
// Created by Ryan on 2018/10/18.
//

#include "JSONVectorReader.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <iostream>
#include <fstream>
#define ASSERT_VALID(x) assert(x)
using std::fstream;
using rapidjson::Document;
using rapidjson::Value;
using std::cout;
using namespace std;
JSONVectorReader::JSONVectorReader(const string &filePath) {
    loadFile(filePath);
}

bool JSONVectorReader::loadFile(const string &filePath) {
    fstream JSON_FILE(filePath.c_str());
    string s, line;
    while (getline(JSON_FILE, line)) {
        s += line;
    }
    JSON_FILE.close();
    return isLoad = !document.ParseInsitu((char *) s.c_str()).HasParseError();
}

bool JSONVectorReader::loadJSON(string jsonPlainText) {
    return !document.ParseInsitu((char *) jsonPlainText.c_str()).HasParseError();
}

vector<string> JSONVectorReader::GetArray(string key) {
    documentIsLoaded();
    vector<string> vec;
    auto findMember = document.FindMember(key.c_str());
    if(findMember == document.MemberEnd()) {
        return vec;
    }
    const Value& val = document[key.c_str()];
    ASSERT_VALID(val.IsArray());

    for(Value::ConstValueIterator iter = val.Begin();iter != val.End();++iter) {
        vec.emplace_back(iter->GetString());
    }
    return vec;
}

string JSONVectorReader::GetString(string key) {
    documentIsLoaded();
    auto findMember = document.FindMember(key.c_str());
    if(findMember == document.MemberEnd()) {
        return string();
    }
    auto& val = document[key.c_str()];
    ASSERT_VALID(val.IsString());
    return string(val.GetString());
}

void JSONVectorReader::documentIsLoaded() {
    if (!isLoad) {
        throw "No JSON file is loaded";
    }
}


