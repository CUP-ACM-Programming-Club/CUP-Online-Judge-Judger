//
// Created by ryan on 18-1-9.
//

#ifndef JUDGE_CLIENT_WEBSOCKET_H
#define JUDGE_CLIENT_WEBSOCKET_H

#include <bits/stdc++.h>
#include "easywsclient.hpp"
#include "json.hpp"

class websocket
{
public:
	websocket();

	explicit websocket(const std::string &url);

	~websocket();

	bool connect(const std::string &url);

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
};


#endif //JUDGE_CLIENT_WEBSOCKET_H
