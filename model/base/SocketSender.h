//
// Created by lhybh on 2020/8/3.
//

#ifndef JUDGE_CLIENT_SOCKETSENDER_H
#define JUDGE_CLIENT_SOCKETSENDER_H

#include <string>

class SocketSender {
public:
    virtual SocketSender &connect(const std::string &url) = 0;

    virtual SocketSender &setAddress(const std::string &addr) = 0;

    virtual SocketSender &setPort(const int port) = 0;

    virtual SocketSender &setAddressAndPort(const std::string &addr) = 0;

    virtual SocketSender &close() = 0;

    virtual bool isConnected() = 0;

    virtual SocketSender &emit(const std::string& str) = 0;

    virtual SocketSender &send(const std::string &str) = 0;

    virtual SocketSender &operator<<(const std::string &str) = 0;
};


#endif //JUDGE_CLIENT_SOCKETSENDER_H
