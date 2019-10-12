#include <utility>
#include "../../library/json.hpp"
//
// Created by Ryan on 2018-12-18.
//
#include <iostream>

using std::cerr;
using std::endl;
using nlohmann::detail::type_error;
#include "Bundle.h"
#include "../../library/judge_lib.h"

bool Bundle::setValue(const string& key, Pack val) {
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

bool Bundle::setJudger(char * str) {
    return setValue("judger", Pack(string(str)));
}

bool Bundle::setJudger(string& str) {
    return setValue("judger", Pack(string(str)));
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
        try {
            switch (i.second.getType()) {
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
            JSON.dump();
        }
        catch(type_error& e) {
            JSON["value"][i.first] = e.what();
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
    _map["total_point"] = Pack(0);
}

Bundle::Bundle() {
    init();
}

Bundle::operator string() {
    return toJSONString();
}

void Bundle::clear() {
    init();
    if (has("wid") && get("wid").isInt()) {
        setValue("wid", Pack(get("wid").setInt(get("wid").getInt() + 1)));
    }
    else {
        setValue("wid", Pack(0));
    }
}

Pack& Bundle::get(const string& key) {
    return this->_map[key];
}

bool Bundle::has(const string& key) {
    return this->_map.find(key) != this->_map.end();
}