//
// Created by Ryan Lee on 2019-07-03.
//

#ifndef JUDGE_CLIENT_PACK_H
#define JUDGE_CLIENT_PACK_H
#include <string>
using std::string;
enum {
    INT,
    DOUBLE,
    STRING,
    UNDEFINED
};

class Pack {
public:
    Pack();
    Pack(int);
    Pack(double);
    Pack(string);
    int getInt();

    Pack& setInt(int);

    bool isInt();

    bool increment();

    double getFloat();

    Pack& setFloat(double);

    bool isFloat();

    string& getString();

    Pack& setString(string);

    bool isString();

    int getType();

    operator int();

    operator string();

    operator double();
private:
    int num{};
    double f{};
    string str;
    int type;
};


#endif //JUDGE_CLIENT_PACK_H
