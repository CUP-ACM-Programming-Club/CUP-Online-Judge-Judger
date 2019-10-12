//
// Created by ryan on 18-1-9.
//

#ifndef JUDGE_CLIENT_WEBSOCKET_H
#define JUDGE_CLIENT_WEBSOCKET_H

#include <bits/stdc++.h>
#include <mutex>
#include <thread>
#include "../../library/easywsclient.hpp"
#include "../../library/json.hpp"
#include "../base/ThreadPool.h"
#include "../../library/judge_lib.h"

class websocket
{
public:
	websocket();

	explicit websocket(const std::string &url);

	~websocket();

	bool connect(const std::string &url);

	bool setAddress(const std::string &addr);

	bool setPort(const int port);

	bool setAddressAndPort(const std::string &addr);

	bool close();

	bool isConnected();

	websocket &emit(const std::string &str);

	websocket &emit(const nlohmann::json &json);

	websocket &send(const std::string &str);

	websocket &send(const nlohmann::json &json);

	websocket &operator<<(const std::string &str);

	websocket &operator<<(const nlohmann::json &json);

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


#endif //JUDGE_CLIENT_WEBSOCKET_H
