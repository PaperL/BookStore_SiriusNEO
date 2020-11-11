# 图书管理系统 - 开发文档 #



### 0 关于本文档 ###

**作者:**  Lin Chaofan/SiriusNEO

**当前状态:** 或许已完成  ~~咕咕咕咕咕咕咕咕~~



**简介:** SJTU 程序设计 A 班的第一次大作业 图书管理系统的设计文档

​		  ~~萌新的第一篇设计文档，正在读这个文档的可怜孩子还请多多海涵~~

**个人认为的难点：** 1. 对书本的show命令，需要维护数据结构进行快速查找

​       						 2. 文件IO



**UPD:** 2020/10/13 由于要求发生变化，删除了原来的 BookManager 类，相关操作合并至 BookStore 类

***



### 1 代码文件结构

本条目文件名后面的**（数字）**表示该文件在结构中的等级，调用关系为  **4 -> 3 -> 2 -> 1**

本project要求一定程度的OOP，所以采用如下文件结构，请将**声明**写在 .h 文件，**实现**写在 .cpp 文件

- **main.cpp**（1）

  主函数, 需要实现命令字符串的读入、加工

  **注意：新增quit要求，直接在main里面实现即可**

- **bookstore.h and bookstore.cpp**（2）

  一个书店，包含：

  1. 一个 UserManager
  2. 书本的相关操作

  3. 财务、操作日志等书店管理功能

- **usermanager.h and usermanager.cpp**（3）

  实现本系统账户管理

- **blocklist.h and blocklist.cpp**（4）

  实现一个块状链表用于索引


***



### 2 算法思路

该project总体上不需要什么高级算法，这里重点讲一下数据结构部分的相关算法

- show 的命令需要支持通过 ISBN、name、author、keyword 找到对应的书

  书是统一线性存储在数据文件中的，通过索引得到某本书在文件中的 offset 来访问它

  所以我们维护**四个索引**分别支持以上四种查找，其中的元素为 node<str, offset>，str 是以上四种字符串

  （keyword要拆开，比如对于 China|Jiang，要分成 China 和 Jiang 两个字符串）

  采用块状链表维护一个按 str 比大小单调的序列，这样查找复杂度就是 O(sqrt n)

  **注意：keyword 有多个，这意味着 modify 后需要对每个关键字都添加一个 node**

- 实现完查找后考虑 show，我的想法是按照 ISBN、name、author、keyword 的顺序，

  如果提供了某个参数（比如没给 ISBN，但给了其它三个），就按优先级最高的查找（ISBN绝对不重复，name很大概率不重复，author 和 keyword 不好说）出一些书（如先找出 name 符合条件的书）

  然后直接暴力 for 这些书看看剩下的参数符不符合

- 此外，对于用户系统，我们也要维护一个用 user_id 进行查找的索引

- 对日志的查找要求比较简单，输出近time次直接从文件末倒上去time个位置就好，因此不需维护索引，

  直接存在数据文件里就ok，report特定操作人员的操作直接 for 一遍判断

***



### 3 类

本条目的分类按照  **代码文件 -> 类名 -> 成员变量、函数名** 分级

**注：** 字符串类型变量统一用 string 类型，但是 string 存在 size 不确定的问题，所以在文件操作过程中

​		要类型转化成 char*

每个操作都要注意权限的相关判断

#### a. bookstore

