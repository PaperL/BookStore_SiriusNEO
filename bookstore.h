//
// Created by PaperL on 2020/11/9.
//

#ifndef BOOKSTORE_SIRIUSNEO_BOOKSTORE_H
#define BOOKSTORE_SIRIUSNEO_BOOKSTORE_H

#include <string.h>
#include <vector>

#include "blocklist.h"
#include "usermanager.h"

using namespace std;

blocklist isbn_cmd("isbn.bin");
blocklist name_cmd("name.bin");
blocklist author_cmd("author.bin");
blocklist keyword_cmd("keyword.bin");

blocklist finance_cmd("finance.dat");

class Book {
public:
    char ISBN[24];
    char name[64];
    char author[64];
    double price;
    int quantity;

    Book();
};

class Bookstore {
private:
    UserManager user_cmd;

    enum logTypeEnum {
        reportMyself, reportEmployee, reportLog
    };

    enum findTypeEnum {
        findName, findAuthor, findKeyword
    };

    int bookNumber;

    inline void splitString(string &arg,string &ret);

    void showLog(logTypeEnum logType);

    void addFinance(double price, bool sgn);

    void showFinance(int time);

    void import(int quantity, double price);

    void buy(string ISBN, int quantity);

    int find(string ISBN);

    void findplus(findTypeEnum findType, string key, vector<int> &array);

    void select(string ISBN);

    void modify(int offset, string ISBN, string name, string author, string keyword, double price);

public:
    Bookstore();

    void operation(string cmd);//记得判断权限
};


#endif //BOOKSTORE_SIRIUSNEO_BOOKSTORE_H
