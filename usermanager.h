//
// Created by PaperL on 2020/11/9.
//
#ifndef BOOKSTORE_SIRIUSNEO_USERMANAGER_H
#define BOOKSTORE_SIRIUSNEO_USERMANAGER_H

#include <cstdio>
#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <string.h>


#include "blocklist.h"

#define PaperL_Debug

using namespace std;

class User {
public:
    int privilege;
    int curBook;
    char id[32];
    char passwd[32];
    char name[32];

    User();
};

class UserManager {
private:

    blocklist id_cmd;

    const string fname;
    fstream fi, fo, fip, fop;

    enum userStringTypeEnum {
        stringId, stringPasswd, stringName
    };

    int userNumber;
    vector<User> userStack;

    //blocklist id_cmd = blocklist("id.bin");
    //blocklist id_cmd("id.bin"); 这种写法会有歧义,编译器无法判断是成员变量还是成员函数

    inline bool userStringCheck(userStringTypeEnum userStringType, string arg);

    //inline void fwriteUser(int pri,string id,string passwd,string name);

    inline User freadUser(int offset);
    //fwriteUser已直接在useadd和repwd中实现

public:
    UserManager();

    bool privilegeCheck(int privilegeNeed);

    int userSelect();

    void changeSelect(int offset);

    void su(string id, string passwd);

    void logout();

    void reg(string id, string passwd, string name);

    void useradd(string id, string passwd, int privilege, string name, int registerFlag = 0);

    void repwd(string id, string oldpwd, string newpwd);

    void del(string id);


};


#endif //BOOKSTORE_SIRIUSNEO_USERMANAGER_H
