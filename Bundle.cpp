//
// Created by Ryan on 2018-12-18.
//
#include <iostream>
using std::cerr;
using std::endl;
#include "Bundle.h"

bool Bundle::setSolutionID(int solution_id) {
    return setValue("solution_id", to_string(solution_id));
}

bool Bundle::setResult(int state) {
    return setValue("state", to_string(state));
}

bool Bundle::setFinished(int finished) {
    return setValue("finish", to_string(finished));
}

bool Bundle::setUsedTime(double time) {
    return setValue("time", to_string(time));
}

bool Bundle::setMemoryUse(int memory) {
    return setValue("memory", to_string(memory));
}

bool Bundle::setPassPoint(int pass_point) {
    return setValue("pass_point", to_string(pass_point));
}



template<class T>
bool Bundle::setValue(const string key,const string val) {
    try {
        this->_map[key] = val;
    }
    catch (char* e) {
        cerr << "Bundle::setValue failed." << endl;
        return false;
    }
    return true;
}

