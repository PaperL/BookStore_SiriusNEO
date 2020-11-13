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

UserManager::UserManager() {
    userStack.clear();
    userNumber = 0;

    fi.open(fname, ios::in | ios::binary);
    if (!fi.is_open()) {//第一次运行创建文件
        fi.close();
        cout << "Discover data file \"" << fname << "\" is missing." << endl;
        cout << "Creating blank file \"" << fname << "\" ..." << endl;
        fo.open(fname, ios::out | ios::trunc | ios::binary);
        fo.seekp(0, ios::beg);//设置root账户
        User tempUser;
        string temps;
        tempUser.privilege = 7;
        temps = "root", strcpy(tempUser.id, temps.c_str());
        temps = "root", strcpy(tempUser.name, temps.c_str());
        temps = "sjtu", strcpy(tempUser.passwd, temps.c_str());
        fo.write(reinterpret_cast<char *>(&tempUser), sizeof(User));
        fo.close();
    } else fi.close();
}

inline bool UserManager::sCheck(string arg) {//判断是否为连续无空格数字字母下划线，长度不超过30
    if (arg.size() > 30)return false;
    for (int i = arg.size() - 1; i >= 0; --i) {
        if (!((arg[i] >= 'a' && arg[i] <= 'z') || (arg[i] >= 'A' && arg[i] <= 'Z')
              || (arg[i] >= '0' && arg[i] <= '9') || arg[i] == '_'))
            return false;
    }
    return true;
}

inline User UserManager::freadUser(int offset) {
    User tempUser;
    fip.open(fname, ios::in | ios::binary);
    fip.seekg(offset, ios::beg);
    fip.read(reinterpret_cast<char *>(&tempUser), sizeof(User));
    fip.close();
    return tempUser;
}

void UserManager::su(string id, string passwd) {
    if (!(sCheck(id) && sCheck(passwd))) {//判断字符串合法
        printf("Invalid\n");
        return;
    }
    vector<Node> tempVec;
    id_cmd.findNode(id, tempVec);//查找是否有该用户
    if (tempVec.empty()) {
        printf("Invalid\n");
        return;
    }
    char findFlag = 0;
    User tempUser;
    for (int i = tempVec.size() - 1; i >= 0; --i) {
        if (tempVec[i].isdel == 0) {
            findFlag = 1;
            tempUser = freadUser(tempVec[i].offset);
            break;
        }
    }
    if (findFlag == 1 && passwd == tempUser.passwd) {//检查密码，加入UserStack
        tempUser.curBook = -1;
        userStack[userNumber++] = tempUser;
    }//如果目前没有该账户或密码错误
    else printf("Invalid\n");
}

void UserManager::logout() {
    if (userNumber == 0) {
        printf("Invalid\n");
        return;
    }
    --userNumber;
}

void UserManager::reg(string id, string passwd, string name) {//注册权限为1账户
    useradd(id, passwd, 1, name);
}

void UserManager::useradd(string id, string passwd, int privilege, string name) {
    if (!(sCheck(id) && sCheck(passwd) && sCheck(name)
          && (privilege == 0 || privilege == 1 || privilege == 3 || privilege == 7))) {//判断账户合法
        printf("Invalid\n");
        return;
    }

    vector<Node> tempVec;
    User tempUser;
    id_cmd.findNode(id, tempVec);//查找是否有该用户
    if (!tempVec.empty()) {
        char findFlag = 0;

        for (int i = tempVec.size() - 1; i >= 0; --i) {
            if (tempVec[i].isdel == 0) {
                findFlag = 1;
                break;
            }
        }
        if (findFlag == 1) {//存在同id用户
            printf("Invalid\n");
            return;
        }
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

void UserManager::repwd(string id, string oldpwd, string newpwd) {//todo 注意root账户没有oldpwd的情况
    //本函数很多内容来自 void UserManager::su
    if (!(sCheck(id) && sCheck(oldpwd) && sCheck(newpwd))) {//判断字符串合法
        printf("Invalid\n");
        return;
    }

    vector<Node> tempVec;
    id_cmd.findNode(id, tempVec);//查找是否有该用户
    if (tempVec.empty()) {
        printf("Invalid\n");
        return;
    }
    int offset;
    char findFlag = 0;
    User tempUser;
    for (int i = tempVec.size() - 1; i >= 0; --i) {
        if (tempVec[i].isdel == 0) {
            findFlag = 1;
            offset = tempVec[i].offset;
            tempUser = freadUser(offset);
            break;
        }
    }
    if (findFlag == 0)return;//没有该账户
    //当前登陆用户如果是非root用户需验证密码,root用户不用
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
    if (!sCheck(id)) {
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

