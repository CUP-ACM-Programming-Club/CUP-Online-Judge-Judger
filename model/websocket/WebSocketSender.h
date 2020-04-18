//
// Created by ryan on 18-1-9.
//

#ifndef JUDGE_CLIENT_WEBSOCKETSENDER_H
#define JUDGE_CLIENT_WEBSOCKETSENDER_H

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include "../../library/easywsclient.hpp"
#include "../base/ThreadPool.h"
#include "../../library/judge_lib.h"

class WebSocketSender
{
public:
	WebSocketSender();

	explicit WebSocketSender(const std::string &url);

	~WebSocketSender();

	bool connect(const std::string &url);

	bool setAddress(const std::string &addr);

	bool setPort(const int port);

	bool setAddressAndPort(const std::string &addr);

	bool close();

	bool isConnected();

	WebSocketSender &emit(const std::string &str);


	WebSocketSender &send(const std::string &str);

	WebSocketSender &operator<<(const std::string &str);
private:
	easywsclient::WebSocket::pointer wsconnect;
	bool connected;
	std::mutex lock;
	std::string address_;
	int port_;
    vector<std::future<bool>>taskQueue_;
    deque<easywsclient::WebSocket::pointer> wsconnectQueue;
    std::mutex queueLock;
    void drain();
    void initQueue(const string& url);
    easywsclient::WebSocket::pointer getConnection();
    void recycleConnection(easywsclient::WebSocket::pointer pointer);
};


#endif //JUDGE_CLIENT_WEBSOCKETSENDER_H
