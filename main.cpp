#include <iosfwd>//ifstream
#include <iostream>
#include <string>
#include "bookstore.h"

//#define PaperL_Debug
//#define FileIO
//#define bonusFunction

using namespace std;

Bookstore bookstore_cmd;

#ifdef bonusFunction

#include<string>
#include<windows.h>
#include<conio.h>//getch()
#include<stdlib.h>//system()
#include<ctime>

enum ConsoleForegroundColor {
    CFC_Red = FOREGROUND_INTENSITY | FOREGROUND_RED,
    CFC_Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CFC_Blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    CFC_Yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    CFC_Purple = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    CFC_Cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    CFC_Gray = FOREGROUND_INTENSITY,
    CFC_White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    CFC_HighWhite = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    CFC_Black = 0,
};

enum ConsoleBackGroundColor {
    CBC_Red = BACKGROUND_INTENSITY | BACKGROUND_RED,
    CBC_Green = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
    CBC_Blue = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
    CBC_Yellow = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
    CBC_Purple = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
    CBC_Cyan = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
    CBC_White = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
    CBC_HighWhite = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
    CBC_Black = 0,
};

void delay(const int &time) {
    clock_t now = clock();
    while (clock() - now < time) {
    }
}

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

void pos(int x, int y) {
    COORD posPoint = {(short) x, (short) y}; //设置坐标
    SetConsoleCursorPosition(hOut, posPoint);
}

void bByT() {
    WORD temp =
            FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
}

void cyan_choose() {
    WORD cyan = FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE;
    SetConsoleTextAttribute(hOut, cyan);
}

void on_Select() {
    WORD select = FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED;
    SetConsoleTextAttribute(hOut, select);
}

void onChoose(int x, int y) {
    if (x == 25 && y == 10) {
        on_Select();
        cout << "1. 开始 ";
    } else if (x == 25 && y == 20) {
        on_Select();
        cout << "3. 继续 ";
    } else if (x == 65 && y == 10) {
        on_Select();
        cout << "2. 选择 ";
    } else if (x == 65 && y == 20) {
        on_Select();
        cout << "4. 退出 ";
    }
}

void star() {
    cyan_choose();
    pos(25, 10);
    cout << "1. 开始 ";
    pos(65, 10);
    cout << "2. 选择 ";
    pos(25, 20);
    cout << "3. 继续 ";
    pos(65, 20);
    cout << "4. 退出 ";
}

void textAnimation(int x, int y, string text, ConsoleForegroundColor FC, ConsoleBackGroundColor BC) {
    SetConsoleTextAttribute(hOut, FC | BC);
    pos(x, y);
    for (int i = 0; i < text.size(); ++i) {
        putchar(text[i]);
        delay(15);
    }
}

void text(int x, int y, string textToShow, ConsoleForegroundColor FC, ConsoleBackGroundColor BC) {
    SetConsoleTextAttribute(hOut, FC | BC);
    pos(x, y);
    cout << textToShow;
}

const string UIString1[7] = {"Login", "Logout", "Register", "Change Password", "Show Book List", "Search Book",
                             "Purchase Book"}
, UIString3[11] = {"Login", "Logout", "Add a User", "Change Password", "Select a Book", "Modify Book Info",
                   "Import Book", "Show Book List", "Search Book", "Purchase Book", "My Operation Report"},
        UIString7[15] = {"Login", "Logout", "Add a User", "Change Password", "Select a Book", "Modify Book Info",
                         "Import Book", "Show Book List", "Search Book", "Purchase Book", "Finance Report",
                         "Employee Report", "System Log", "DO NOT PRESS THIS"};

