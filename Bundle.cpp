#include <utility>

//
// Created by Ryan on 2018-12-18.
//
#include <iostream>

using std::cerr;
using std::endl;

#include "Bundle.h"
#include "static_var.h"

bool Bundle::setValue(string key, Pack val) {
    try {
        this->_map[key] = std::move(val);
    }
    catch (char *e) {
        cerr << "Bundle::setValue failed." << endl;
        return false;
    }
    return true;
}

bool Bundle::setSolutionID(int solution_id) {
    return setValue("solution_id", Pack(solution_id));
}

bool Bundle::setResult(int state) {
    return setValue("state", Pack(state));
}

bool Bundle::setFinished(int finished) {
    return setValue("finish", Pack(finished));
}

bool Bundle::setUsedTime(double time) {
    return setValue("time", Pack(time));
}

bool Bundle::setMemoryUse(int memory) {
    return setValue("memory", Pack(memory));
}

bool Bundle::setPassPoint(int pass_point) {
    return setValue("pass_point", Pack(pass_point));
}

bool Bundle::setPassRate(double pass_rate) {
    return setValue("pass_rate", Pack(pass_rate));
}


bool Bundle::setTestRunResult(string &test_run_result) {
    return setValue("test_run_result", Pack(checkUTF8Valid(test_run_result)));
}

bool Bundle::setCompileInfo(string &compile_info) {
    return setValue("compile_info", Pack(checkUTF8Valid(compile_info)));
}

bool Bundle::setSim(int sim) {
    return setValue("sim", Pack(sim));
}

bool Bundle::setSimSource(int sim_s_id) {
    return setValue("sim_s_id", Pack(sim_s_id));
}

bool Bundle::setTotalPoint(int total_point) {
    return setValue("total_point", Pack(total_point));
}

string Bundle::trim(string &str) {
    if (str.length() > CODE_LENGTH_LIMIT) {
        auto diff_len = str.length() - CODE_LENGTH_LIMIT;
        string copy_str = str.substr(0, CODE_LENGTH_LIMIT);
        copy_str += "\n...\nOmit" + to_string(diff_len) + " character" + (diff_len > 1 ? "s" : "");
        return copy_str;
    } else {
        return str;
    }
}

string Bundle::checkUTF8Valid(string &str) {
    string copy_str(str);
    if (utf8_check_is_valid(copy_str)) {
        return trim(copy_str);
    } else {
        return "检测到非法UTF-8输出";
    }
}

string Bundle::toJSONString() {
    json JSON;
    JSON["type"] = "judger";
    for(auto i:_map) {
        switch(i.second.getType()) {
            case INT:
                JSON["value"][i.first] = i.second.getInt();
                break;
            case DOUBLE:
                JSON["value"][i.first] = i.second.getFloat();
                break;
            case STRING:
                JSON["value"][i.first] = i.second.getString();
                break;
            default:
                JSON["value"][i.first] = false;
        }
    }
    return JSON.dump();
}

void Bundle::init() {
    _map["solution_id"] = Pack();
    _map["state"] = Pack();
    _map["finish"] = Pack();
    _map["time"] = Pack();
    _map["memory"] = Pack();
    _map["pass_point"] = Pack();
    _map["pass_rate"] = Pack();
    _map["test_run_result"] = Pack("");
    _map["compile_info"] = Pack("");
    _map["sim"] = Pack();
    _map["sim_s_id"] = Pack();
    _map["total_point"] = Pack();
}

Bundle::Bundle() {
    init();
}

Bundle::operator string() {
    return toJSONString();
}

void Bundle::clear() {
    init();
}

