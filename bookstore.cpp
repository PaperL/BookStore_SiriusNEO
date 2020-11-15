//
// Created by PaperL on 2020/11/9.
//

#include "bookstore.h"

Book::Book() {
    memset(ISBN, 0, sizeof(ISBN));
    memset(name, 0, sizeof(name));
    memset(author, 0, sizeof(author));
    memset(keyword, 0, sizeof(keyword));
    price = -1;
    quantity = 0;
}

bool Book::operator<(const Book &x) const {
    string s1 = ISBN, s2 = x.ISBN;
    return s1 < s2;
}

//================================
//===== BookstoreFileManager =====
//================================

Bookstore::BookstoreFileManager::BookstoreFileManager() :
        fnameBook("books.dat"), fnameFinance("finance.dat"), fnameLog("log.dat") {
    //检查book.dat
    fi.open(fnameBook, ios::in | ios::binary);
    if (!fi.is_open()) {
        fi.close();
        cout << "Discover index file \"" << fnameBook << "\" is missing." << endl;
        cout << "Creating blank file \"" << fnameBook << "\" ..." << endl;
        fi.open(fnameBook, ios::out | ios::trunc | ios::binary);
    }
    fi.close();
    //检查finance.dat
    fi.open(fnameFinance, ios::in | ios::binary);
    if (!fi.is_open()) {
        fi.close();
        cout << "Discover index file \"" << fnameFinance << "\" is missing." << endl;
        cout << "Creating blank file \"" << fnameFinance << "\" ..." << endl;
        fi.open(fnameFinance, ios::out | ios::trunc | ios::binary);
    }
    fi.close();
    //检查log.dat
    fi.open(fnameLog, ios::in | ios::binary);
    if (!fi.is_open()) {
        fi.close();
        cout << "Discover index file \"" << fnameLog << "\" is missing." << endl;
        cout << "Creating blank file \"" << fnameLog << "\" ..." << endl;
        fi.open(fnameLog, ios::out | ios::trunc | ios::binary);
    }
    fi.close();
}

inline void Bookstore::BookstoreFileManager::freadBook(int offset, Book &arg) {
    fi.open(fnameBook, ios::in | ios::binary);
    fi.seekg(offset, ios::beg);
    fi.read(reinterpret_cast<char *>(&arg), sizeof(Book));
    fi.close();
}

inline int Bookstore::BookstoreFileManager::fwriteBook(Book &arg) {
    int temp;
    fo.open(fnameBook, ios::in | ios::out | ios::binary);
    fo.seekp(0, ios::end);
    temp = fo.tellp();
    fo.write(reinterpret_cast<char *>(&arg), sizeof(Book));
    fo.close();
    return temp;
}

inline void Bookstore::BookstoreFileManager::fwriteBook(int offset, Book &arg) {
    fo.open(fnameBook, ios::in | ios::out | ios::binary);
    fo.seekp(offset, ios::beg);
    fo.write(reinterpret_cast<char *>(&arg), sizeof(Book));
    fo.close();
}

//================================
//========== Bookstore ===========
//================================

Bookstore::Bookstore() {
    bookNumber = 0;
}

inline void Bookstore::splitString(string &arg, string &ret, int keywordFlag) {//将arg拆分出第一部分ret
    int p1 = 0, p2;
    ret.clear();
    if (keywordFlag == 0) {//正常情况,以空白符(空格与制表符)为分隔符
        while ((arg[p1] == ' ' || arg[p1] == '\t') && p1 < arg.length())++p1;
        if (p1 < arg.length()) {
            p2 = p1;
            while ((arg[p2] != ' ' && arg[p2] != '\t') && p2 < arg.length())++p2;
            ret = arg.substr(p1, p2 - p1);
            arg = arg.substr(p2, arg.length() - p2);
        }
    } else {//拆分keyword字符串的情况,以'|'为分隔符
        //理论上此时arg已经没有首尾空白符了
        while ((arg[p1] != '|') && p1 < arg.length())++p1;
        if (p1 < arg.length() - 1) {
            ret = arg.substr(0, p1);
            arg = arg.substr(p1 + 1, arg.length() - p1 - 1);
        }
    }
}

