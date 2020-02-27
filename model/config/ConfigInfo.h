//
// Created by Haoyuan Li on 2020/1/14.
//

#ifndef JUDGE_CLIENT_CONFIGINFO_H
#define JUDGE_CLIENT_CONFIGINFO_H

#include <string>
using std::string;

class ConfigInfo {
private:
    string hostname;
    string username;
    string password;
    string dbName;
    int port;
    int javaTimebonus;
    int javaMemoryBonus;
    int simEnable;
    string java_xms;
    string java_xmx;
    string http_judge;
    string httpUserName;
    string httpPassword;
    int allTestMode;
    int fullDiff;
    int shareMemoryRun;
    int useMaxTime;
    int usePtrace;
public:
    ConfigInfo& setHostname(string& hostname);
    string getHostname();
    ConfigInfo& setUserName(string& username);
    string getUserName();
    ConfigInfo& setPassword(string& password);
    string getPassword();
    ConfigInfo& setDbName(string& dbName);
    string getDbName();
    ConfigInfo& setPort(int port);
    int getPort();
    ConfigInfo& setJavaTimeBonus(int javaTimebonus);
    int getJavaTimeBonus();
    ConfigInfo& setJavaMemoryBonus(int javaMemoryBonus);
    int getJavaMemoryBonus();
    ConfigInfo& setSimEnable(int simEnable);
    int getSimEnable();
    ConfigInfo& setJavaXms(string& java_xms);
    string getJavaXms();
    ConfigInfo& setJavaXmx(string& java_xmx);
    string getJavaXmx();
    ConfigInfo& setHttpJudge(string& httpJudge);
    string getHttpJudge();
    ConfigInfo& setHttpUserName(string& httpUserName);
    string getHttpUserName();
    ConfigInfo& setHttpPassword(string& httpPassword);
    string getHttpPassword();
    ConfigInfo& setAllTestMode(int allTestMode);
    int getAllTestMode();
    ConfigInfo& setFullDiff(int fullDiff);
    int getFullDiff();
    ConfigInfo& setShareMemoryRun(int shareMemoryRun);
    int getShareMemoryRun();
    ConfigInfo& setUseMaxTime(int useMaxTime);
    int getUseMaxTime();
    ConfigInfo& setUsePtrace(int usePtrace);
    int getUsePtrace();

    ConfigInfo& readFromFile(string&);
};


#endif //JUDGE_CLIENT_CONFIGINFO_H
