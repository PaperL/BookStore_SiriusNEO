#include<iostream>
#include<string.h>
#include "blocklist.h"

using namespace std;
blocklist isbn_cmd("isbn.bin");
blocklist name_cmd("name.bin");
blocklist author_cmd("author.bin");
blocklist keyword_cmd("keyword.bin");
blocklist id_cmd("id.bin");

inline void w_str(char *tempc, string temps) {
    strcpy(tempc, temps.c_str());
}

int main() {
    Node a;
    a.isdel = 1926;
    a.offset = 10086;
    w_str(a.str, "abcde");
    isbn_cmd.addNode(a);

    a.isdel = 1;
    a.offset = 1023;
    w_str(a.str, "zxy");
    isbn_cmd.addNode(a);

    a.isdel = 1;
    a.offset = 3;
    w_str(a.str, "zzz");
    isbn_cmd.addNode(a);

    a.isdel = 1;
    a.offset = 4;
    w_str(a.str, "ddd");
    isbn_cmd.addNode(a);

    a.isdel = 1;
    a.offset = 5;
    w_str(a.str, "zxy");
    isbn_cmd.addNode(a);

    vector<Node> tempVec;
    isbn_cmd.findNode("zxy", tempVec);

    cout << tempVec.size() << endl;
    for (int i = 0; i < tempVec.size(); ++i) {
        cout << tempVec[i].isdel << "," << tempVec[i].offset << "," << tempVec[i].str << endl;
    }

    return 0;
}