void drawUI(int privilege, string userName) {
    if (privilege == 0)
        text(28, 13, "Welcome!", CFC_Yellow, CBC_Black);
    else if (privilege == 1)
        text(28, 8, "Welcome, " + userName + "!", CFC_Yellow, CBC_Black);
    else if (privilege == 3)
        text(28, 6, "Welcome, " + userName + "!", CFC_Yellow, CBC_Black);
    else if (privilege == 7)
        text(28, 4, "Welcome, Boss!", CFC_Yellow, CBC_Black);

    if (privilege == 1)
        text(28, 10, "What are you going to do?", CFC_Yellow, CBC_Black);
    if (privilege == 3)
        text(28, 8, "What are you going to do?", CFC_Yellow, CBC_Black);

    int tempLen, tempCol;

    if (privilege == 0) {
        text(32, 16, "Login", CFC_Green, CBC_Black);
        text(32, 19, "Register", CFC_Green, CBC_Black);

    } else if (privilege == 1) {
        tempCol = 32;
        tempLen = 12;
        int i;
        for (i = 0; i <= 3; ++i)
            text(tempCol, tempLen + i * 2, UIString1[i], CFC_Blue, CBC_Black);
        for (i = 4; i <= 6; ++i)
            text(tempCol, tempLen + i * 2, UIString1[i], CFC_Green, CBC_Black);

    } else if (privilege == 3) {
        tempCol = 30;
        tempLen = 10;
        int i;
        for (i = 0; i <= 3; ++i)
            text(tempCol, tempLen + i * 2, UIString3[i], CFC_Blue, CBC_Black);
        for (i = 4; i <= 6; ++i)
            text(tempCol, tempLen + i * 2, UIString3[i], CFC_Yellow, CBC_Black);
        for (i = 7; i <= 9; ++i)
            text(tempCol, tempLen + i * 2, UIString3[i], CFC_Green, CBC_Black);
        text(tempCol, tempLen + 10 * 2, UIString3[i], CFC_Red, CBC_Black);

    } else if (privilege == 7) {
        tempCol = 28;
        tempLen = 6;
        int i;
        for (i = 0; i <= 4; ++i)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Blue, CBC_Black);
        for (i = 5; i <= 7; ++i)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Yellow, CBC_Black);
        for (i = 8; i <= 10; ++i)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Green, CBC_Black);
        for (i = 11; i <= 14; ++i)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Red, CBC_Black);
    }

}

void drawUIAnimation(int privilege, string userName) {
    if (privilege == 0)
        textAnimation(28, 13, "Welcome!", CFC_Yellow, CBC_Black);
    else if (privilege == 1)
        textAnimation(28, 8, "Welcome, " + userName + "!", CFC_Yellow, CBC_Black);
    else if (privilege == 3)
        textAnimation(28, 6, "Welcome, " + userName + "!", CFC_Yellow, CBC_Black);
    else if (privilege == 7)
        text(28, 4, "Welcome, Boss!", CFC_Yellow, CBC_Black);

    if (privilege == 1)
        textAnimation(28, 10, "What are you going to do?", CFC_Yellow, CBC_Black);
    if (privilege == 3)
        textAnimation(28, 8, "What are you going to do?", CFC_Yellow, CBC_Black);

    int tempLen, tempCol;

    if (privilege == 0) {
        textAnimation(32, 16, "Login", CFC_Green, CBC_Black);
        textAnimation(32, 19, "Register", CFC_Green, CBC_Black);

    } else if (privilege == 1) {
        tempCol = 32;
        tempLen = 12;
        int i;
        for (i = 0; i <= 3; ++i)
            textAnimation(tempCol, tempLen + i * 2, UIString1[i], CFC_Blue, CBC_Black);
        for (i = 4; i <= 6; ++i)
            textAnimation(tempCol, tempLen + i * 2, UIString1[i], CFC_Green, CBC_Black);

    } else if (privilege == 3) {
        tempCol = 30;
        tempLen = 10;
        int i;
        for (i = 0; i <= 3; ++i)
            textAnimation(tempCol, tempLen + i * 2, UIString3[i], CFC_Blue, CBC_Black);
        for (i = 4; i <= 6; ++i)
            textAnimation(tempCol, tempLen + i * 2, UIString3[i], CFC_Yellow, CBC_Black);
        for (i = 7; i <= 9; ++i)
            textAnimation(tempCol, tempLen + i * 2, UIString3[i], CFC_Green, CBC_Black);
        textAnimation(tempCol, tempLen + 10 * 2, UIString3[i], CFC_Red, CBC_Black);

    } else if (privilege == 7) {
        tempCol = 28;
        tempLen = 6;
        int i;
        for (i = 0; i <= 4; ++i)
            textAnimation(tempCol, tempLen + i * 2, UIString7[i], CFC_Blue, CBC_Black);
        for (i = 5; i <= 7; ++i)
            textAnimation(tempCol, tempLen + i * 2, UIString7[i], CFC_Yellow, CBC_Black);
        for (i = 8; i <= 10; ++i)
            textAnimation(tempCol, tempLen + i * 2, UIString7[i], CFC_Green, CBC_Black);
        for (i = 11; i <= 14; ++i)
            textAnimation(tempCol, tempLen + i * 2, UIString7[i], CFC_Red, CBC_Black);
    }

}


