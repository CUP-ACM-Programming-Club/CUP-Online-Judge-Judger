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
#include "../base/SocketSender.h"

class WebSocketSender : public SocketSender
{
public:
	WebSocketSender();

	explicit WebSocketSender(const std::string &url);

	~WebSocketSender();

	SocketSender& connect(const std::string &url) override;

    SocketSender& setAddress(const std::string &addr) override;

    SocketSender& setPort(const int port) override;

    SocketSender& setAddressAndPort(const std::string &addr) override;

    SocketSender& close() override;

	bool isConnected() override;

    SocketSender &emit(const std::string &str) override;


    SocketSender &send(const std::string &str) override;

    SocketSender &operator<<(const std::string &str) override;
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
    ThreadPool threadPool_ = ThreadPool(std::thread::hardware_concurrency());
};


#endif //JUDGE_CLIENT_WEBSOCKETSENDER_H
