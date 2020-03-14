//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_LANGUAGE_H
#define JUDGE_CLIENT_LANGUAGE_H
#define extlang extern "C" Language*
#define deslang extern "C" void
class Language {
public:
    virtual void run(int memory) = 0;
    virtual void setProcessLimit();
};

typedef Language* createLanguageInstance();

typedef void destroyLanguageInstance(Language*);


#endif //JUDGE_CLIENT_LANGUAGE_H