void drawUI(int privilege, int index) {
    int tempLen, tempCol;

    if (privilege == 1) {
        tempCol = 32;
        tempLen = 12;
        int i = index;
        if (i <= 3)
            text(tempCol, tempLen + i * 2, UIString1[i], CFC_Blue, CBC_Black);
        else
            text(tempCol, tempLen + i * 2, UIString1[i], CFC_Green, CBC_Black);

    } else if (privilege == 3) {
        tempCol = 30;
        tempLen = 10;
        int i = index;
        if (i <= 3)
            text(tempCol, tempLen + i * 2, UIString3[i], CFC_Blue, CBC_Black);
        else if (i <= 6)
            text(tempCol, tempLen + i * 2, UIString3[i], CFC_Yellow, CBC_Black);
        else if (i <= 9)
            text(tempCol, tempLen + i * 2, UIString3[i], CFC_Green, CBC_Black);
        else
            text(tempCol, tempLen + 10 * 2, UIString3[i], CFC_Red, CBC_Black);

    } else if (privilege == 7) {
        tempCol = 28;
        tempLen = 6;
        int i = index;
        if (i <= 4)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Blue, CBC_Black);
        else if (i <= 7)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Yellow, CBC_Black);
        else if (i <= 10)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Green, CBC_Black);
        else if (i <= 14)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Red, CBC_Black);
    }
}

void drawHighlightUI(int privilege, int index) {
    int tempLen, tempCol;

    if (privilege == 1) {
        tempCol = 32;
        tempLen = 12;
        int i = index;
        if (i <= 3)
            text(tempCol, tempLen + i * 2, UIString1[i], CFC_Blue, CBC_Yellow);
        else
            text(tempCol, tempLen + i * 2, UIString1[i], CFC_Green, CBC_Yellow);

    } else if (privilege == 3) {
        tempCol = 30;
        tempLen = 10;
        int i = index;
        if (i <= 3)
            text(tempCol, tempLen + i * 2, UIString3[i], CFC_Blue, CBC_Yellow);
        else if (i <= 6)
            text(tempCol, tempLen + i * 2, UIString3[i], CFC_Yellow, CBC_Green);
        else if (i <= 9)
            text(tempCol, tempLen + i * 2, UIString3[i], CFC_Green, CBC_Yellow);
        else
            text(tempCol, tempLen + 10 * 2, UIString3[i], CFC_Red, CBC_Yellow);

    } else if (privilege == 7) {
        tempCol = 28;
        tempLen = 6;
        int i = index;
        if (i <= 4)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Blue, CBC_Yellow);
        else if (i <= 7)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Yellow, CBC_Green);
        else if (i <= 10)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Green, CBC_Yellow);
        else if (i <= 14)
            text(tempCol, tempLen + i * 2, UIString7[i], CFC_Red, CBC_Yellow);
    }
}

void doCommand(int posX, int posY, string a, string b) {
    string tempS;
    textAnimation(posX, posY, a, CFC_Gray, CBC_Black);
    pos(posX, posY + 1);
    SetConsoleTextAttribute(hOut, CFC_HighWhite | CBC_Black);
    getline(cin, tempS);
    pos(posX, posY + 2);
    SetConsoleTextAttribute(hOut, CFC_Gray | CBC_Black);
    bookstore_cmd.operation(b + tempS);
    textAnimation(posX, posY, "Press Any Key to Continue...", CFC_Purple, CBC_Black);
    if (a.size() > 25) {
        for (int i = 0; i < a.size() - 25; ++i)putchar(' ');
    }
}

