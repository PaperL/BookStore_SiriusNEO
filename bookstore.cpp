//
// Created by PaperL on 2020/11/9.
//

#include "bookstore.h"

Book::Book() {
#ifdef PaperL_Debug
    cout << "In Constructor \"Book\" :" << endl;
#endif
    memset(ISBN, 0, sizeof(ISBN));
    memset(name, 0, sizeof(name));
    memset(author, 0, sizeof(author));
    memset(keyword, 0, sizeof(keyword));
    price = -1;
    quantity = 0;
}

Book &Book::operator=(const Book &right) {
    if (this == &right)
        return *this;
    strcpy(ISBN, right.ISBN);
    strcpy(name, right.name);
    strcpy(author, right.author);
    strcpy(keyword, right.keyword);
    price = right.price;
    quantity = right.quantity;
    return *this;
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
#ifdef PaperL_Debug
    cout << "In Constructor \"BookstoreFileManager\" :" << endl;
#endif

    //检查book.dat
    fi.open(fnameBook, ios::in | ios::binary);
    if (!fi.is_open()) {
        fi.close();
#ifdef PaperL_Debug
        cout << "Discover index file \"" << fnameBook << "\" is missing." << endl;
        cout << "Creating blank file \"" << fnameBook << "\" ..." << endl;
#endif
        fi.open(fnameBook, ios::out | ios::trunc | ios::binary);
        //book.dat开头应写入图书总数(int)
        int tempInt = 0;
        fi.seekp(0, ios::beg);
        fi.write(reinterpret_cast<char *>(&tempInt), sizeof(int));
    }
    fi.close();

    //检查finance.dat
    fi.open(fnameFinance, ios::in | ios::binary);
    if (!fi.is_open()) {
        fi.close();
#ifdef PaperL_Debug
        cout << "Discover index file \"" << fnameFinance << "\" is missing." << endl;
        cout << "Creating blank file \"" << fnameFinance << "\" ..." << endl;
#endif
        fi.open(fnameFinance, ios::out | ios::trunc | ios::binary);
        //finance.dat开头应写入交易次数(int),总收入(double),总支出(double)
        int tempInt = 0;
        double tempDouble = 0;
        fi.seekp(0, ios::beg);
        fi.write(reinterpret_cast<char *>(&tempInt), sizeof(int));
        fi.write(reinterpret_cast<char *>(&tempDouble), sizeof(double));
        fi.write(reinterpret_cast<char *>(&tempDouble), sizeof(double));
    }
    fi.close();

    //检查log.dat
    fi.open(fnameLog, ios::in | ios::binary);
    if (!fi.is_open()) {
        fi.close();
#ifdef PaperL_Debug
        cout << "Discover index file \"" << fnameLog << "\" is missing." << endl;
        cout << "Creating blank file \"" << fnameLog << "\" ..." << endl;
#endif
        fi.open(fnameLog, ios::out | ios::trunc | ios::binary);
    }
    fi.close();
}

inline void Bookstore::BookstoreFileManager::financeInit(int &tradeNum, double &income, double &outgo) {
#ifdef PaperL_Debug
    cout << "In Function \"financeInit\":" << endl;
#endif
    //获取finance.dat开头数据tradeNumber,totIncome,totExpense
    fi.open(fnameFinance, ios::in | ios::binary);
    fi.seekg(0, ios::beg);
    fi.read(reinterpret_cast<char *>(&tradeNum), sizeof(int));
    fi.read(reinterpret_cast<char *>(&income), sizeof(double));
    fi.read(reinterpret_cast<char *>(&outgo), sizeof(double));
    fi.close();
}

