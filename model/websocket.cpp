//
// Created by ryan on 18-1-9.
//

#include "websocket.h"

using namespace std;
using json = nlohmann::json;
using easywsclient::WebSocket;

websocket::websocket() {
    connected = false;
}

websocket::websocket(const string &url) {
    wsconnect = WebSocket::from_url(url);
    connected = true;
}

bool websocket::connect(const string &url) {
    wsconnect = WebSocket::from_url(url);
    connected = true;
    return true;
}

websocket &websocket::emit(const string &str) {
    wsconnect->send(str);
    wsconnect->poll();
    return *this;
}

WebSocket::pointer websocket::get_pointer() {
    return wsconnect;
}

websocket::~websocket() {
    connected = false;
    wsconnect->close();
    delete wsconnect;
}

websocket &websocket::operator<<(const string &str) {
    return emit(str);
}

websocket &websocket::emit(const json &json) {
    return emit(json.dump());
}

websocket &websocket::operator<<(const json &json) {
    if (isConnected())
        return emit(json.dump());
    else
        return *this;
}

bool websocket::isConnected() {
    return connected;
}

websocket &websocket::send(const string &str) {
    return emit(str);
}

websocket &websocket::send(const json &json) {
    return emit(json);
}

bool websocket::close() {
    wsconnect->close();
    connected = false;
    delete wsconnect;
    return !connected;
}

bool websocket::setAddress(const std::string &addr) {
    address_ = addr;
    return true;
}

bool websocket::setPort(const int port) {
    port_= port;
    return true;
}
