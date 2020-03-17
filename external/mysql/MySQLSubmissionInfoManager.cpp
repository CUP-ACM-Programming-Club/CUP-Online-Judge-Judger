//
// Created by Haoyuan Li on 2020/3/18.
//

#include "MySQLSubmissionInfoManager.h"
const int BUFFER_SIZE = 1 << 20;
void MySQLSubmissionInfoManager::getCustomInput(int solutionId, char *work_dir) {
    char sql[BUFFER_SIZE], src_pth[BUFFER_SIZE];
    // get the source code
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(sql, "SELECT input_text FROM custominput WHERE solution_id=%d",
            solutionId);
    conn.query(conn, sql, strlen(sql));
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    if (row != nullptr) {

        // create the src file
        sprintf(src_pth, "data.in");
        FILE *fp_src = fopen(src_pth, "w");
        fprintf(fp_src, "%s", row[0]);
        fclose(fp_src);

    }
    mysql_free_result(res);
}

void MySQLSubmissionInfoManager::getProblemInfo(int p_id, double &time_lmt, int &mem_lmt, int &isspj) {
    char sql[BUFFER_SIZE];
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(sql,
            "SELECT time_limit,memory_limit,spj FROM problem where problem_id=%d",
            p_id);
    conn.query(conn, sql, strlen(sql));
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    time_lmt = atof(row[0]);
    mem_lmt = atoi(row[1]);
    isspj = (row[2][0] == '1');
    mysql_free_result(res);
    if (time_lmt <= 0)
        time_lmt = 1;
}

void MySQLSubmissionInfoManager::getSolution(int solution_id, char *work_dir, int lang, char *usercode, const char* suffix, int DEBUG) {
    char sql[BUFFER_SIZE], src_pth[BUFFER_SIZE];
    // get the source code
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(sql, "SELECT source FROM source_code WHERE solution_id=%d",
            solution_id);
    conn.query(conn, sql, strlen(sql));
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    sprintf(usercode, "%s", row[0]);
    // create the src file
    sprintf(src_pth, "Main.%s", suffix);
    if (DEBUG) {
        printf("Main=%s", src_pth);
        cout << usercode << endl;
    }
    FILE *fp_src = fopen(src_pth, "we");
    fprintf(fp_src, "%s", row[0]);
    mysql_free_result(res);
    fclose(fp_src);
}

void MySQLSubmissionInfoManager::getSolutionInfo(int solutionId, int &p_id, char *userId, int &lang) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[BUFFER_SIZE];
    // get the problem id and user id from Table:solution
    sprintf(sql,
            "SELECT problem_id, user_id, language FROM solution where solution_id=%d",
            solutionId);
    //printf("%s\n",sql);
    conn.query(conn, sql, strlen(sql));
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    p_id = atoi(row[0]);
    strcpy(userId, row[1]);
    lang = atoi(row[2]);
    mysql_free_result(res);
}

MySQLSubmissionAdapter &MySQLSubmissionInfoManager::setPort(int port) {
    conn.setPort(port);
    return *this;
}

MySQLSubmissionAdapter &MySQLSubmissionInfoManager::setDBName(const std::string db) {
    conn.setDBName(db);
    return *this;
}

MySQLSubmissionAdapter &MySQLSubmissionInfoManager::setUserName(const std::string &user_name) {
    conn.setUserName(user_name);
    return *this;
}

MySQLSubmissionAdapter &MySQLSubmissionInfoManager::setPassword(const std::string &password) {
    conn.setPassword(password);
    return *this;
}

MySQLSubmissionAdapter &MySQLSubmissionInfoManager::setHostName(const std::string host_name) {
    conn.setHostName(host_name);
    return *this;
}

bool MySQLSubmissionInfoManager::isConnected() {
    return conn.isConnected();
}

bool MySQLSubmissionInfoManager::start() {
    return conn.start();
}

extern "C" MySQLSubmissionAdapter* createInstance() {
    return new MySQLSubmissionInfoManager;
}