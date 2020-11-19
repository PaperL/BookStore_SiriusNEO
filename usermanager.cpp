//
// Created by PaperL on 2020/11/9.
//

#include "usermanager.h"

User::User() {
#ifdef PaperL_Debug
    cout << "In Constructor \"User\" :" << endl;
#endif
    privilege = -1;
    curBook = -1;
    memset(id, 0, sizeof(id));
    memset(passwd, 0, sizeof(passwd));
    memset(name, 0, sizeof(name));
}

User &User::operator=(const User &right) {
    if (this == &right)
        return *this;
    privilege = right.privilege;
    curBook = right.curBook;
    strcpy(id, right.id);
    strcpy(passwd, right.passwd);
    strcpy(name, right.name);
    return *this;
}

UserManager::UserManager() : id_cmd("id.bin"), fname("users.dat") {
#ifdef PaperL_Debug
    cout << "In Constructor \"UserManager\" :" << endl;
#endif
    //初始只有一个0权限空账户
    User tempUser;
    tempUser.privilege = 0;
    userStack.clear();
    userStack.push_back(tempUser);
    userNumber = 1;

    fi.open(fname, ios::in | ios::binary);
    if (!fi.is_open()) {//第一次运行创建文件
        fi.close();
#ifdef PaperL_Debug
        cout << "Discover data file \"" << fname << "\" is missing." << endl;
        //cout << "Creating blank file \"" << fname << "\" ..." << endl;
#endif
        fo.open(fname, ios::out | ios::trunc | ios::binary);
        //fo.clear();
        fo.seekp(0, ios::beg);//设置root账户
        string temps;
        tempUser.privilege = 7;
        temps = "root", strcpy(tempUser.id, temps.c_str());
        temps = "root", strcpy(tempUser.name, temps.c_str());
        temps = "sjtu", strcpy(tempUser.passwd, temps.c_str());
        fo.write(reinterpret_cast<char *>(&tempUser), sizeof(User));
        fo.close();
        id_cmd.addNode(Node(0, "root"));
    } else fi.close();

    /*fi.open(fname, ios::in | ios::binary);//todo seekg到文件末后会返回failure -1, 需要clear才能使用
    fip.open(fname, ios::in | ios::binary);

    fo.open(fname, ios::in | ios::out | ios::binary);
    fop.open(fname, ios::in | ios::out | ios::binary);*/
}

