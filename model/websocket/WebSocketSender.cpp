//
// Created by ryan on 18-1-9.
//

#include "WebSocketSender.h"
#define LOCK(lock) std::lock_guard<std::mutex> lockGuard(lock)
using namespace std;
using json = nlohmann::json;
using easywsclient::WebSocket;
ThreadPool threadPool_(std::thread::hardware_concurrency());

WebSocketSender::WebSocketSender(){
    connected = false;
}

WebSocketSender::WebSocketSender(const string &url){
    LOCK(lock);
    initQueue(url);
    connected = true;
}

bool WebSocketSender::connect(const string &url) {
    LOCK(lock);
    address_ = url;
    if (connected) {
        return false;
    }
    initQueue(url);
    connected = true;
    return true;
}

WebSocketSender &WebSocketSender::emit(const string &str) {
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


WebSocketSender::~WebSocketSender() {
    LOCK(lock);
    drain();
    connected = false;
    while(!wsconnectQueue.empty()) {
        auto connection = wsconnectQueue.front();
        wsconnectQueue.pop_front();
        delete connection;
    }
}

WebSocketSender &WebSocketSender::operator<<(const string &str) {
    return emit(str);
}

WebSocketSender &WebSocketSender::emit(const json &json) {
    return emit(json.dump());
}

WebSocketSender &WebSocketSender::operator<<(const json &json) {
    if (isConnected())
        return emit(json.dump());
    else
        return *this;
}

bool WebSocketSender::isConnected() {
    return connected;
}

WebSocketSender &WebSocketSender::send(const string &str) {
    return emit(str);
}

WebSocketSender &WebSocketSender::send(const json &json) {
    return emit(json);
}

bool WebSocketSender::close() {
    LOCK(lock);
    wsconnect->close();
    connected = false;
    delete wsconnect;
    return !connected;
}

bool WebSocketSender::setAddress(const std::string &addr) {
    LOCK(lock);
    address_ = addr;
    return true;
}

bool WebSocketSender::setPort(const int port) {
    LOCK(lock);
    port_= port;
    return true;
}

void WebSocketSender::drain() {
    reverse(taskQueue_.begin(), taskQueue_.end());
    while(!taskQueue_.empty()) {
        taskQueue_.back().get();
        taskQueue_.pop_back();
    }
}

void WebSocketSender::initQueue(const string& url) {
    for(int i = 0, num = std::thread::hardware_concurrency(); i < num; ++i) {
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
