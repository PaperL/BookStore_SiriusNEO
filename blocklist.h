//
// Created by PaperL on 2020/11/9.
//
#ifndef BOOKSTORE_SIRIUSNEO_BLOCKLIST_H
#define BOOKSTORE_SIRIUSNEO_BLOCKLIST_H

#include <cstdio>
#include <iostream>
#include <fstream>

#include <algorithm>

#include <vector>
#include <string>
#include <string.h>

#define BLOCK_SIZE 320
#define BLOCK_SPLIT_THRESHOLD 310
#define BLOCK_SPLIT_LEFT 100
#define BLOCK_MERGE_THRESHOLD 50

//#define PaperL_Debug

using namespace std;

class Node {
public:
    int offset;
    char str[64];

    bool operator<(const Node &x) const;

    Node();

    Node(const int &arg1, const string &arg2);

    Node &operator=(const Node &right);
};

class Block {
public:
    int nxt;
    int pre;
    int num;
    Node array[BLOCK_SIZE];

    Block();

    Block &operator=(const Block &right);
};

class blocklist {
private:
    const string fname;
    fstream fi, fo, fi2, fo2, fip, fip2, fop, fop2;

    inline int nextBlock(const int &offset);

    inline void delBlock(const int &offset);

    void mergeBlock(const int &offset1, const int &offset2);

    void splitBlock(const int &offset);

public:
    blocklist(const string &arg);

    ~blocklist();

    void findNode(const string &key, vector<int> &array);

    void addNode(const Node &node);

    int deleteNode(const Node &node);

#ifdef PaperL_Debug
    void debugPrint();
#endif

};


#endif //BOOKSTORE_SIRIUSNEO_BLOCKLIST_H
