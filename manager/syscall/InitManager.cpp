//
// Created by Ryan on 2020-01-06.
//

#include "InitManager.h"
#include "../../header/static_var.h"
#include <cstring>
using std::memset;

void InitManager::initSyscallLimits(int lang, int *call_counter, int record_call, int call_array_size) {
    int i;
    memset(call_counter, 0, sizeof(call_counter));
    if (DEBUG)
        write_log(oj_home, "init_call_counter:%d", lang);
    if (record_call) { // C & C++
        for (i = 0; i < call_array_size; i++) {
            call_counter[i] = 0;
        }
    } else {
        shared_ptr<Language> languageModel(getLanguageModel(lang));
        languageModel->initCallCounter(call_counter);
    }
}