void doCommand(int posX, int posY, string a) {
    pos(posX, posY + 2);
    SetConsoleTextAttribute(hOut, CFC_Gray | CBC_Black);
    bookstore_cmd.operation(a);
    textAnimation(posX, posY, "Press Any Key to Continue...", CFC_Purple, CBC_Black);
}


#endif

int main() {

#ifndef bonusFunction

#ifdef PaperL_Debug
    cout << "@ Initialization Finish." << endl << endl;
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
        //cout << "====@ Operation : \"" << inputString << "\"" << endl;
        cout << "<" << inputString << ">:";
#endif

        bookstore_cmd.operation(inputString);

#ifdef PaperL_Debug
        //cout << "====@ Operation Finish" << endl << endl;
#endif

        //getline(cin, inputString);
    }
#ifdef FileIO
    fclose(stdin);
    fclose(stdout);
#endif

#endif

#ifdef bonusFunction

    system("mode con cols=80 lines=40");
    SetConsoleTitle("SiriusNEO BookStore Beta");
    system("color 0E");
    textAnimation(20, 17, "Welcome to SiriusNEO BookStore System!", CFC_Cyan, CBC_Black);
    //pos(20, 17);
    //printf("Welcome to SiriusNEO BookStore System!");
    delay(2500);
    system("cls");

    string userName;
    int pre_privilege = -1;
    int privilege = 0;
    string inputString;

    while (1) {

        bookstore_cmd.getInfo(privilege, userName);
        system("cls");
        if (pre_privilege != privilege) {
            drawUIAnimation(privilege, userName);
            pre_privilege = privilege;
        } else
            drawUI(privilege, userName);

        int posX, posY;
        char tempC;
        if (privilege == 0) {
            posX = 32, posY = 16;
            text(32, 16, "Login", CFC_Green, CBC_Yellow);
            tempC = getch();
            while (tempC != 13 && tempC != 27) {
                //pos(0, 0);
                //cout << posY << "  " << (int) tempC;

                switch (tempC) {
                    case 72:
                        posY -= 3;
                        break;
                    case 80:
                        posY += 3;
                        break;
                }
                if (posY < 16)posY = 16;
                if (posY > 19)posY = 19;
                if (posY == 16) {
                    text(32, 16, "Login", CFC_Green, CBC_Yellow);
                    text(32, 19, "Register", CFC_Green, CBC_Black);
                } else {
                    text(32, 16, "Login", CFC_Green, CBC_Black);
                    text(32, 19, "Register", CFC_Green, CBC_Yellow);
                }

                tempC = getch();
            }
            if (tempC == 27)break;

            if (posY == 16) {
                doCommand(28, 22, "Input [user-id] [passwd]:", "su ");
            } else {
                doCommand(28, 22, "Input [user-id] [passwd] [name]:", "register ");
            }

        } else if (privilege == 1) {
            posX = 32, posY = 12;
            text(posX, posY, "Login", CFC_Blue, CBC_Yellow);
            tempC = 0;
            while (tempC != 13 && tempC != 27) {
                //pos(0, 0);
                //cout << posY << "  " << (int) tempC;
                drawUI(privilege, (posY - 12) / 2);

                switch (tempC) {
                    case 72:
                        posY -= 2;
                        break;
                    case 80:
                        posY += 2;
                        break;
                }

                if (posY < 12)posY = 12;
                if (posY > 24)posY = 24;
                drawHighlightUI(privilege, (posY - 12) / 2);
                tempC = getch();
            }
            if (tempC == 27)break;

            if (posY == 12)
                doCommand(28, 28, "Input [user-id] [passwd]:", "su ");
            else if (posY == 14)
                doCommand(28, 28, "logout");
            else if (posY == 16)
                doCommand(28, 28, "Input [user-id] [passwd] [name]:", "register ");
            else if (posY == 18)
                doCommand(28, 28, "Input [user-id] [old-passwd] [new-passwd]:", "passwd ");
            else if (posY == 20)
                doCommand(0, 28, "show");
            else if (posY == 22) {
                textAnimation(0, 27, "Examples: {-ISBN=1-1-4}, {-name=\"The Man Who..\"}, {-keyword=\"R12|Fullcolor\"}",
                              CFC_Purple, CBC_Black);
                doCommand(0, 28, "Input an Argument:", "show ");
                textAnimation(0, 29, "                                                                             ",
                              CFC_Gray, CBC_Black);
            } else if (posY == 24)
                doCommand(28, 28, "Input [ISBN] [quantity]:", "buy ");

        } else if (privilege == 3) {
            posX = 30, posY = 10;
            text(posX, posY, "Login", CFC_Blue, CBC_Yellow);
            tempC = 0;
            while (tempC != 13 && tempC != 27) {
                //pos(0, 0);
                //cout << posY << "  " << (int) tempC;
                drawUI(privilege, (posY - 10) / 2);

                switch (tempC) {
                    case 72:
                        posY -= 2;
                        break;
                    case 80:
                        posY += 2;
                        break;
                }

                if (posY < 10)posY = 10;
                if (posY > 30)posY = 30;
                drawHighlightUI(privilege, (posY - 10) / 2);
                tempC = getch();
            }
            if (tempC == 27)break;

            if (posY == 10)
                doCommand(28, 32, "Input [user-id] [passwd]:", "su ");
            else if (posY == 12)
                doCommand(28, 32, "logout");
            else if (posY == 14)
                doCommand(28, 32, "Input [user-id] [passwd] [7/3/1] [name]:", "useradd ");
            else if (posY == 16)
                doCommand(28, 32, "Input [user-id] [old-passwd] [new-passwd]:", "passwd ");
            else if (posY == 18)
                doCommand(28, 32, "Input [ISBN]:", "select ");
            else if (posY == 20) {
                textAnimation(0, 31, "Examples: {-ISBN=1-1-4}, {-name=\"The Man Who..\"}, {-keyword=\"R12|Fullcolor\"}",
                              CFC_Purple, CBC_Black);
                doCommand(28, 32, "Input an Argument:", "modify ");
                textAnimation(0, 31, "                                                                             ",
                              CFC_Gray, CBC_Black);
            } else if (posY == 22)
                doCommand(28, 32, "Input [quantity] [cost price in total]:", "import ");
            else if (posY == 24)
                doCommand(0, 32, "show");
            else if (posY == 26) {
                textAnimation(0, 31, "Examples: {-ISBN=1-1-4}, {-name=\"The Man Who..\"}, {-keyword=\"R12|Fullcolor\"}",
                              CFC_Purple, CBC_Black);
                doCommand(0, 32, "Input an Argument:", "show ");
                textAnimation(0, 31, "                                                                             ",
                              CFC_Gray, CBC_Black);
            } else if (posY == 28)
                doCommand(28, 32, "Input [ISBN] [quantity]:", "buy ");
            else
                textAnimation(26, 32, "Sorry I can tell you nothing.", CFC_Purple, CBC_Black);


        } else if (privilege == 7) {
            posX = 28, posY = 6;
            text(posX, posY, "Login", CFC_Blue, CBC_Yellow);
            tempC = 0;
            while (tempC != 13 && tempC != 27) {
                //pos(0, 0);
                //cout << posY << "  " << (int) tempC;
                drawUI(privilege, (posY - 6) / 2);

                switch (tempC) {
                    case 72:
                        posY -= 2;
                        break;
                    case 80:
                        posY += 2;
                        break;
                }

                if (posY < 6)posY = 6;
                if (posY > 32)posY = 32;
                drawHighlightUI(privilege, (posY - 6) / 2);
                tempC = getch();
            }
            if (tempC == 27)break;

            if (posY == 6)
                doCommand(26, 34, "Input [user-id] [passwd]:", "su ");
            else if (posY == 8)
                doCommand(26, 34, "logout");
            else if (posY == 10)
                doCommand(26, 34, "Input [user-id] [passwd] [7/3/1] [name]:", "useradd ");
            else if (posY == 12)
                doCommand(26, 34, "Input [user-id] [old-passwd] [new-passwd]:", "passwd ");
            else if (posY == 14)
                doCommand(26, 34, "Input [ISBN]:", "select ");
            else if (posY == 16) {
                textAnimation(0, 33, "Examples: {-ISBN=1-1-4}, {-name=\"The Man Who..\"}, {-keyword=\"R12|Fullcolor\"}",
                              CFC_Purple, CBC_Black);
                doCommand(26, 34, "Input an Argument:", "modify ");
                textAnimation(0, 33, "                                                                             ",
                              CFC_Gray, CBC_Black);
            } else if (posY == 18)
                doCommand(26, 34, "Input [quantity] [cost price in total]:", "import ");
            else if (posY == 20)
                doCommand(0, 34, "show");
            else if (posY == 22) {
                textAnimation(0, 33, "Examples: {-ISBN=1-1-4}, {-name=\"The Man Who..\"}, {-keyword=\"R12|Fullcolor\"}",
                              CFC_Purple, CBC_Black);
                doCommand(0, 34, "Input an Argument:", "show ");
                textAnimation(0, 33, "                                                                             ",
                              CFC_Gray, CBC_Black);
            } else if (posY == 24)
                doCommand(26, 34, "Input [ISBN] [quantity]:", "buy ");
            else if (posY >= 26 && posY <= 30)
                textAnimation(26, 34, "Sorry I can tell you nothing.", CFC_Purple, CBC_Black);
            else {
                textAnimation(26, 34, "Why you don't believe me?", CFC_Purple, CBC_Black);
                delay(2000);
                textAnimation(26, 34, "You are a really bad guy.", CFC_Purple, CBC_Black);
                delay(2000);
                textAnimation(26, 34, "I'm so sad.              ", CFC_Purple, CBC_Black);
                delay(2000);
                textAnimation(22, 34, "You know I can't get out of the screen ..", CFC_Purple, CBC_Black);
                delay(1000);
                textAnimation(22, 34, "    .. to punch in your face, uh?        ", CFC_Purple, CBC_Black);
                delay(2000);
                textAnimation(26, 34, "But what I do can ..         ", CFC_Purple, CBC_Black);
                delay(2000);
                textAnimation(26, 34, "DELETE YOUR DATABASE !!!", CFC_Red, CBC_Black);
                fstream deleteIt;
                deleteIt.open("author.bin", ios::out | ios::trunc | ios::binary), deleteIt.close();
                deleteIt.open("books.dat", ios::out | ios::trunc | ios::binary), deleteIt.close();
                deleteIt.open("finance.dat", ios::out | ios::trunc | ios::binary), deleteIt.close();
                deleteIt.open("id.bin", ios::out | ios::trunc | ios::binary), deleteIt.close();
                deleteIt.open("isbn.bin", ios::out | ios::trunc | ios::binary), deleteIt.close();
                deleteIt.open("keyword.bin", ios::out | ios::trunc | ios::binary), deleteIt.close();
                deleteIt.open("log.dat", ios::out | ios::trunc | ios::binary), deleteIt.close();
                deleteIt.open("name.bin", ios::out | ios::trunc | ios::binary), deleteIt.close();
                deleteIt.open("users.dat", ios::out | ios::trunc | ios::binary), deleteIt.close();
                delay(2000);
                textAnimation(26, 34, "OK, It's Done.             ", CFC_Green, CBC_Black);
                delay(1000);
                textAnimation(26, 34, "See you       ", CFC_Green, CBC_Black);
                return 0;
            }

        }

        /*string inputString;
        while (getline(cin, inputString) && inputString != "exit" && inputString != "quit")
            bookstore_cmd.operation(inputString);*/
        tempC = getch();
        if (tempC == 27)break;
    }
#endif

    return 0;
}
