//
// Created by Haoyuan Li on 2020/3/26.
//

#include "Compare.h"

int Compare::getDebug() {
    return DEBUG;
}

void Compare::setDebug(int debug) {
    DEBUG = debug;
}

void Compare::setFullDiff(int fullDiff) {
    full_diff = fullDiff;
}

int Compare::getFullDiff() {
    return full_diff;
}
