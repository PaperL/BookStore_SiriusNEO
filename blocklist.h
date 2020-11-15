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
    int offset;
    int isdel;
    char str[64];

    bool operator<(const Node &x) const;

    Node();

    Node(int offset, string s);
};

class Block {
public:
    int nxt;
    int pre;
    int num;
    Node array[SIZE];

    Block();
};

class blocklist {
private:
    const string fname;
    fstream fi, fo, fi2, fo2, fip;

    inline int nextBlock(int offset);

    void delBlock(int offset);

    void mergeBlock(int offset1, int offset2);

    void splitBlock(int offset, int index);

public:
    blocklist(string arg);

    void findNode(string key, vector<int> &array);//todo 这边改了int

    void addNode(Node node);

    int deleteNode(string key);
};


#endif //BOOKSTORE_SIRIUSNEO_BLOCKLIST_H
