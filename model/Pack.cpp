#include <utility>

//
// Created by Ryan Lee on 2019-07-03.
//

#include "Pack.h"

Pack::Pack() {
    type = UNDEFINED;
}

Pack::Pack(int num) {
    setInt(num);
}

Pack::Pack(double num) {
    setFloat(num);
}

Pack::Pack(string str) {
    setString(std::move(str));
}

int Pack::getInt() {
    return num;
}

Pack &Pack::setInt(int num) {
    type = INT;
    this->num = num;
    return *this;
}

double Pack::getFloat() {
    return f;
}

Pack &Pack::setFloat(double num) {
    type = DOUBLE;
    this->f = num;
    return *this;
}

string &Pack::getString() {
    return str;
}


Pack &Pack::setString(string str) {
    type = STRING;
    this->str = std::move(str);
    return *this;
}

int Pack::getType() {
    return type;
}

bool Pack::isInt() {
    return type == INT;
}

bool Pack::isFloat() {
    return type == DOUBLE;
}

bool Pack::isString() {
    return type == STRING;
}

bool Pack::increment() {
    if (isInt()) {
        ++num;
        return true;
    }
    else {
        return false;
    }
}

Pack::operator int() {
    return type == INT ? num : 0;
}

Pack::operator string() {
    return type == STRING ? str : string();
}

Pack::operator double() {
    return type == DOUBLE ? f : 0.0f;
}