inline bool
Bookstore::bookStringCheck(bookStringTypeEnum bookStringType, const string &arg) {//todo const引用是个好东西,最好能用的都用上
    //该函数视空arg为合法
    if (bookStringType == stringISBN) {
        if (arg.length() > 20)return false;
        //此处题面未注明ISBN的合法字符集
        /*for (int i = 0; i < arg.length(); ++i) {
            if (!((arg[i] >= '0' && arg[i] <= '9') || (arg[i] == '-')))
                return false;
        }*/
    } else if (bookStringType == stringBookName || bookStringType == stringAuthor) {
        if (arg.length() > 62)return false;
        if (arg[0] != '\"' || arg[arg.length() - 1] != '\"')return false;
        for (int i = 1; i < arg.length() - 1; ++i) {
            if (!((arg[i] >= 'a' && arg[i] <= 'z') || (arg[i] >= 'A' && arg[i] <= 'Z')))
                return false;
        }
    } else {//bookStringType == stringKeyword
        //此处只判断单个keyword,即'|'为非法字符
        if (arg.length() > 62)return false;
        if (arg[0] != '\"' || arg[arg.length() - 1] != '\"')return false;
        for (int i = 1; i < arg.length() - 1; ++i) {
            if (!((arg[i] >= 'a' && arg[i] <= 'z') || (arg[i] >= 'A' && arg[i] <= 'Z')))
                return false;
        }
    }
    return true;
}

//inline Book Bookstore::freadBook(int offset) {}

inline void Bookstore::printBook(Book &arg) {

}

void Bookstore::showLog(logTypeEnum logType) {}

void Bookstore::addFinance(double price, bool sgn) {}

void Bookstore::showFinance(int time) {}

void Bookstore::import(int quantity, double price) {
    //select为-1时不能进行modify和import操作
    //在su时已经保证初始select为-1
    //import的price是总价而非单价
}

void Bookstore::buy(string ISBN, int quantity) {
    //todo buy指令输出购买花费，保留两位小数
}

int Bookstore::find(string ISBN) {}

void Bookstore::findplus(findTypeEnum findType, string key, vector<int> &array) {}

void Bookstore::select(string ISBN) {
    if (!user_cmd.privilegeCheck(3) || !bookStringCheck(stringISBN, ISBN)) {//没有足够权限(7)
        printf("Invalid\n");
        return;
    }
    Book tempBook;
    int temp = find(ISBN);
    if (temp == -1) {//没有此书则新建空白书
        strcpy(tempBook.ISBN, ISBN.c_str());
        user_cmd.changeSelect(bookstoreFile_cmd.fwriteBook(tempBook));
    } else user_cmd.changeSelect(temp);//有则选中
}

void Bookstore::modify(int offset, string ISBN, string name, string author, string keyword, double price) {
    //price在operation中保证非负，-1表无值
    if (offset == -1 || !bookStringCheck(stringISBN, ISBN) || !bookStringCheck(stringBookName, name)
        || !bookStringCheck(stringAuthor, author)) {
        printf("Invalid\n");
        return;
    }
    Book tempBook;

    bookstoreFile_cmd.freadBook(offset, tempBook);
    if (!ISBN.empty()) {
        isbn_cmd.deleteNode(tempBook.ISBN);
        isbn_cmd.addNode(Node(offset, ISBN));
        strcpy(tempBook.ISBN, ISBN.c_str());
    }
    if (!name.empty()) {
        name_cmd.deleteNode(tempBook.name);
        name_cmd.addNode(Node(offset, name));
        strcpy(tempBook.name, name.c_str());
    }
    if (!author.empty()) {
        author_cmd.deleteNode(tempBook.author);
        author_cmd.addNode(Node(offset, author));
        strcpy(tempBook.author, author.c_str());
    }
    if (!keyword.empty()) {
        string temps, temps2;
        temps = tempBoo
        splitString(keyword, temps, 1);
        while (!keyword.empty()) {
            keyword_cmd.addNode(Node(offset, temps));
            splitString(keyword, temps, 1);
        }

        string temps;
        splitString(keyword, temps, 1);
        while (!keyword.empty()) {
            keyword_cmd.addNode(Node(offset, temps));
            splitString(keyword, temps, 1);
        }
    }
}

