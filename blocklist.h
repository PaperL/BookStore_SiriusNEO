//
// Created by PaperL on 2020/11/9.
//
#ifndef BOOKSTORE_SIRIUSNEO_BLOCKLIST_H
#define BOOKSTORE_SIRIUSNEO_BLOCKLIST_H

#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <string.h>

#define SIZE 320

using namespace std;

class Node {
public:
    Node();

    int offset;
    int isdel;
    char str[64];

    bool operator<(const Node &x) const;
};

class Block {
public:
    Block();

    int nxt;
    int pre;
    int num;
    Node array[SIZE];
};

class blocklist {
private:
    string fname;
    fstream fi, fo, fi2, fo2, fip;

    inline int nextBlock(int offset);

public:
    blocklist(string arg);

    void delBlock(int offset);

    void mergeBlock(int offset1, int offset2);

    void splitBlock(int offset, int index);

    void findNode(string key, vector<Node> &array);

    void addNode(Node node);
};


#endif //BOOKSTORE_SIRIUSNEO_BLOCKLIST_H
