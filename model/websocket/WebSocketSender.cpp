//
// Created by ryan on 18-1-9.
//

#include "WebSocketSender.h"
#include <algorithm>
#define LOCK(lock) std::lock_guard<std::mutex> lockGuard(lock)
using namespace std;
using easywsclient::WebSocket;

WebSocketSender::WebSocketSender() {
    connected = false;
}

WebSocketSender::WebSocketSender(const string &url) {
    LOCK(lock);
    initQueue(url);
    connected = true;
}

SocketSender& WebSocketSender::connect(const string &url) {
    LOCK(lock);
    address_ = url;
    if (connected) {
        return *this;
    }
    initQueue(url);
    connected = true;
    return *this;
}

SocketSender &WebSocketSender::emit(const string &str) {
    LOCK(lock);
    taskQueue_.emplace_back(threadPool_.enqueue([this, str] {
        auto wsconnect = this->getConnection();
        wsconnect->send(str);
        wsconnect->poll();
        this->recycleConnection(wsconnect);
        return true;
    }));
    return *this;
}


WebSocketSender::~WebSocketSender() {
    LOCK(lock);
    drain();
    connected = false;
    while (!wsconnectQueue.empty()) {
        auto connection = wsconnectQueue.front();
        wsconnectQueue.pop_front();
        delete connection;
    }
}

SocketSender &WebSocketSender::operator<<(const string &str) {
    return emit(str);
}

bool WebSocketSender::isConnected() {
    return connected;
}

SocketSender &WebSocketSender::send(const string &str) {
    return emit(str);
}

SocketSender &WebSocketSender::close() {
    LOCK(lock);
    wsconnect->close();
    connected = false;
    delete wsconnect;
    return *this;
}

SocketSender &WebSocketSender::setAddress(const std::string &addr) {
    LOCK(lock);
    address_ = addr;
    return *this;
}

SocketSender &WebSocketSender::setAddressAndPort(const std::string &addr) {
    LOCK(lock);
    const auto pos = addr.find(':');
    if (pos == std::string::npos) {
        exit(1);
    }
    string address = addr.substr(0, pos);
    string port = addr.substr(pos + 1);
    address_ = std::move(address);
    port_ = stoi(port);
    return *this;
}

SocketSender &WebSocketSender::setPort(const int port) {
    LOCK(lock);
    port_ = port;
    return *this;
}

void WebSocketSender::drain() {
    reverse(taskQueue_.begin(), taskQueue_.end());
    while (!taskQueue_.empty()) {
        taskQueue_.back().get();
        taskQueue_.pop_back();
    }
}

void WebSocketSender::initQueue(const string &url) {
    for (int i = 0, num = std::thread::hardware_concurrency(); i < num; ++i) {
        wsconnectQueue.push_back(WebSocket::from_url(url));
    }
}

easywsclient::WebSocket::pointer WebSocketSender::getConnection() {
    LOCK(queueLock);
    auto pointer = wsconnectQueue.front();
    wsconnectQueue.pop_front();
    return pointer;
}

void WebSocketSender::recycleConnection(easywsclient::WebSocket::pointer pointer) {
    LOCK(queueLock);
    wsconnectQueue.push_back(pointer);
}