inline void Bookstore::BookstoreFileManager::financeBasicWrite(int &num, double &income, double &outgo) {
#ifdef PaperL_Debug
    cout << "In Function \"financeBasicWrite\":" << endl;
#endif
    //写入finance.dat开头数据tradeNumber,totIncome,totExpense
    fo.open(fnameFinance, ios::in | ios::out | ios::binary);
    fo.seekp(0, ios::beg);
    fo.write(reinterpret_cast<char *>(&num), sizeof(int));
    fo.write(reinterpret_cast<char *>(&income), sizeof(double));
    fo.write(reinterpret_cast<char *>(&outgo), sizeof(double));
    fo.close();
}

inline void Bookstore::BookstoreFileManager::financeWrite(double &price, bool &sgn) {
#ifdef PaperL_Debug
    cout << "In Function \"financeWrite\":" << endl;
#endif
    //在finance.dat末写入一笔交易
    fo.open(fnameFinance, ios::in | ios::out | ios::binary);
    fo.seekp(0, ios::end);
    fo.write(reinterpret_cast<char *>(&price), sizeof(double));
    fo.write(reinterpret_cast<char *>(&sgn), sizeof(bool));
    fo.close();
}

inline void Bookstore::BookstoreFileManager::bookInit(int &bookNum) {
#ifdef PaperL_Debug
    cout << "In Function \"bookInit\":" << endl;
#endif
    //获取book.dat开头数据bookNumber
    fi.open(fnameBook, ios::in | ios::binary);
    fi.seekg(0, ios::beg);
    fi.read(reinterpret_cast<char *>(&bookNum), sizeof(int));
    fi.close();
}

inline void Bookstore::BookstoreFileManager::bookBasicWrite(int &bookNum) {
#ifdef PaperL_Debug
    cout << "In Function \"bookBasicWrite\":" << endl;
#endif
    //写入book.dat开头数据bookNumber
    fo.open(fnameBook, ios::in | ios::out | ios::binary);
    fo.seekp(0, ios::beg);
    fo.write(reinterpret_cast<char *>(&bookNum), sizeof(int));
    fo.close();
}

inline void Bookstore::BookstoreFileManager::freadBook(int offset, Book &arg) {
#ifdef PaperL_Debug
    cout << "In Function \"freadBook\": offset = " << offset << endl;
#endif
    fi.open(fnameBook, ios::in | ios::binary);
    fi.seekg(offset, ios::beg);
    fi.read(reinterpret_cast<char *>(&arg), sizeof(Book));
    fi.close();
}

inline void Bookstore::BookstoreFileManager::freadBook(vector<Book> &array) {
#ifdef PaperL_Debug
    cout << "In Function \"freadBook\" Plus:" << endl;
#endif
    fi.open(fnameBook, ios::in | ios::binary);
    fi.seekg(0, ios::beg);
    int temp;
    Book tempBook;
    fi.read(reinterpret_cast<char *>(&temp), sizeof(int));
    for (int i = 0; i < temp; ++i) {
        fi.read(reinterpret_cast<char *>(&tempBook), sizeof(Book));
        array.push_back(tempBook);
    }
    fi.close();
}

inline int Bookstore::BookstoreFileManager::fwriteBook(Book &arg) {
#ifdef PaperL_Debug
    cout << "In Function \"fwriteBook\" New Book:" << endl;
#endif
    int temp;
    fo.open(fnameBook, ios::in | ios::out | ios::binary);
    fo.seekp(0, ios::end);
    temp = fo.tellp();
    fo.write(reinterpret_cast<char *>(&arg), sizeof(Book));
    fo.close();
    return temp;
}

inline void Bookstore::BookstoreFileManager::fwriteBook(int offset, Book &arg) {
#ifdef PaperL_Debug
    cout << "In Function \"fwriteBook\" modify book:" << endl;
#endif
    fo.open(fnameBook, ios::in | ios::out | ios::binary);
    fo.seekp(offset, ios::beg);
    fo.write(reinterpret_cast<char *>(&arg), sizeof(Book));
    fo.close();
}



//================================
//========== Bookstore ===========
//================================

