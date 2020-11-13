//
// Created by PaperL on 2020/11/9.
//
#ifndef BOOKSTORE_SIRIUSNEO_USERMANAGER_H
#define BOOKSTORE_SIRIUSNEO_USERMANAGER_H

#include <fstream>
#include <string.h>
#include <string>
#include <vector>

#include "blocklist.h"

using namespace std;

blocklist id_cmd("id.bin");

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
    const string fname = "users.dat";
    fstream fi, fo, fip, fop;

    int userNumber;
    vector<User> userStack;

    inline bool sCheck(string arg);

    //inline void fwriteUser(int pri,string id,string passwd,string name);

    inline User freadUser(int offset);

public:
    UserManager();

    void su(string id, string passwd);

    void logout();

    void reg(string id, string passwd, string name);

    void useradd(string id, string passwd, int privilege, string name);

    void repwd(string id, string oldpwd, string newpwd);

    void del(string id);
};


#endif //BOOKSTORE_SIRIUSNEO_USERMANAGER_H
