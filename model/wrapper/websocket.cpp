//
// Created by ryan on 18-1-9.
//

#include "websocket.h"
#define LOCK(lock) std::lock_guard<std::mutex> lockGuard(lock)
using namespace std;
using json = nlohmann::json;
using easywsclient::WebSocket;
ThreadPool threadPool_(std::thread::hardware_concurrency());

websocket::websocket(){
    connected = false;
}

websocket::websocket(const string &url){
    LOCK(lock);
    initQueue(url);
    connected = true;
}

bool websocket::connect(const string &url) {
    LOCK(lock);
    address_ = url;
    if (connected) {
        return false;
    }
    initQueue(url);
    connected = true;
    return true;
}

websocket &websocket::emit(const string &str) {
    LOCK(lock);
    taskQueue_.emplace_back(threadPool_.enqueue([this, str]{
        auto wsconnect = this->getConnection();
        wsconnect->send(str);
        wsconnect->poll();
        this->recycleConnection(wsconnect);
        return true;
    }));
    return *this;
}


websocket::~websocket() {
    LOCK(lock);
    drain();
    connected = false;
    while(!wsconnectQueue.empty()) {
        auto connection = wsconnectQueue.front();
        wsconnectQueue.pop_front();
        delete connection;
    }
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
    LOCK(lock);
    wsconnect->close();
    connected = false;
    delete wsconnect;
    return !connected;
}

bool websocket::setAddress(const std::string &addr) {
    LOCK(lock);
    address_ = addr;
    return true;
}

bool websocket::setPort(const int port) {
    LOCK(lock);
    port_= port;
    return true;
}

void websocket::drain() {
    reverse(taskQueue_.begin(), taskQueue_.end());
    while(!taskQueue_.empty()) {
        taskQueue_.back().get();
        taskQueue_.pop_back();
    }
}

void websocket::initQueue(const string& url) {
    for(int i = 0, num = std::thread::hardware_concurrency(); i < num; ++i) {
        wsconnectQueue.push_back(WebSocket::from_url(url));
    }
}

easywsclient::WebSocket::pointer websocket::getConnection() {
    LOCK(queueLock);
    auto pointer = wsconnectQueue.front();
    wsconnectQueue.pop_front();
    return pointer;
}

void websocket::recycleConnection(easywsclient::WebSocket::pointer pointer) {
    LOCK(queueLock);
    wsconnectQueue.push_back(pointer);
}
