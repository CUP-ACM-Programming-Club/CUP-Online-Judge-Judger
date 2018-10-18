//
// Created by Ryan on 2018/10/18.
//

#ifndef CPP_COMPILERARGSREADER_H
#define CPP_COMPILERARGSREADER_H

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <vector>
#include <iostream>
#include <string>
using std::cout;
using std::string;
using std::vector;
using rapidjson::Document;

class CompilerArgsReader {
public:
    CompilerArgsReader(const string &filePath);

    CompilerArgsReader() : isLoad(false) {}

    ~CompilerArgsReader() = default;

    bool loadFile(const string &filePath);

    bool loadJSON(string jsonPlainText);

    vector<string> Get(string key);

private:
    bool isLoad;
    Document document;
};


#endif //CPP_COMPILERARGSREADER_H
