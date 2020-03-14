//
// Created by Haoyuan Li on 2020/3/14.
//

#ifndef JUDGE_CLIENT_GETLANGUAGEMODEL_H
#define JUDGE_CLIENT_GETLANGUAGEMODEL_H
#include <dlfcn.h>
#include "../Language.h"
#include <string>
class getLanguageModel {
public:
    Language* getModel(int);
private:
    std::string getLanguageName(int);
};


#endif //JUDGE_CLIENT_GETLANGUAGEMODEL_H
