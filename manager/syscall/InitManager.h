//
// Created by Ryan on 2020-01-06.
//

#ifndef JUDGE_CLIENT_INITMANAGER_H
#define JUDGE_CLIENT_INITMANAGER_H


class InitManager {
public:
    static void initSyscallLimits(int lang, int* call_counter, int record_call, int call_array_size);
};


#endif //JUDGE_CLIENT_INITMANAGER_H
