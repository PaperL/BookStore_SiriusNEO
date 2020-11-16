//
// Created by PaperL on 2020/11/9.
//

#ifndef BOOKSTORE_SIRIUSNEO_BOOKSTORE_H
#define BOOKSTORE_SIRIUSNEO_BOOKSTORE_H

#include <cstdio>
#include <iostream>

#include <algorithm>
#include <exception>

#include <vector>
#include <string>
#include <string.h>

#include "blocklist.h"
#include "usermanager.h"

#define PaperL_Debug

using namespace std;

class Book {
public:
    char ISBN[24];
    char name[64];
    char author[64];
    char keyword[64];//todo keyword是后加的，有一定概率出问题
    double price;
    int quantity;

    Book();

    bool operator<(const Book &x) const;
};

class Bookstore {
private:

    blocklist isbn_cmd;
    blocklist name_cmd;
    blocklist author_cmd;
    blocklist keyword_cmd;

    UserManager user_cmd;

    class BookstoreFileManager {
    private:
        const string fnameBook, fnameFinance, fnameLog;
        fstream fi, fo;
    public:
        BookstoreFileManager();

        inline void financeInit(int &tradeNum, double &income, double &outgo);

        inline void financeBasicWrite(int &num, double &income, double &outgo);//本可以用const的，但是强制类型转换好像不行

        inline void financeWrite(double &price, bool &sgn);//也可以用pair，但是没什么必要


        inline void bookInit(int &bookNum);

        inline void bookBasicWrite(int &bookNum);

        inline void freadBook(int offset, Book &arg);

        inline void freadBook(vector<Book> &array);//读取所有图书信息

        inline int fwriteBook(Book &arg);

        inline void fwriteBook(int offset, Book &arg);



        //此处输出功能原本应由 Bookstore::printBook 实现
        //但是为了降低空间复杂度在此函数内实现输出


        vector<string> freadFinance();

        void fwriteFinance();

        vector<string> freadLog();

        void fwriteLog();
    } bookstoreFile_cmd;

    enum logTypeEnum {
        reportMyself, reportEmployee, reportLog, reportFinance
    };

    enum findTypeEnum {
        findName, findAuthor, findKeyword
    };

    enum bookStringTypeEnum {
        stringISBN, stringBookName, stringAuthor, stringKeyword
    };

    int tradeNumber;//总交易次数
    double totIncome, totExpense;//总收入、总支出
    int bookNumber;//图书总数

    inline void splitString(string &arg, string &ret, int keywordFlag = 0);

    inline bool bookStringCheck(bookStringTypeEnum bookStringType, const string &arg);

    //inline Book freadBook(int offset);

    inline void printBook(const Book &arg);

    void showLog(logTypeEnum logType);

    void addFinance(double price, bool sgn);

    void showFinance(int time = -1);

    void import(int quantity, double price);

    void buy(const string &ISBN, const int &quantity);

    int find(const string &ISBN);

    void findplus(findTypeEnum findType, const string &key, vector<int> &array);

    void select(const string &ISBN);

    void modify(const int &offset, const string &ISBN, const string &name,
                const string &author, string keyword, const double &price);

public:
    Bookstore();

    void operation(string cmd);//记得判断权限
};


#endif //BOOKSTORE_SIRIUSNEO_BOOKSTORE_H
