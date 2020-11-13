#include<iostream>
#include<string>
#include "bookstore.h"

using namespace std;

Bookstore bookstore_cmd;

int main() {
    string inputString;
    getline(cin, inputString);
    while (!inputString.empty()) {
        bookstore_cmd.operation(inputString);
        getline(cin, inputString);
    }
    return 0;
}
