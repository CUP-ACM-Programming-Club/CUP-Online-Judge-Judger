//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_LANGUAGE_H
#define JUDGE_CLIENT_LANGUAGE_H
#define extlang extern "C" Language*
#define deslang extern "C" void
#include <vector>
#include <string>
class Language {
public:
    virtual void run(int memory) = 0;
    virtual void setProcessLimit();
    virtual void setCompileProcessLimit();
    virtual void compile(std::vector<std::string>&, const char*, const char*);
protected:
    virtual void setCPULimit();
    virtual void setFSizeLimit();
    virtual void setASLimit();
    virtual void setAlarm();
};

typedef Language* createLanguageInstance();

typedef void destroyLanguageInstance(Language*);


#endif //JUDGE_CLIENT_LANGUAGE_H