Bookstore::Bookstore() : isbn_cmd("isbn.bin"), name_cmd("name.bin"), author_cmd("author.bin"),
                         keyword_cmd("keyword.bin") {
#ifdef PaperL_Debug
    cout << "In Constructor \"Bookstore\" :" << endl;
#endif
    bookNumber = 0;
    bookstoreFile_cmd.financeInit(tradeNumber, totIncome, totExpense);//todo 这里能不能初始化？
    bookstoreFile_cmd.bookInit(bookNumber);
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
        } else arg.clear();//整个arg都为空白字符
    } else {//拆分keyword字符串的情况,以'|'为分隔符
        //理论上此时arg已经没有首尾空白符了
        while ((arg[p1] != '|') && p1 < arg.length())++p1;
        if (p1 < arg.length()) {//本为(p1 < arg.length())但依然不能检查出 "a|" 这种非法字符串
            ret = arg.substr(0, p1);
            try {//substr(pos,n) 当 pos > this.size 时抛出异常 out of range
                //p1 + 1跳过'|'
                arg = arg.substr(p1 + 1, arg.length() - p1 - 1);
            }
            catch (exception tempException) {
                arg.clear();
            }
        } else {
            ret = arg;
            arg.clear();
        }
    }
#ifdef PaperL_Debug
    //cout << "    Split : get \"" << ret << "\", left \"" << arg << "\"" << endl;
#endif
}

inline bool
Bookstore::bookStringCheck(bookStringTypeEnum bookStringType, const string &arg) {//todo const引用是个好东西,最好能用的都用上
#ifdef PaperL_Debug
    cout << "    bookStringCheck ";
    if (bookStringType == stringISBN) {
        cout << "\"ISBN\"" << endl;
    } else if (bookStringType == stringBookName || bookStringType == stringAuthor) {
        cout << "\"Name\" or \"Author\"" << endl;
    } else {//bookStringType == stringKeyword
        cout << "\"Keyword\"" << endl;
    }
#endif
    if (!arg.empty()) {//视空arg为合法
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
                if (arg[i] == '\"')//不能包含双引号
                    return false;
            }
        } else {//bookStringType == stringKeyword
            //此处只判断单个keyword,即'|'为非法字符
            if (arg.length() > 62)return false;
            if (arg[0] != '\"' || arg[arg.length() - 1] != '\"')return false;
            for (int i = 1; i < arg.length() - 1; ++i) {
                if (arg[i] == '|' || arg[i] == '\"')
                    return false;
            }
        }
    }
#ifdef PaperL_Debug
    cout << "    bookStringCheck ";
    if (bookStringType == stringISBN) {
        cout << "\"ISBN\" succeed" << endl;
    } else if (bookStringType == stringBookName || bookStringType == stringAuthor) {
        cout << "\"Name\" or \"Author\" succeed" << endl;
    } else {//bookStringType == stringKeyword
        cout << "\"Keyword\" succeed" << endl;
    }
#endif
    return true;
}

//inline Book Bookstore::freadBook(int offset) {}

inline void Bookstore::printBook(const Book &arg) {
    if (arg.price == -1)//这个特判问就是题面不合理
        printf("%s\t%s\t%s\t%s\t0.00\t%d\n", arg.ISBN, arg.name, arg.author, arg.keyword, arg.quantity);
    else
        printf("%s\t%s\t%s\t%s\t%.2lf\t%d\n", arg.ISBN, arg.name, arg.author, arg.keyword, arg.price, arg.quantity);
}

void Bookstore::showLog(logTypeEnum logType) {
    //todo 待实现
    if (logType == reportLog) {

    }
}

void Bookstore::addFinance(double price, bool sgn) {//sgn:true支出,false收入
#ifdef PaperL_Debug
    cout << "In Function \"addFinance\":" << endl;
#endif
    ++tradeNumber;
    if (sgn)totExpense += price;
    else totIncome += price;
    bookstoreFile_cmd.financeWrite(price, sgn);
    bookstoreFile_cmd.financeBasicWrite(tradeNumber, totIncome, totExpense);
}

