//
// Created by PaperL on 2020/11/9.
//

#include "usermanager.h"

User::User() {
    privilege = -1;
    curBook = -1;
    memset(id, 0, sizeof(id));
    memset(passwd, 0, sizeof(passwd));
    memset(name, 0, sizeof(name));
}

UserManager::UserManager() : fname("users.dat") {
    //初始只有一个0权限空账户
    User tempUser;
    tempUser.privilege = 0;
    userStack.clear();
    userStack[0] = tempUser;
    userNumber = 1;

    fi.open(fname, ios::in | ios::binary);
    if (!fi.is_open()) {//第一次运行创建文件
        fi.close();
        cout << "Discover data file \"" << fname << "\" is missing." << endl;
        cout << "Creating blank file \"" << fname << "\" ..." << endl;
        fo.open(fname, ios::out | ios::trunc | ios::binary);
        fo.seekp(0, ios::beg);//设置root账户
        string temps;
        tempUser.privilege = 7;
        temps = "root", strcpy(tempUser.id, temps.c_str());
        temps = "root", strcpy(tempUser.name, temps.c_str());
        temps = "sjtu", strcpy(tempUser.passwd, temps.c_str());
        fo.write(reinterpret_cast<char *>(&tempUser), sizeof(User));
        fo.close();
    } else fi.close();
}

inline bool UserManager::userStringCheck(userStringTypeEnum userStringType, string arg) {
    if (arg.size() > 30)return false;
    //id/passwd:判断是否为连续无空格数字字母下划线，长度不超过30
    if (userStringType == stringId || userStringType == stringPasswd) {
        for (int i = arg.size() - 1; i >= 0; --i) {
            if (!((arg[i] >= 'a' && arg[i] <= 'z') || (arg[i] >= 'A' && arg[i] <= 'Z')
                  || (arg[i] >= '0' && arg[i] <= '9') || arg[i] == '_'))
                return false;
        }
    }
    return true;
    //name:判断是否为连续无空格，长度不超过30
    //无空格在切分时已保证，详见bookstore.h内splitString
}

inline User UserManager::freadUser(int offset) {
    User tempUser;
    fip.open(fname, ios::in | ios::binary);
    fip.seekg(offset, ios::beg);
    fip.read(reinterpret_cast<char *>(&tempUser), sizeof(User));
    fip.close();
    return tempUser;
}

bool UserManager::privilegeCheck(int privilegeNeed) {
    if (userStack[userNumber - 1].privilege < privilegeNeed)//没有足够权限
        return false;
    else
        return true;
}

inline int UserManager::userSelect() { return userStack[userNumber - 1].curBook; }

inline void UserManager::changeSelect(int offset) { userStack[userNumber - 1].curBook = offset; }

void UserManager::su(string id, string passwd) {
    if (!(userStringCheck(stringId, id) && userStringCheck(stringPasswd, passwd))) {//判断字符串合法
        printf("Invalid\n");
        return;
    }
    vector<int> tempVec;
    id_cmd.findNode(id, tempVec);//查找是否有该用户
    if (tempVec.empty()) {
        printf("Invalid\n");
        return;
    }
    //todo 可删除调试代码
    if (tempVec.size() > 1)
        printf("UserManager::su Error! 发现相同id用户");

    User tempUser;
    tempUser = freadUser(tempVec[0]);
    if (passwd == tempUser.passwd) {//检查密码，加入UserStack
        tempUser.curBook = -1;
        userStack[userNumber++] = tempUser;
    }//如果目前没有该账户或密码错误
    else printf("Invalid\n");
}

void UserManager::logout() {
    //无登录账户（只有栈底的0权限空账户)
    if (userNumber == 1)
        printf("Invalid\n");
    else
        --userNumber;
}

void UserManager::reg(string id, string passwd, string name) {//注册权限为1账户
    useradd(id, passwd, 1, name, 1);
}

void UserManager::useradd(string id, string passwd, int privilege, string name, int registerFlag) {
    if (registerFlag == 0 && !privilegeCheck(3)) {//没有足够权限(3)
        printf("Invalid\n");
        return;
    }
    if (!(userStringCheck(stringId, id) && userStringCheck(stringPasswd, passwd) && userStringCheck(stringName, name)
          && (privilege == 0 || privilege == 1 || privilege == 3 || privilege == 7))) {//判断账户合法
        printf("Invalid\n");
        return;//输入不合法
    }

    vector<int> tempVec;
    User tempUser;
    id_cmd.findNode(id, tempVec);//查找是否有该用户
    if (!tempVec.empty()) {//存在同id账户
        //todo 可删除调试代码
        if (tempVec.size() > 1)
            printf("UserManager::su Error! 发现相同id用户");

        printf("Invalid\n");
        return;
    }
    //账户写入文件
    tempUser.privilege = privilege;
    tempUser.curBook = -1;
    strcpy(tempUser.id, id.c_str());
    strcpy(tempUser.passwd, passwd.c_str());
    strcpy(tempUser.name, name.c_str());
    fo.open(fname, ios::in | ios::out | ios::binary);
    fo.seekp(0, ios::end);
    Node tempNode((int) fo.tellp(), id);
    fo.write(reinterpret_cast<char *>(&tempUser), sizeof(User));
    fo.close();
    //添加账户索引
    id_cmd.addNode(tempNode);
}

void UserManager::repwd(string id, string newpwd, string oldpwd) {
    //本函数很多内容来自 void UserManager::su
    if (!privilegeCheck(1)) {//没有足够权限(1)
        printf("Invalid\n");
        return;
    }
    if (!(userStringCheck(stringId, id) && userStringCheck(stringPasswd, oldpwd) &&
          userStringCheck(stringPasswd, newpwd))) {//判断字符串合法
        printf("Invalid\n");
        return;
    }

    vector<int> tempVec;
    id_cmd.findNode(id, tempVec);//查找是否有该用户
    if (tempVec.empty()) {//没有该账户
        printf("Invalid\n");
        return;
    }
    //todo 可删除调试代码
    if (tempVec.size() > 1)
        printf("UserManager::repwd Error! 找到多个同id用户");

    int offset;
    User tempUser;
    offset = tempVec[0];
    tempUser = freadUser(offset);
    //当前登陆用户如果是非root用户需验证密码,root用户不用
    //高权限用户低权限用户，提供密码，但是密码错误，为Invalid。
    if (userStack[userNumber - 1].privilege != 7 && oldpwd == tempUser.passwd) {
        //修改账户密码并写入文件
        strcpy(tempUser.passwd, newpwd.c_str());
        fo.open(fname, ios::in | ios::out | ios::binary);
        fo.seekp(offset, ios::beg);
        fo.write(reinterpret_cast<char *>(&tempUser), sizeof(User));
        fo.close();
    }//如果非root用户密码错误
    else printf("Invalid\n");
}

void UserManager::del(string id) {
    //注意不能删除已经登陆的账户！
    if (!privilegeCheck(7)) {//没有足够权限(7)
        printf("Invalid\n");
        return;
    }
    if (!userStringCheck(stringId, id)) {
        printf("Invalid\n");
        return;
    }
    string temps;
    for (int i = 0; i < userNumber; ++i) {
        temps = userStack[i].id;
        if (temps == id) {
            printf("Invalid\n");
            return;
        }
    }
    if (id_cmd.deleteNode(id) != 0)//删除操作失败
        printf("Invalid\n");
}

