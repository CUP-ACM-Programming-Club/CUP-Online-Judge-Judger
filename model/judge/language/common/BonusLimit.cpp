//
// Created by Haoyuan Li on 2020/3/16.
//

#include "BonusLimit.h"

double BonusLimit::buildBonusTimeLimit(double timeLimit, double bonus) {
    return timeLimit * bonus + bonus;
}

int BonusLimit::buildBonusMemoryLimit(int memoryLimit, int bonus) {
    return memoryLimit + bonus;
}
