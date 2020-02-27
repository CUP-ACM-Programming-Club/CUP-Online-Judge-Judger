//
// Created by Haoyuan Li on 2020/1/14.
//

#include "ConfigInfo.h"
#include "../base/JSONVectorReader.h"

ConfigInfo &ConfigInfo::readFromFile(string &file) {
    JSONVectorReader reader(file);
    this->hostname = reader.GetString("hostname");
    this->username = reader.GetString("username");
    this->password = reader.GetString("password");
    this->dbName = reader.GetString("dbName");
    this->port =  reader.GetInt("port");
    this->javaTimebonus = reader.GetInt("java_time_bonus");
    this->javaMemoryBonus = reader.GetInt("java_memory_bonus");
    this->simEnable = reader.GetInt("sim_enable");
    this->java_xms = reader.GetString("java_xms");
    this->java_xmx = reader.GetString("java_xmx");
    this->http_judge = reader.GetString("http_judge");
    this->httpUserName = reader.GetString("http_username");
    this->httpPassword = reader.GetString("http_password");
    this->allTestMode = reader.GetInt("all_test_mode");
    this->fullDiff = reader.GetInt("full_diff");
    this->shareMemoryRun = reader.GetInt("share_memory_run");
    this->useMaxTime = reader.GetInt("use_max_time");
    this->usePtrace = reader.GetInt("use_ptrace");
    return *this;
}

ConfigInfo &ConfigInfo::setHostname(string &hostname) {
    this->hostname = hostname;
    return *this;
}

string ConfigInfo::getHostname() {
    return hostname;
}

ConfigInfo &ConfigInfo::setUserName(string &username) {
    this->username = username;
    return  *this;
}

string ConfigInfo::getUserName() {
    return username;
}

ConfigInfo &ConfigInfo::setPassword(string &password) {
    this->password = password;
    return *this;
}

string ConfigInfo::getPassword() {
    return password;
}

ConfigInfo &ConfigInfo::setDbName(string &dbName) {
    this->dbName = dbName;
    return *this;
}

string ConfigInfo::getDbName() {
    return dbName;
}

ConfigInfo &ConfigInfo::setPort(int port) {
    this->port = port;
    return *this;
}

int ConfigInfo::getPort() {
    return port;
}

ConfigInfo &ConfigInfo::setJavaTimeBonus(int javaTimebonus) {
    this->javaTimebonus = javaTimebonus;
    return *this;
}

int ConfigInfo::getJavaTimeBonus() {
    return javaTimebonus;
}

ConfigInfo &ConfigInfo::setJavaMemoryBonus(int javaMemoryBonus) {
    this->javaMemoryBonus = javaMemoryBonus;
    return *this;
}

int ConfigInfo::getJavaMemoryBonus() {
    return javaMemoryBonus;
}

ConfigInfo &ConfigInfo::setSimEnable(int simEnable) {
    this->simEnable = simEnable;
    return *this;
}

int ConfigInfo::getSimEnable() {
    return simEnable;
}

ConfigInfo &ConfigInfo::setJavaXms(string &java_xms) {
    this->java_xms = java_xms;
    return *this;
}

string ConfigInfo::getJavaXms() {
    return java_xms;
}

ConfigInfo &ConfigInfo::setJavaXmx(string &java_xmx) {
    this->java_xmx = java_xmx;
    return *this;
}

string ConfigInfo::getJavaXmx() {
    return java_xmx;
}

ConfigInfo &ConfigInfo::setHttpJudge(string &httpJudge) {
    this->http_judge = httpJudge;
    return *this;
}

string ConfigInfo::getHttpJudge() {
    return http_judge;
}

ConfigInfo &ConfigInfo::setHttpUserName(string &httpUserName) {
    this->httpUserName = httpUserName;
    return  *this;
}

string ConfigInfo::getHttpUserName() {
    return httpUserName;
}

ConfigInfo &ConfigInfo::setHttpPassword(string &httpPassword) {
    this->httpPassword = httpPassword;
    return *this;
}

string ConfigInfo::getHttpPassword() {
    return this->httpPassword;
}

ConfigInfo &ConfigInfo::setAllTestMode(int allTestMode) {
    this->allTestMode =  allTestMode;
    return *this;
}

int ConfigInfo::getAllTestMode() {
    return allTestMode;
}

ConfigInfo &ConfigInfo::setFullDiff(int fullDiff) {
    this->fullDiff = fullDiff;
    return *this;
}

int ConfigInfo::getFullDiff() {
    return fullDiff;
}

ConfigInfo &ConfigInfo::setShareMemoryRun(int shareMemoryRun) {
    this->shareMemoryRun = shareMemoryRun;
    return *this;
}

int ConfigInfo::getShareMemoryRun() {
    return shareMemoryRun;
}

ConfigInfo &ConfigInfo::setUseMaxTime(int useMaxTime) {
    this->useMaxTime = useMaxTime;
    return *this;
}

int ConfigInfo::getUseMaxTime() {
    return useMaxTime;
}

ConfigInfo &ConfigInfo::setUsePtrace(int usePtrace) {
    this->usePtrace = usePtrace;
    return *this;
}

int ConfigInfo::getUsePtrace() {
    return usePtrace;
}


