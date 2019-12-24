//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_LANGUAGE_H
#define JUDGE_CLIENT_LANGUAGE_H

class Language {
public:
    virtual void run(int memory) = 0;
    virtual void setProcessLimit();
};


#endif //JUDGE_CLIENT_LANGUAGE_H
