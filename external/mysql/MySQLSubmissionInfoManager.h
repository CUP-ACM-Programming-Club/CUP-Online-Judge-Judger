//
// Created by Haoyuan Li on 2020/3/18.
//

#ifndef JUDGE_MYSQL_MYSQLSUBMISSIONINFOMANAGER_H
#define JUDGE_MYSQL_MYSQLSUBMISSIONINFOMANAGER_H


#include "MySQLSubmissionAdapter.h"
#include "MySQLAutoPointer.h"

class MySQLSubmissionInfoManager: public MySQLSubmissionAdapter {
public:
    void getCustomInput(int solutionId, char *work_dir) override;

    void getProblemInfo(int p_id, double &time_lmt, int &mem_lmt, int &isspj) override;

    void getSolution(int solution_id, char *work_dir, int lang, char *usercode, const char* suffix, int DEBUG) override;

    void getSolutionInfo(int solutionId, int &p_id, char *userId, int &lang) override;

    MySQLSubmissionAdapter &setPort(int port) override;

    MySQLSubmissionAdapter &setDBName(const std::string db) override;

    MySQLSubmissionAdapter &setUserName(const std::string &user_name) override;

    MySQLSubmissionAdapter &setPassword(const std::string &password) override;

    MySQLSubmissionAdapter &setHostName(const std::string host_name) override;

    bool isConnected() override;

    bool start() override;
private:
    MySQLAutoPointer conn;
};


#endif //JUDGE_MYSQL_MYSQLSUBMISSIONINFOMANAGER_H