- BookStore 类

  ```c++
  UserManager usercmd;
  
  //操作与日志
  operation(string cmd);  //所有操作整合起来的函数，参数cmd表示操作名
  					    //对当前用户的权限进行判断，过低则输出 Invalid
                          //操作后记录到操作日志中
  						//注意在这里对 modify 和 show 的特殊处进行处理
  showLog(int type);     //类型分别对应 report myself | report employee | log   

  //金钱相关
  
  addFinance(double price, bool sgn); //finance.dat第一个int储存交易次数，后两个double储存收入、支出总量
  		      //每次储存时更新次数、收入支出，同时在文档末尾添加本次交易（price代表金额，sgn代表符号）
  showFinance(int time); //省略time：输出收入、支出总量  不省略：输出近time次交易
  import(int quantity, double price);
  buy(string ISBN, int quantity);   //用户买书，注意减少相应书的存量以及输出购买花费  
  
  //书本管理相关
  int bookNumber;
  find(string ISBN);     //用ISBN作为索引返回对应的书在数据文件中的offset
  findplus(int type, string key); //type:操作类型，1~3代表name author keyword，key是查询的信息字符串  
                                  //不同于ISBN，由于可以一对多，需要返回一系列符合条件offset
  							    //show借由以上两命令在operation暴力实现，具体见算法思路
  select(string ISBN);   //find到后赋值给最外层登录的User.curBook
  modify(int offset, string ISBN, string name, string author, string keyword, double price);
  						 //修改数据文件，对于每个要修改的信息，先用修改前的信息（如果有的话）找到并在索引中懒惰删除这些                            node，再往往索引文件中添加对应新node
  ```
  
- Book 类

  ```c++
  string ISBN, name, author;
  double price;
  int quantity;              //存货，默认值为0
  ```



#### b. usermanager

- UserManager 类

  ```c++
  int userNumber;
  
  vector<User> userStack;  //当前登录的用户栈，记录在文件nowusers.dat
  						 //每次启动程序把nowusers.dat的内容全部存进该vector中
  
  UserManager(); //构造函数 注意初始化手动创建一个root用户
  su(string id, string passwd);   //查找对应id的user，check passwd，登录成功将该用户pushback到当前登录用户
                                  //stack中，并且更新nowusers.dat
  logout();                       //弹出stack中的当前用户，并更新nowusers.dat
  
  reg(string id, string passwd, string name);  
  useradd(string id, string passwd, int privilige, string name); 
  
  repwd(string id, string newpwd, string oldpwd);  //oldpwd设一个默认值，如果当前为root允许通过
  
  del(string id);                //查找对应id的node，打标记
  ```

- User 类

  ```C++
  int privilige;    // 7 3 1
  int curBook;      //该用户选中书的offset，默认值设置成一个非法值方便判Invalid
  string id, passwd, name; 
  ```



#### c.  blocklist

​    维护块状链表用于索引

- Node 类

  ```c++
  string str;
  int offset;
  bool isdel;   // ==true 表示删除
  operation <   //定义小于号为 Node1.str < Node2.str
  ```

- Block 类

  ```c++
  Node array[SIZE]; //每块的大小根据数据范围预先定好
  int pre, nxt; //链表前驱和后继的offset
  Node minn;        //维护该块中最小元素便于插入
  ```

- BlockList 类

  ```c++
  string fname;  //索引文件名
  Block();   //初始化一个空块
  delBlock(int offset);   //删除一个块，操作方法类比链表（pre的nxt指向nxt，nxt的pre指向pre）
  mergeBlock(int offset1, int offset2); //块与块的合并，合并方法：将一个块所有元素暴力加入另一个块，然后删掉该块
  splitBlock(int offset, int index); //从某位置分裂，分裂方法：新建块，将该位置后所有元素暴力塞到新块
  findNode(string key);   //通过str找到对应的node，返回一个vector（因为可能str相同）
  addNode(Node node);     //插入某元素，方法：暴力或lower_bound找到该元素应该属于哪块，然后暴力插入
                          //删掉一个node直接给它打标记吧
  ```

***



### 4 文件

#### a. 数据文件

- **books.dat**

  储存书本信息

- **users.dat**

  储存用户信息

- **nowusers.dat** 

  所有目前登录的用户

- **finance.dat** 

  储存收入支出情况

- **log.dat** 

  储存操作日志

#### b. 索引文件

- **isbn.bin**

- **name.bin**

- **author.bin**

- **keyword.bin**

- **id.bin**

  通过id查找用户

***



### 5 提升

块状链表类可以用 B+ 树代替，但我不会（