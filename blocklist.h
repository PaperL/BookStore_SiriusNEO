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

#define PaperL_Debug

#define SIZE 320

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
    Node array[SIZE];

    Block();

    Block &operator=(const Block &right);
};

class blocklist {
private:
    const string fname;
    fstream fi, fo, fi2, fo2, fip;

    inline int nextBlock(int offset);

    void delBlock(int offset);

    void mergeBlock(int offset1, int offset2);

    void splitBlock(int offset, int leftNum);

public:
    blocklist(const string &arg);

    void findNode(const string &key, vector<int> &array);

    void addNode(const Node &node);

    int deleteNode(const Node &node);
};


#endif //BOOKSTORE_SIRIUSNEO_BLOCKLIST_H
