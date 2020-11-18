#include <stdio.h>
#include <iosfwd>//ifstream
#include <iostream>
#include <string>

#include "bookstore.h"

//#define PaperL_Debug
#define FileIO

using namespace std;

Bookstore bookstore_cmd;

int main() {

#ifdef PaperL_Debug
    cout << "@ Initialization Finish." << endl<<endl;
#endif

#ifdef FileIO
    freopen("1.txt","r",stdin);
    ifstream inputStream("1.in");//cin重定向至文件
    cin.rdbuf(inputStream.rdbuf());
    ofstream outputStream("1.ans");
    cout.rdbuf(outputStream.rdbuf());
    freopen("1.ans","w",stdout);//printf重定向至文件
#endif

    string inputString;
    //while(getline) 在EOF时跳出循环
    while (getline(cin, inputString) && inputString != "exit" && inputString != "quit") {

#ifdef PaperL_Debug
        cout << "    @ Operation : \"" << inputString << "\"" << endl;
#endif

        bookstore_cmd.operation(inputString);

#ifdef PaperL_Debug
        cout << "    @ Operation Finish" << endl;
#endif

        //getline(cin, inputString);
    }
#ifdef FileIO
    fclose(stdin);
    fclose(stdout);
#endif
    return 0;
}