void Bookstore::showFinance(int time) {
    if (!user_cmd.privilegeCheck(7)) {//没有足够权限(3)
        printf("Invalid\n");
        return;
    }//todo 待实现
    if (time == -1) {//输出总收入支出

    } else {//输出最近time次收入支出

    }
}

void Bookstore::import(int quantity, double price) {
#ifdef PaperL_Debug
    cout << "In Function \"import\":" << endl;
#endif
    //select为-1时不能进行modify和import操作
    //在su时已经保证初始select为-1
    //import的price是总价而非单价
    if (user_cmd.userSelect() == -1 || !user_cmd.privilegeCheck(3)//未选中Book或没有足够权限(3)
        || quantity < 0 || price < 0) {
        printf("Invalid\n");
        return;
    }
    addFinance(price, true);//支出
    //修改Book.quantity
    Book tempBook;
    bookstoreFile_cmd.freadBook(user_cmd.userSelect(), tempBook);
    tempBook.quantity += quantity;
    bookstoreFile_cmd.fwriteBook(user_cmd.userSelect(), tempBook);
}

void Bookstore::buy(const string &ISBN, const int &quantity) {
    //buy指令输出购买花费，保留两位小数
    if (ISBN.empty() || quantity < 0 || !user_cmd.privilegeCheck(1)) {//购买书数为负数或没有足够权限(1)
        printf("Invalid\n");
        return;
    }
    int temp = find(ISBN);
    if (temp != -1) {
        //修改Book.quantity
        Book tempBook;
        bookstoreFile_cmd.freadBook(user_cmd.userSelect(), tempBook);
        if (tempBook.quantity < quantity) {//库存不够
            printf("Invalid\n");
            return;
        }
        if(tempBook.price!=-1) {//todo 为什么标程默认价格0???
            addFinance(quantity * tempBook.price, false);
            tempBook.quantity -= quantity;
            bookstoreFile_cmd.fwriteBook(user_cmd.userSelect(), tempBook);
            printf("%.2lf\n", tempBook.price * quantity);//输出价格
        }else{
            addFinance(0, false);
            tempBook.quantity -= quantity;
            bookstoreFile_cmd.fwriteBook(user_cmd.userSelect(), tempBook);
            printf("0.00\n");//输出价格
        }
    } else printf("Invalid\n");
}

int Bookstore::find(const string &ISBN) {
#ifdef PaperL_Debug
    cout << "In Function \"find\":" << endl;
#endif
    vector<int> tempVec;
    tempVec.clear();
    isbn_cmd.findNode(ISBN, tempVec);//如果这边有多个Node就出bug了,但愿不会
#ifdef PaperL_Debug
    if (tempVec.size() > 1)
        printf("Bookstore::find Error! 索引中有ISBN相同的Book");
#endif
#ifdef PaperL_Debug
    if (tempVec.empty())
        cout << "\"find\" no result" << endl;
    else
        cout << "\"find\" succeed" << endl;
#endif
    if (!tempVec.empty()) return tempVec[0];
    else return -1;
}

void Bookstore::findplus(findTypeEnum findType, const string &key, vector<int> &array) {
#ifdef PaperL_Debug
    cout << "In Function \"findplus\": string \"" << key << "\"" << endl;
#endif
    if (findType == findName) {
        name_cmd.findNode(key, array);
    } else if (findType == findAuthor) {
        author_cmd.findNode(key, array);
    } else {//findType == findKeyword
        keyword_cmd.findNode(key, array);
    }
}

