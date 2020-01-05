//
// Created by Ryan on 2020-01-06.
//

#include "InitManager.h"
#include "../../header/static_var.h"
#include "../../header/okcalls.h"
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
    } else if (isCOrCPP(lang)) { // C & C++
        for (i = 0; i == 0 || LANG_CV[i]; i++) {
            call_counter[LANG_CV[i]] = HOJ_MAX_LIMIT;
        }
    } else if (lang == PASCAL) { // Pascal
        for (i = 0; i == 0 || LANG_PV[i]; i++)
            call_counter[LANG_PV[i]] = HOJ_MAX_LIMIT;
    } else if (isJava(lang)) { // Java
        for (i = 0; i == 0 || LANG_JV[i]; i++)
            call_counter[LANG_JV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == RUBY) { // Ruby
        for (i = 0; i == 0 || LANG_RV[i]; i++)
            call_counter[LANG_RV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == BASH) { // Bash
        for (i = 0; i == 0 || LANG_BV[i]; i++)
            call_counter[LANG_BV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PYTHON2) { // Python
        for (i = 0; i == 0 || LANG_YV[i]; i++)
            call_counter[LANG_YV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PYTHON3) {
        for (i = 0; i == 0 || LANG_PY3V[i]; i++)
            call_counter[LANG_PY3V[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PHP) { // php
        for (i = 0; i == 0 || LANG_PHV[i]; i++)
            call_counter[LANG_PHV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PERL) { // perl
        for (i = 0; i == 0 || LANG_PLV[i]; i++)
            call_counter[LANG_PLV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == CSHARP) { // mono c#
        for (i = 0; i == 0 || LANG_CSV[i]; i++)
            call_counter[LANG_CSV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == OBJC) { //objective c
        for (i = 0; i == 0 || LANG_OV[i]; i++)
            call_counter[LANG_OV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == FREEBASIC) { //free basic
        for (i = 0; i == 0 || LANG_BASICV[i]; i++)
            call_counter[LANG_BASICV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == SCHEMA) { //scheme guile
        for (i = 0; i == 0 || LANG_SV[i]; i++)
            call_counter[LANG_SV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == LUA) { //lua
        for (i = 0; i == 0 || LANG_LUAV[i]; i++)
            call_counter[LANG_LUAV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == JAVASCRIPT) { //nodejs
        for (i = 0; i == 0 || LANG_JSV[i]; i++)
            call_counter[LANG_JSV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == L_GO) { //go
        for (i = 0; i == 0 || LANG_GOV[i]; i++)
            call_counter[LANG_GOV[i]] = HOJ_MAX_LIMIT;
    } else if (lang == PYPY) {
        for (i = 0; i == 0 || LANG_PYPYV[i]; ++i) {
            call_counter[LANG_PYPYV[i]] = HOJ_MAX_LIMIT;
        }
    } else if (lang == PYPY3) {
        for (i = 0; i == 0 || LANG_PYPY3V[i]; ++i) {
            call_counter[LANG_PYPY3V[i]] = HOJ_MAX_LIMIT;
        }
    }
}
