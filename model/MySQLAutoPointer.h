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
    int port_number;
    string host_name,user_name,password, db_name;
public:
    MySQLAutoPointer() = default;
    ~MySQLAutoPointer();
    operator MYSQL*();
    bool start();
    bool setHostName(string h);
    bool setUserName(string u);
    bool setPassword(string p);
    bool setDBName(string db);
    bool setPort(int p);
    bool isConnected();
    bool setDebugMode(bool state);
};


#endif //JUDGE_CLIENT_MYSQLAUTOPOINTER_H