void Bookstore::select(const string &ISBN) {
#ifdef PaperL_Debug
    cout << "In Function \"select\":" << endl;
#endif
    if (!user_cmd.privilegeCheck(3)
        || ISBN.empty() || !bookStringCheck(stringISBN, ISBN)) {//没有足够权限(7)
        printf("Invalid\n");
        return;
    }
    Book tempBook;
    int temp = find(ISBN);
    if (temp == -1) {//没有此书则新建空白书
        strcpy(tempBook.ISBN, ISBN.c_str());
        temp = bookstoreFile_cmd.fwriteBook(tempBook);
        user_cmd.changeSelect(temp);
        isbn_cmd.addNode(Node(temp, ISBN));
        bookstoreFile_cmd.bookBasicWrite(++bookNumber);
    } else user_cmd.changeSelect(temp);//有则选中
}

void Bookstore::modify(const int &offset, const string &ISBN, string &name,
                       string &author, string keyword, const double &price) {
#ifdef PaperL_Debug
    cout << "In Function \"modify\":" << endl;
#endif
    //offset = user_cmd.userSelect() 已在operation中保证不为 -1
    //price在operation中已保证非负，-1表无值
    if (// !user_cmd.privilegeCheck(3)//没有足够权限(3)
        !bookStringCheck(stringISBN, ISBN) || !bookStringCheck(stringBookName, name)
        || !bookStringCheck(stringAuthor, author)
        || (!ISBN.empty() && find(ISBN) != -1)) {//如果企图改为已存在的ISBN则非法
#ifdef PaperL_Debug
        if (!ISBN.empty() && find(ISBN) == -1)
            cout << "    find ISBN fail" << endl;
#endif
        printf("Invalid\n");
        return;
    }

    Book tempBook;//覆盖改写Book内容
    bookstoreFile_cmd.freadBook(offset, tempBook);
    if (!ISBN.empty()) {
#ifdef PaperL_Debug
        cout << "  \"ISBN\":" << endl;
#endif
        isbn_cmd.deleteNode(Node(offset, tempBook.ISBN));
        isbn_cmd.addNode(Node(offset, ISBN));
        strcpy(tempBook.ISBN, ISBN.c_str());
    }
    if (!name.empty()) {
#ifdef PaperL_Debug
        cout << "  \"name\":" << endl;
#endif
        name = name.substr(1, name.size() - 2);//去除首末双引号
        name_cmd.deleteNode(Node(offset, tempBook.name));
        name_cmd.addNode(Node(offset, name));
        strcpy(tempBook.name, name.c_str());
    }
    if (!author.empty()) {
#ifdef PaperL_Debug
        cout << "  \"author\":" << endl;
#endif
        author = author.substr(1, author.size() - 2);//去除首末双引号
        author_cmd.deleteNode(Node(offset, tempBook.author));
        author_cmd.addNode(Node(offset, author));
        strcpy(tempBook.author, author.c_str());
    }
    if (!keyword.empty()) {
#ifdef PaperL_Debug
        cout << "  \"keyword\":" << endl;
#endif
        //去除首末双引号
        keyword = keyword.substr(1, keyword.size() - 2);
        strcpy(tempBook.keyword, keyword.c_str());

        string temps, temps2;
        temps2 = tempBook.keyword;//先把旧keyword索引内容删除
        splitString(temps2, temps, 1);
        while (!temps.empty()) {
            keyword_cmd.deleteNode(Node(offset, temps));
            splitString(temps2, temps, 1);
        }
        //加入新Keyword索引
        splitString(keyword, temps, 1);
        while (!temps.empty()) {
            keyword_cmd.addNode(Node(offset, temps));
            splitString(keyword, temps, 1);
        }
    }
    if (price != -1) tempBook.price = price;

    //写入文件
    bookstoreFile_cmd.fwriteBook(offset, tempBook);
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

        if (arg0.empty())
            user_cmd.su(arg1, arg2);
        else printf("Invalid\n");
    }
        //---------------------------------logout
    else if (arg0 == "logout") {
        splitString(cmd, arg0);
        if (arg0.empty())
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
        if (arg0.empty()) {
            try {//todo stoi的输入为空白字符串时不会报错，合法数字后跟非法字符也不会报错，最好还是自己写吧
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
        if (arg0.empty())
            user_cmd.reg(arg1, arg2, arg3);
        else printf("Invalid\n");
    }
        //---------------------------------delete
    else if (arg0 == "delete") {
        splitString(cmd, arg1);

        splitString(cmd, arg0);
        if (arg0.empty())
            user_cmd.del(arg1);
        else printf("Invalid\n");
    }
        //---------------------------------passwd
    else if (arg0 == "passwd") {
        splitString(cmd, arg1);
        splitString(cmd, arg2);
        splitString(cmd, arg3);

        splitString(cmd, arg0);
        if (arg0.empty())
            user_cmd.repwd(arg1, arg2, arg3);
        else printf("Invalid\n");
    }

        //书籍操作
        //---------------------------------select
    else if (arg0 == "select") {
        splitString(cmd, arg1);

        splitString(cmd, arg0);
        if (arg0.empty())
            select(arg1);
        else printf("Invalid\n");
    }
        //---------------------------------modify
    else if (arg0 == "modify") {
        if (user_cmd.userSelect() != -1 && user_cmd.privilegeCheck(3)) {
            string modifyISBN, modifyName, modifyAuthor, modifyKeyword;
            double modifyPrice = -1;
            char invalidFlag = 0;
            splitString(cmd, arg1);
            if (arg1.empty())invalidFlag = 1;//modify指令后没有任何参数则为非法指令
            while (!arg1.empty()) {
                if (arg1.substr(0, 6) == "-ISBN=") {
                    if (modifyISBN.empty()) {//参数只能赋值一次，重复出现则为无效命令
                        modifyISBN = arg1.substr(6, arg1.length() - 6);
                        if (modifyISBN.empty())invalidFlag = 1;//如果该参数为空则为无效命令
                    } else invalidFlag = 1;
                } else if (arg1.substr(0, 6) == "-name=") {
                    if (modifyName.empty()) {
                        modifyName = arg1.substr(6, arg1.length() - 6);
                        if (modifyName.empty())invalidFlag = 1;
                    } else invalidFlag = 1;
                } else if (arg1.substr(0, 8) == "-author=") {
                    if (modifyAuthor.empty()) {
                        modifyAuthor = arg1.substr(8, arg1.length() - 8);
                        if (modifyAuthor.empty())invalidFlag = 1;
                    } else invalidFlag = 1;
                } else if (arg1.substr(0, 9) == "-keyword=") {
                    if (modifyKeyword.empty()) {
                        modifyKeyword = arg1.substr(9, arg1.length() - 9);
                        if (modifyKeyword.empty())invalidFlag = 1;
                    } else invalidFlag = 1;
                } else if (arg1.substr(0, 7) == "-price=") {
                    if (modifyPrice == -1) {
                        arg1 = arg1.substr(7, arg1.length() - 7);
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
        } else printf("Invalid\n");
    }
        //---------------------------------import
    else if (arg0 == "import") {
        splitString(cmd, arg1);
        splitString(cmd, arg2);

        splitString(cmd, arg0);
        if (arg0.empty()) {
            try {//todo 同useradd.todo
                import(stoi(arg1), stod(arg2));
            }
            catch (exception tempException) {
                printf("Invalid\n");
            }
        } else printf("Invalid\n");
    }
        //---------------------------------show
    else if (arg0 == "show") {
        if (!cmd.empty()) {//带参数show指令
            splitString(cmd, arg1);
            //show finance
            if (arg1.substr(0, 7) == "finance") {
                splitString(cmd, arg2);
                if (!arg2.empty()) {//show finance [-time] 指令
                    splitString(cmd, arg0);
                    if (arg0.empty()) {
                        try {
                            int temp = stoi(arg2);
                            if (temp >= 0)
                                showFinance(stoi(arg2));
                            else//time参数小于零则非法
                                printf("Invalid\n");
                        }
                        catch (exception tempException) {
                            printf("Invalid\n");
                        }
                    } else printf("Invalid\n");//show finance [-time] 后还有多余内容则非法
                } else {//show finance 指令
                    showFinance();
                }
            }
                // Book单关键词show
            else if (user_cmd.privilegeCheck(1)) {
                if (arg1.substr(0, 6) == "-ISBN=") {
                    arg1 = arg1.substr(6, arg1.length() - 6);
                    if (!arg1.empty() && bookStringCheck(stringISBN, arg1)) {
                        int temp = find(arg1);
                        if (temp == -1)
                            printf("\n");//如果没有符合条件的书，输出一个空行
                        else {
                            Book tempBook;
                            bookstoreFile_cmd.freadBook(temp, tempBook);
                            printBook(tempBook);
                        }
                    } else printf("Invalid\n");//非法ISBN参数
                } else {
                    char invalidFlag = 0;
                    vector<int> tempArray;
                    tempArray.clear();
                    if (arg1.substr(0, 6) == "-name=") {
                        arg1 = arg1.substr(6, arg1.length() - 6);
                        if (!arg1.empty() && bookStringCheck(stringBookName, arg1))
                            findplus(findName, arg1.substr(1, arg1.size() - 2), tempArray);//arg1.substr()去双引号
                        else invalidFlag = 1;
                    } else if (arg1.substr(0, 8) == "-author=") {
                        arg1 = arg1.substr(8, arg1.length() - 8);
                        if (!arg1.empty() && bookStringCheck(stringAuthor, arg1))
                            findplus(findAuthor, arg1.substr(1, arg1.size() - 2), tempArray);
                        else invalidFlag = 1;
                    } else if (arg1.substr(0, 9) == "-keyword=") {
                        arg1 = arg1.substr(9, arg1.length() - 9);
                        if (!arg1.empty() && bookStringCheck(stringKeyword, arg1))
                            findplus(findKeyword, arg1.substr(1, arg1.size() - 2), tempArray);
                        else invalidFlag = 1;
                    }
                        //show命令参数不为任何合法参数
                    else {
                        printf("Invalid\n");
                        invalidFlag = 1;
                    }

                    if (invalidFlag == 0) {
                        if (!tempArray.empty()) {
                            //将tempArray中所有书籍，根据ISBN号排序并输出
                            int i;
                            vector<Book> tempBookArray;
                            Book tempBook;
                            tempBookArray.clear();
                            for (i = 0; i < tempArray.size(); ++i) {
                                bookstoreFile_cmd.freadBook(tempArray[i], tempBook);
                                tempBookArray.push_back(tempBook);
                            }
                            sort(tempBookArray.begin(), tempBookArray.end());
                            for (i = 0; i < tempArray.size(); ++i)
                                printBook(tempBookArray[i]);
                        } else printf("\n");
                    } else printf("Invalid\n");
                }
            } else printf("Invalid\n");//show命令带非finance的参数且权限不足(1)
        } else {
            //按ISBN号升序输出所有书
            if (user_cmd.privilegeCheck(1)) {
                vector<Book> tempBookArray;
                tempBookArray.clear();
                bookstoreFile_cmd.freadBook(tempBookArray);
                sort(tempBookArray.begin(), tempBookArray.end());
                for (int i = 0; i < tempBookArray.size(); ++i)
                    printBook(tempBookArray[i]);
            } else
                printf("Invalid\n");
        }
    }
        //---------------------------------buy
    else if (arg0 == "buy") {
        splitString(cmd, arg1);
        splitString(cmd, arg2);

        splitString(cmd, arg0);
        if (arg0.empty()) {
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
        if (arg0.empty()) {
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
        if (arg0.empty()) {
            showLog(reportLog);
        } else printf("Invalid\n");
    }
        //不合法指令
    else printf("Invalid\n");
}


