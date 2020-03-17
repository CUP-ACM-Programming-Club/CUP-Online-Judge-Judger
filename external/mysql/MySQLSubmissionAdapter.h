//
// Created by Haoyuan Li on 2020/3/18.
//

#ifndef JUDGE_CLIENT_MYSQLSUBMISSIONADAPTER_H
#define JUDGE_CLIENT_MYSQLSUBMISSIONADAPTER_H

#include <string>
class MySQLSubmissionAdapter {
public:
    virtual void getCustomInput(int solutionId, char* work_dir) = 0;
    virtual void getProblemInfo(int p_id, double& time_lmt, int&mem_lmt, int& isspj) = 0;
    virtual void getSolution(int solution_id, char* work_dir, int lang, char*usercode, const char* suffix, int DEBUG) = 0;
    virtual void getSolutionInfo(int solutionId, int& p_id, char* userId, int& lang) = 0;
    virtual MySQLSubmissionAdapter& setPort(int port) = 0;
    virtual MySQLSubmissionAdapter& setDBName(const std::string db) = 0;
    virtual MySQLSubmissionAdapter& setUserName(const std::string& user_name) = 0;
    virtual MySQLSubmissionAdapter& setPassword(const std::string& password) = 0;
    virtual MySQLSubmissionAdapter& setHostName(const std::string host_name) = 0;
    virtual bool isConnected() = 0;
    virtual bool start() = 0;
};

typedef MySQLSubmissionAdapter* createMySQLAdapterInstance();


#endif //JUDGE_CLIENT_MYSQLSUBMISSIONADAPTER_H
