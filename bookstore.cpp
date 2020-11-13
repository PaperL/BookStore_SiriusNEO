//
// Created by PaperL on 2020/11/9.
//

#include "bookstore.h"

Book::Book() {
    memset(ISBN, 0, sizeof(ISBN));
    memset(name, 0, sizeof(name));
    memset(author, 0, sizeof(author));
    price = -1;
    quantity = 0;
}

Bookstore::Bookstore() {
    bookNumber = 0;
}

inline void Bookstore::splitString(string &arg, string &ret) {//将arg拆分出第一部分ret
    //todo 可能未完成
    int p1 = 0, p2;
    ret.clear();
    while ((arg[p1] == ' ' || arg[p1] == '\t') && p1 < arg.length())++p1;
    if (p1 < arg.length()) {
        p2 = p1;
        while ((arg[p2] != ' ' && arg[p2] != '\t') && p2 < arg.length())++p2;
        ret = arg.substr(p1, p2 - p1);
        arg = arg.substr(p2, arg.length() - p2);
    }
}

void Bookstore::showLog(logTypeEnum logType) {}

void Bookstore::addFinance(double price, bool sgn) {}

void Bookstore::showFinance(int time) {}

void Bookstore::import(int quantity, double price) {}

void Bookstore::buy(string ISBN, int quantity) {}

int Bookstore::find(string ISBN) {}

void Bookstore::findplus(findTypeEnum findType, string key, vector<int> &array) {}

void Bookstore::select(string ISBN) {}

void Bookstore::modify(int offset, string ISBN, string name, string author, string keyword, double price) {}

void Bookstore::operation(string cmd) {
    string arg;
    splitString(cmd, arg);

}


