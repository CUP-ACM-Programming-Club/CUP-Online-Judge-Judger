//
// Created by Haoyuan Li on 2020/3/14.
//

#include "getLanguageModel.h"
#include "../../../../header/static_var.h"
#include <dlfcn.h>
Language* getLanguageModel::getModel(int language) {
    string languageName = this->getLanguageName(language);
    void* languageHandler = dlopen((languageName + ".so").c_str(), RTLD_LAZY);
    if (!languageHandler) {
        cerr << "Cannot load library: " << dlerror() << endl;
        exit(1);
    }
    dlerror();
    createLanguageInstance* createInstance = (createLanguageInstance*) dlsym(languageHandler, (string("createInstance") + languageName).c_str());
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << endl;
        exit(1);
    }
    destroyLanguageInstance* destroyInstance = (destroyLanguageInstance*) dlsym(languageHandler, (string("destroyInstance") + languageName).c_str());
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << endl;
        exit(1);
    }

    Language* languageInstance = createInstance();
    return languageInstance;
}

std::string getLanguageModel::getLanguageName(int language) {
    switch (language) {
        case L_C11:
            return "c11";
        case L_CPP17:
            return "c17";
        case PASCAL:
            return "pascal";
        case JAVA:
            return "java";
        case RUBY:
            return "ruby";
        case BASH:
            return "bash";
        case PYTHON2:
            return "python2";
        case PHP:
            return "php";
        case PERL:
            return "perl";
        case CSHARP:
            return "csharp";
        case OBJC:
            return "objectivec";
        case FREEBASIC:
            return "freebasic";
        case SCHEMA:
            return "schema";
        case CLANG:
            return "clang";
        case CLANGPP:
            return "clangpp";
        case LUA:
            return "lua";
        case JAVASCRIPT:
            return "javascript";
        case L_GO:
            return "go";
        case PYTHON3:
            return "python3";
        case CPP11:
            return "cpp11";
        case CPP98:
            return "cpp98";
        case L_C99:
            return "c99";
        case KOTLIN:
            break;
        case JAVA8:
            return "java8";
        case JAVA7:
            return "java7";
        case PYPY:
            return "pypy";
        case PYPY3:
            return "pypy3";
        case JAVA6:
            return "java6";
        case CLANG11:
            return "clang11";
        case CLANGPP17:
            return "clangpp17";
        default:
            break;
    }
}
