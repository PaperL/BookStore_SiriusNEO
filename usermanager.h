//
// Created by PaperL on 2020/11/9.
//
#ifndef BOOKSTORE_SIRIUSNEO_USERMANAGER_H
#define BOOKSTORE_SIRIUSNEO_USERMANAGER_H

#include <string>
#include <vector>

#include "blocklist.h"

using namespace std;

extern blocklist id_cmd;

class User {
public:
    int privilege;
    int curBook;
    string id, passwd, name;
};

class UserManager {
private:
public:
    int userNumber;
    vector<User> userStack;

    UserManager();

    void su(string id, string passwd);

    void logout();

    void reg(string id, string passwd, string name);

    void useradd(string id, string passwd, int privilege, string name);

    void repwd(string id, string newpws, string oldpwd);

    void del(string id);
};


#endif //BOOKSTORE_SIRIUSNEO_USERMANAGER_H
