//
// Created by eleme on 2019-08-16.
//

#ifndef JUDGE_CLIENT_COMPILERCONFIGREADER_H
#define JUDGE_CLIENT_COMPILERCONFIGREADER_H
#include "../base/JSONVectorReader.h"
#include "../../header/static_var.h"
class CompilerConfigReader {
private:
    JSONVectorReader jsonReader;
    LanguageConstants language;
    string plainJSONText;
public:
    CompilerConfigReader();
    CompilerConfigReader(string);
    CompilerConfigReader(JSONVectorReader);
    vector<string> getCompilerArgs();
    vector<string> getCompilerArgs(LanguageConstants);
    void setLanguage(LanguageConstants);
    LanguageConstants getLanguage();
    void clear();
};


#endif //JUDGE_CLIENT_COMPILERCONFIGREADER_H