UserManager::~UserManager() {
#ifdef PaperL_Debug
    cout << "In Destructor \"UserManager\" :" << endl;
#endif
    fi.close();
    fip.close();

    fo.close();
    fop.close();
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

inline User UserManager::freadUser(const int &offset) {
#ifdef PaperL_Debug
    cout << "In Function \"freadUser\":" << endl;
#endif
    User tempUser;
    fip.open(fname, ios::in | ios::binary);
    //fip.clear();
    fip.seekg(offset, ios::beg);
    fip.read(reinterpret_cast<char *>(&tempUser), sizeof(User));
    fip.close();
    return tempUser;
}

bool UserManager::privilegeCheck(int privilegeNeed) {
    if (userStack[userNumber - 1].privilege < privilegeNeed) {//没有足够权限
#ifdef PaperL_Debug
        cout << "    privilegeCheck (" << privilegeNeed << ") fail" << endl;
#endif
        return false;
    } else {
#ifdef PaperL_Debug
        cout << "    privilegeCheck (" << privilegeNeed << ") succeed" << endl;
#endif
        return true;
    }
}

int UserManager::userSelect() {
#ifdef PaperL_Debug
    cout << "In Function \"userSelect\":" << endl;
#endif
    return userStack[userNumber - 1].curBook;
}

void UserManager::changeSelect(const int &offset) {
#ifdef PaperL_Debug
    cout << "In Function \"changeSelect\":" << endl;
#endif
    userStack[userNumber - 1].curBook = offset;
}

void UserManager::su(string id, string passwd) {
#ifdef PaperL_Debug
    cout << "In Function \"su\":" << endl;
#endif
    if (id.empty() || !userStringCheck(stringId, id) || !userStringCheck(stringPasswd, passwd)) {
        printf("Invalid\n");//判断字符串合法
        return;
    }
    char highPrivilegeCheck = 0;
    if (passwd.empty())//高权限账户登录到低权限不需要密码
        highPrivilegeCheck = 1;

    vector<int> tempVec;
    id_cmd.findNode(id, tempVec);//查找是否有该用户
    if (tempVec.empty()) {
        printf("Invalid\n");
        return;
    }
#ifdef PaperL_Debug
    if (tempVec.size() > 1)
        printf("UserManager::su Error! 发现相同id用户");
    cout << "tempVec[0] = " << tempVec[0] << endl;
#endif
    string temps;
    User tempUser;
    tempUser = freadUser(tempVec[0]);
#ifdef PaperL_Debug
    cout << "freadUser : privilege = " << tempUser.privilege << ", id = \"" << tempUser.id << "\", passwd = \""
         << tempUser.passwd << "\"" << endl;
    cout << "            name = \"" << tempUser.name << "\", curBook = " << tempUser.curBook << endl;
#endif
    if (highPrivilegeCheck == 0) {
        temps = tempUser.passwd;
#ifdef PaperL_Debug
        if (passwd != temps)
            cout << "    wrong password" << endl;
#endif
        if (passwd == temps) {//检查密码，加入UserStack
            tempUser.curBook = -1;
            ++userNumber;
#ifdef PaperL_Debug
            cout << "    privilege:" << tempUser.privilege << "  userNumber:" << userNumber << endl;
#endif
            userStack.push_back(tempUser);
        }//如果目前没有该账户或密码错误
        else printf("Invalid\n");
    } else {//检查是否为高权限登录到低权限
        if (privilegeCheck(tempUser.privilege + 1)) {// + 1 : 需要权限大于
            tempUser.curBook = -1;
            ++userNumber;
            userStack.push_back(tempUser);
        } else printf("Invalid\n");
    }
}

void UserManager::logout() {
#ifdef PaperL_Debug
    cout << "In Function \"logout\":" << endl;
#endif
    //无登录账户（只有栈底的0权限空账户)时操作非法
    //只要有登录账户，权限必大于等于1
    if (userNumber == 1)
        printf("Invalid\n");
    else {
        userStack.erase(userStack.end() - 1);
        //Vector加入元素需用push_back，所以logout要删除元素，不能懒惰删除
        --userNumber;
    }
}

void UserManager::reg(string id, string passwd, string name) {//注册权限为1账户
#ifdef PaperL_Debug
    cout << "In Function \"reg\":" << endl;
#endif
    useradd(id, passwd, 1, name, 1);
}

void UserManager::useradd(string id, string passwd, int privilege, string name, int registerFlag) {
#ifdef PaperL_Debug
    cout << "In Function \"useradd\":" << endl;
#endif
    if (registerFlag == 0//非注册指令,即useradd指令时,需要权限(3)且不能创建比自身权限大的账户
        && (userStack[userNumber - 1].privilege < 3 || userStack[userNumber - 1].privilege <= privilege)) {
        printf("Invalid\n");
        return;
    }
    if (id.empty() || passwd.empty() || name.empty()//任何一项为空则非法
        || !userStringCheck(stringId, id) || !userStringCheck(stringPasswd, passwd)
        || !userStringCheck(stringName, name)
        || !(privilege == 1 || privilege == 3 || privilege == 7)) {//判断账户合法
        printf("Invalid\n");
        return;//输入不合法
    }

    vector<int> tempVec;
    User tempUser;
    id_cmd.findNode(id, tempVec);//查找是否有该用户
    if (!tempVec.empty()) {//存在同id账户
#ifdef PaperL_Debug
        if (tempVec.size() > 1)
            printf("UserManager::su Error! 发现相同id用户");
#endif
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
    //fo.clear();
    fo.seekp(0, ios::end);
    Node tempNode((int) fo.tellp(), id);
    fo.write(reinterpret_cast<char *>(&tempUser), sizeof(User));
    fo.close();
    //添加账户索引
    id_cmd.addNode(tempNode);
}

void UserManager::repwd(string id, string oldpwd, string newpwd) {
#ifdef PaperL_Debug
    cout << "In Function \"repwd\":" << endl;
#endif
    //本函数很多内容来自 void UserManager::su
    if (!privilegeCheck(1)) {//没有足够权限(1)
        printf("Invalid\n");
        return;
    }
    //权限7的账户可以省略oldpwd，但若不省略且输入错误oldpwd，仍为非法操作
    char oldpwdIgnored = 0;
    if (newpwd.empty()) {
        if (userStack[userNumber - 1].privilege == 7) {
            newpwd = oldpwd;
            oldpwdIgnored = 1;
        } else {
            printf("Invalid\n");
            return;
        }
    }
    if (newpwd.empty() || !userStringCheck(stringId, id)
        || !userStringCheck(stringPasswd, oldpwd) || !userStringCheck(stringPasswd, newpwd)) {//判断字符串合法
        printf("Invalid\n");
        return;
    }

    vector<int> tempVec;
    id_cmd.findNode(id, tempVec);//查找是否有该用户
    if (tempVec.empty()) {//没有该账户
        printf("Invalid\n");
        return;
    }
#ifdef PaperL_Debug
    if (tempVec.size() > 1)
        printf("UserManager::repwd Error! 找到多个同id用户");
#endif
    int offset;
    User tempUser;
    string temps;

    offset = tempVec[0];
    tempUser = freadUser(offset);
    temps=tempUser.passwd;
    if (oldpwdIgnored == 0 && oldpwd != temps) {
        //未省略或不可省略oldpwd情况下密码错误
        printf("Invalid\n");
    } else {//省略或密码正确
        //修改账户密码并写入文件
        strcpy(tempUser.passwd, newpwd.c_str());
        fo.open(fname, ios::in | ios::out | ios::binary);
        //fo.clear();
        fo.seekp(offset, ios::beg);
        fo.write(reinterpret_cast<char *>(&tempUser), sizeof(User));
        fo.close();
    }
}

void UserManager::del(string id) {
#ifdef PaperL_Debug
    cout << "In Function \"del\":" << endl;
#endif
    if (!privilegeCheck(7)) {//没有足够权限(7)
        printf("Invalid\n");
        return;
    }
    if (id.empty() || !userStringCheck(stringId, id)) {
        printf("Invalid\n");
        return;
    }
    //不能删除已经登陆的账户
    string temps;
    for (int i = 0; i < userNumber; ++i) {
        temps = userStack[i].id;
        if (temps == id) {
            printf("Invalid\n");
            return;
        }
    }

    vector<int> tempVec;
    id_cmd.findNode(id, tempVec);
    if (!tempVec.empty()) {
#ifdef PaperL_Debug
        if (tempVec.size() > 1)
            printf("UserManager::del Error! 发现重id用户");
#endif
        if (id_cmd.deleteNode(Node(tempVec[0], id)) != 0)//删除操作失败
            printf("Invalid\n");
    } else printf("Invalid\n");
}

