#include<iostream>
#include<string>
#include "bookstore.h"

#define PaperL_Debug

using namespace std;

Bookstore bookstore_cmd;

int main() {
    string inputString;
#ifdef PaperL_Debug
    cout << "@ Initialization Finish." << endl;
#endif
    getline(cin, inputString);
    while (inputString != "exit" && inputString != "quit") {
#ifdef PaperL_Debug
        cout << "@ Operation : \"" << inputString << "\"" << endl;
#endif
        bookstore_cmd.operation(inputString);
#ifdef PaperL_Debug
        cout << "@ Operation Finish" << endl;
#endif
        getline(cin, inputString);
    }
    return 0;
}
