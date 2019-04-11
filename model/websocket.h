//
// Created by ryan on 18-1-9.
//

#ifndef JUDGE_CLIENT_WEBSOCKET_H
#define JUDGE_CLIENT_WEBSOCKET_H

#include <bits/stdc++.h>
#include "easywsclient.hpp"
#include "../header/json.hpp"

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

	easywsclient::WebSocket::pointer get_pointer();

	websocket &operator<<(const std::string &str);

	websocket &operator<<(const nlohmann::json &json);

private:
	easywsclient::WebSocket::pointer wsconnect;
	bool connected;
	std::string address_;
	int port_;
};


#endif //JUDGE_CLIENT_WEBSOCKET_H
