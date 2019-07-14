//
// Created by ryan on 19-4-11.
//

#ifndef JUDGE_CLIENT_MYSQLAUTOPOINTER_H
#define JUDGE_CLIENT_MYSQLAUTOPOINTER_H

#include <iostream>
#include <cstring>
using namespace std;
#include <mysql/mysql.h>

class MySQLAutoPointer {
private:
    MYSQL *conn = nullptr;
    bool connected = false;
    bool closed = false;
    bool DEBUG = false;
    int port_number{};
    bool _start = false;
    string host_name,user_name,password, db_name;
    void checkIfNotStart();
public:
    MySQLAutoPointer() = default;
    ~MySQLAutoPointer();
    operator MYSQL*();
    bool start();
    bool setHostName(const string& h);
    bool setUserName(const string& u);
    bool setPassword(const string& p);
    bool setDBName(const string& db);
    bool setPort(int p);
    bool isConnected();
    bool setDebugMode(bool state);
    bool query(MYSQL* pointer, const string& sql, unsigned len);
    bool query(const string& sql);
    bool query(string sql, unsigned len);
};


#endif //JUDGE_CLIENT_MYSQLAUTOPOINTER_H