void Bookstore::operation(string cmd) {
    //本函数保证调用其他函数时的参数为非空无空白字符字符串或正常整形变量
    string arg0, arg1, arg2, arg3;
    splitString(cmd, arg0);//提取出命令类型arg0

    //账户操作
    //---------------------------------su
    if (arg0 == "su") {
        splitString(cmd, arg1);
        splitString(cmd, arg2);
        splitString(cmd, arg0);//如果还有其余多余输入，则为无效命令
        if (!arg0.empty())
            user_cmd.su(arg1, arg2);
        else printf("Invalid\n");
    }
        //---------------------------------logout
    else if (arg0 == "logout") {
        splitString(cmd, arg0);
        if (!arg0.empty())
            user_cmd.logout();
        else printf("Invalid\n");
    }
        //---------------------------------useradd
    else if (arg0 == "useradd") {
        string arg4;
        splitString(cmd, arg1);
        splitString(cmd, arg2);
        splitString(cmd, arg3);
        splitString(cmd, arg4);

        splitString(cmd, arg0);
        if (!arg0.empty()) {
            try {//todo 显然第一次用try很容易出bug
                user_cmd.useradd(arg1, arg2, stoi(arg3), arg4);
            }
            catch (exception tempException) {
                printf("Invalid\n");
            }
        } else printf("Invalid\n");
    }
        //---------------------------------register
    else if (arg0 == "register") {
        splitString(cmd, arg1);
        splitString(cmd, arg2);
        splitString(cmd, arg3);

        splitString(cmd, arg0);
        if (!arg0.empty())
            user_cmd.reg(arg1, arg2, arg3);
        else printf("Invalid\n");
    }
        //---------------------------------delete
    else if (arg0 == "delete") {
        splitString(cmd, arg1);

        splitString(cmd, arg0);
        if (!arg0.empty())
            user_cmd.del(arg1);
        else printf("Invalid\n");
    }
        //---------------------------------passwd
    else if (arg0 == "passwd") {
        splitString(cmd, arg1);
        splitString(cmd, arg2);
        splitString(cmd, arg3);

        splitString(cmd, arg0);
        if (!arg0.empty())
            user_cmd.repwd(arg1, arg2, arg3);
        else printf("Invalid\n");
    }

        //书籍操作
        //---------------------------------select
    else if (arg0 == "select") {
        splitString(cmd, arg1);

        splitString(cmd, arg0);
        if (!arg0.empty())
            select(arg1);
        else printf("Invalid\n");
    }
        //---------------------------------modify
    else if (arg0 == "modify") {
        string modifyISBN, modifyName, modifyAuthor, modifyKeyword;
        double modifyPrice = -1;
        char invalidFlag = 0;
        splitString(cmd, arg1);
        while (!arg1.empty()) {
            if (arg1.substr(0, 6) == "-ISBN=") {
                if (modifyISBN.empty()) {//如果该参数重复出现则为无效命令
                    modifyISBN = arg1.substr(6, arg1.length() - 6);
                    if (modifyISBN == "")invalidFlag = 1;//如果该参数为空则为无效命令
                } else invalidFlag = 1;
            } else if (arg1.substr(0, 6) == "-name=") {
                if (modifyName.empty()) {
                    modifyName = arg1.substr(6, arg1.length() - 6);
                    if (modifyName == "")invalidFlag = 1;
                } else invalidFlag = 1;
            } else if (arg1.substr(0, 8) == "-author=") {
                if (modifyAuthor.empty()) {
                    modifyAuthor = arg1.substr(8, arg1.length() - 8);
                    if (modifyAuthor == "")invalidFlag = 1;
                } else invalidFlag = 1;
            } else if (arg1.substr(0, 9) == "-keyword=") {
                if (modifyKeyword.empty()) {
                    modifyKeyword = arg1.substr(9, arg1.length() - 9);
                    if (modifyKeyword == "")invalidFlag = 1;
                } else invalidFlag = 1;
            } else if (arg1.substr(0, 7) == "-price=") {
                if (modifyPrice == -1) {
                    try {
                        modifyPrice = stod(arg1);
                        if (modifyPrice < 0)invalidFlag = 1;
                    }
                    catch (exception tempException) {
                        invalidFlag = 1;//string转double失败
                    }
                } else invalidFlag = 1;
            } else invalidFlag = 1;
            if (invalidFlag == 1)break;
            splitString(cmd, arg1);
        }
        if (invalidFlag == 1)
            printf("Invalid\n");
        else modify(user_cmd.userSelect(), modifyISBN, modifyName, modifyAuthor, modifyKeyword, modifyPrice);
    }
        //---------------------------------import
    else if (arg0 == "import") {
        splitString(cmd, arg1);
        splitString(cmd, arg2);

        splitString(cmd, arg0);
        if (!arg0.empty()) {
            try {
                import(stoi(arg1), stod(arg2));
            }
            catch (exception tempException) {
                printf("Invalid\n");
            }
        } else printf("Invalid\n");
    }
        //---------------------------------show
    else if (arg0 == "show") {
        splitString(cmd, arg1);

        splitString(cmd, arg0);
        if (!arg0.empty()) {
            if (arg1.substr(0, 6) == "-ISBN=") {
                arg1 = arg1.substr(6, arg1.length() - 6);
                int temp = find(arg1);
                if (temp == -1)
                    printf("\n");//如果没有符合条件的书，输出一个空行
                else {
                    Book tempBook;
                    bookstoreFile_cmd.freadBook(temp, tempBook);
                    printBook(tempBook);//todo 类似这种函数考虑不值传递而是地址传递
                }
            } else {
                char invalidFlag = 0;
                vector<int> tempArray;
                tempArray.clear();
                if (arg1.substr(0, 6) == "-name=") {
                    arg1 = arg1.substr(6, arg1.length() - 6);
                    findplus(findName, arg1, tempArray);
                } else if (arg1.substr(0, 8) == "-author=") {
                    arg1 = arg1.substr(8, arg1.length() - 8);
                    findplus(findAuthor, arg1, tempArray);
                } else if (arg1.substr(0, 9) == "-keyword=") {
                    arg1 = arg1.substr(9, arg1.length() - 9);
                    findplus(findKeyword, arg1, tempArray);
                }
                    //不合法参数
                else {
                    printf("Invalid\n");
                    invalidFlag = 1;
                }
                if (!invalidFlag) {
                    if (!tempArray.empty()) {
                        //将tempArray中所有书籍，根据ISBN号排序并输出
                        int i;
                        vector<Book> tempBookArray;
                        tempBookArray.clear();
                        for (i = 0; i < tempArray.size(); ++i)
                            bookstoreFile_cmd.freadBook(tempArray[i], tempBookArray[i]);
                        sort(tempBookArray.begin(), tempBookArray.end());
                        for (i = 0; i < tempArray.size(); ++i)
                            printBook(tempBookArray[i]);
                    } else printf("\n");
                }
            }
        } else printf("Invalid\n");
    }
        //---------------------------------buy
    else if (arg0 == "buy") {
        splitString(cmd, arg1);
        splitString(cmd, arg2);

        splitString(cmd, arg0);
        if (!arg0.empty()) {
            try {
                buy(arg1, stoi(arg2));
            }
            catch (exception tempException) {
                printf("Invalid\n");
            }
        } else printf("Invalid\n");
    }

        //其他操作
        //---------------------------------report
    else if (arg0 == "report") {
        splitString(cmd, arg1);

        splitString(cmd, arg0);
        if (!arg0.empty()) {
            if (arg1 == "finance")
                showLog(reportFinance);
            else if (arg1 == "employee")
                showLog(reportEmployee);
            else if (arg1 == "myself")
                showLog(reportMyself);
            else
                printf("Invalid\n");
        } else printf("Invalid\n");
    }
        //---------------------------------log
    else if (arg0 == "log") {
        splitString(cmd, arg0);
        if (!arg0.empty()) {
            showLog(reportLog);
        } else printf("Invalid\n");
    }
        //不合法指令
    else printf("Invalid\n");
}


