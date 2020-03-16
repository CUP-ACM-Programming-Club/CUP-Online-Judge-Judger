//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_BONUSLIMIT_H
#define JUDGE_LIBRARY_BONUSLIMIT_H


class BonusLimit {
protected:
    double buildBonusTimeLimit(double timeLimit, double bonus);
    int buildBonusMemoryLimit(int memoryLimit, int bonus);
};


#endif //JUDGE_LIBRARY_BONUSLIMIT_H
