//
// Created by 19150 on 2022/5/30.
//

#include "supportControl.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

extern void helpList(int optimizer)
{
    switch (optimizer) {
        case 0:
            {
                cout << "   *************************************** help ******************************************" << endl;
                cout << "  order  | command       | optimizer     | optimizer     | " << "function explanation    |" << endl;
                cout << "   01.   | register      |               |               | " << "user register           |" << endl;
                cout << "   02.   | login         |               |               | " << "user login              |" << endl;
                cout << "   03.   | logout        |               |               | " << "user logout             |" << endl;
                cout << "   04.   | dir           |               |               | " << "show directory          |" << endl;
                cout << "   05.   | mkdir         | dirname       |               | " << "create directory        |" << endl;
                cout << "   06.   | cd            | dirname       |               | " << "enter directory         |" << endl;
                cout << "   07.   | rmdir         | dirname       |               | " << "delete directory        |" << endl;
                cout << "   08.   | create        | filename      |               | " << "create file             |" << endl;
                cout << "   09.   | write         | filename      | content       | " << "write to file           |" << endl;
                cout << "   10.   | close         | filename      |               | " << "close file              |" << endl;
                cout << "   11.   | open          | filename      |               | " << "open file               |" << endl;
                cout << "   12.   | read          | filename      | length        | " << "read file content       |" << endl;
                cout << "   13.   | delete        | filename      |               | " << "delete file             |" << endl;
                cout << "   14.   | format        |               |               | " << "format file             |" << endl;
                cout << "   15.   | cl            | username      |               | " << "change user             |" << endl;
                cout << "   16.   | justCopy      | filename      |               | " << "copy file               |" << endl;
                cout << "   17.   | paste         |               |               | " << "paste file              |" << endl;
                cout << "   18.   | cut           | filename      |               | " << "cut file                |" << endl;
                cout << "   19.   | rename        | filename      | newName       | " << "rename filename         |" << endl;
                cout << "   20.   | show          | filename      |               | " << "show file content       |" << endl;
                cout << "   21.   | exit          |               |               | " << "exit exe                |" << endl;
                cout << "   22.   | add           | filename      | newName       | " << "add content for file    |" << endl;
                cout << "   23.   | copy          | filename      | newName       | " << "copy content for file   |" << endl;
            }
            break;
        case 1:cout << "extreme help is not access now, please try later"<<endl;
            break;

    }
}

int statistic(const string& filename)
{
    ifstream fin(filename);
    if(!fin.is_open())
        return -1;
    string t;
    fin >> noskipws >> t;
    return t.size();
}

extern string getCurrentTime()
{
    time_t now = time(0);
    char *dt = ctime(&now);
    string current_time = string(dt);
    return current_time;
}

//获取本地电脑主机名，用来进行菜单显示
extern string getLocalIpName() {
    TCHAR buf[MAX_COMPUTERNAME_LENGTH + 2];
    DWORD buf_size;
    buf_size = sizeof buf - 1;
    GetComputerName(buf, &buf_size);
    string a = buf;
    return a;
}
