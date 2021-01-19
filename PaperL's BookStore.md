# PaperL's Bookstore 软件开发文档

- Author: PaperL

- Version: 1.15



# Main`.cpp`

- 初始化`init()`

- 读入指令行然后调用函数`runCommand(string arg)`



# BookStore`.h/.cpp`

### 全局变量 

- *(存储于`BasicData.dat`)*
  
    | 变量内容                          | 变量类型                             | 备注         |
    | --------------------------------- | ------------------------------------ | ------------ |
    | 已登录账号及该账号选中的图书      | 以`vector<pair<user,ISBN> >`实现的栈 | 不保存至文件 |
    | 选中图书在`Data_Book.dat`中的地址 | 整型                                 | 不保存至文件 |
    | 总交易次数                        | 整型                                 |              |
    | 总支出                            | 浮点型                               |              |
    | 总收入                            | 浮点型                               |              |
    | 图书总数量                        | 整型                                 |              |

### 基础函数

- splitString(string &arg, string &ret, int keywordFlag = 0)
    - `arg`为待分割字符串，函数执行后删除首子字符串；`ret`为返回值，为首子字符串
    - `keywordFlag`决定分隔符是否为`'|'`
    - 该函数功能包括鲁棒性处理
- stringCheck(enum stringType, string arg)
  
    - 鲁棒性判断
- privilegeCheck(int restriction);
- **init**() 
    - 初始化函数 (非必须)

### 功能函数

- showFinance(int num)
    - 重载函数 showFinance()
- reportFinance()
- reportEmployee()
- log()
- reportMyself()

### 其他

- 除块状链表 *(`UnrolledLinkedList.h/.cpp`)* 内容，本文档中所有类、函数及枚举均在`BookStore.h`中声明
- *模板函数实现需在库文件 (`.h`) 中完成*



# CommandManager`.cpp`

- **command 指令类**
  
  - 指令参数	*(存储于`Data_Command.dat`)*
    | 变量内容            | 变量类型 |
    | ------------------- | -------- |
    | 指令类型            | 枚举     |
    | 指令参数            | 字符数组 |
    | 用户名              | 字符数组 |
    | 执行情况(成功/失败) | 布尔     |
    | 操作时间(time())    | 长长整型 |
  
- runCommand(string arg)

  - 对用户输入的指令进行处理并执行对应函数

- recordCommand(command arg)

  - 将指令存入`Data_Command.dat`文件



# UserManager`.cpp`

- **user 账号类**
    - 账号参数	*(`Data_User.dat`用)*
        | 变量名 | 变量类型 |
        | ---- | ---- |
        | userID | 字符数组 max length: 30 |
        | name | 字符数组 max length: 30 |
        | authority | 整形 |
        | password | 字符数组 max length: 30 |
- login(string userID, string password)
  
    - 重载函数 login(string userID)
- logout()
- userAdd(user t)
- register(user t)
- delete(int userID)
- changePassword(int userID, string oldPass, string newPass)
  
    - 重载函数 changePassword(int userID, string newPass)



# BookManager`.cpp`

- **book 图书类**
    - 图书参数	*(`Data_Book.dat`用)*
        | 变量名 | 变量类型 |
        | ---- | ---- |
        | ISBN | 字符数组 max length: 20 |
        | name | 字符数组 max length: 60 |
	| author | 字符数组 max length: 60 |
	| keyword | 字符数组 max length: 60 |
	| price | 浮点数 |
	| quantity | 整形 |
- select(string ISBN)
- modify(string arg)
- show(string arg)


- **entry 账本条目类**
    - 图书参数	*(`Data_Book.dat`用)*
        | 变量名 | 变量类型 |
        | ---- | ---- |
        | time    | 长长整型 |
        | userID | 字符数组 max length: 30 |
        | ISBN | 字符数组 max length: 20 |
	| quantity *(正数表示购买，负数表示进货)* | 整形 |
	| price | 浮点数 |
	| totalPrice | 浮点数 |
- import(int num, int price)
- buy(string ISBN, int num)



# UnrolledLinkedList`.h/.cpp`

- **unrolledLinkedList 块状链表类**
    - addElement(string arg, int pointer)
    - rewriteElement(string oldArg, string newArg, int pointer)
    - findElement(string arg)
    - deleteElement(string userID)
      - 该函数仅于删除账号时使用

<!--注意此处仅列出了public函数，private中内容参考: string fileName, nextBlock函数, deleteBlock函数, mergeBlock函数, splitBlock函数, hash函数等-->

<!--以下为块状链表内建议的数据类型-->

```c++
class Element {
public:
    int offset;
    char str[64];

    bool operator<(const Element &x) const;
    Element(const int &arg1 = -1, const string &arg2 = "");
    Element &operator=(const Element &right);
};

class Node {
public:
    int nxt;
    int pre;
    int num;
    Element array[ELEMENT_NUMBER];

    Node();
    Node &operator=(const Node &right);
};
```

# BasicFileManager`.cpp`

**以下函数使用模板函数实现**

读写`BasicData.dat`
- writeBasicData(enum 参数类型, T arg)
- readBasicData(enum 参数类型, T arg)

读写除`BasicData`外其他`Data_*.dat`文件
- writeData(enum 参数类型, T arg, int offset = -1)
  - `offset`为`-1`表示在文件末追加写入，否则在指定位置覆盖写入
- readData(enum 参数类型, int offset)



# 文件存储

**软件运行所需文件及其功能**

- **BasicData**.dat
  - 存储部分全局变量，见 **Bookstore** 部分
- **Data_Command**.dat
  - 按创建顺序存储`command`对象
- **Data_Finance**.dat
  - 按创建顺序存储`entry`对象
- **Data_User**.dat
  - 按创建顺序存储`user`对象
- **Data_Book**.dat
  - 按创建顺序存储`book`对象


- **Index_Userid**.dat
  - 以**块状链表**升序存储`userID`的**哈希值**及该账号于`Data_User.dat`中的位置
- **Index_ISBN**.dat
  - 以块状链表升序存储`ISBN`的哈希值及该图书数据于`Data_Book.dat`中的位置
- **Index_Author**.dat
  - 以块状链表升序存储`author`的哈希值及该图书数据于`Data_Book.dat`中的位置
- **Index_Name**.dat
  - 以块状链表升序存储`name`的哈希值及该图书数据于`Data_Book.dat`中的位置
- **Index_Keyword**.dat
  - 以块状链表升序存储`keyword`的哈希值及该图书数据于`Data_Book.dat`中的位置


# 补充

- 本开发文档未经实现，如有疏漏错误之处还请谅解
- 最后更新于`2021.1.19`
