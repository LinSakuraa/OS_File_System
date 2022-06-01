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
                cout << "         | command       | optimizer     | optimizer     | " << "function explanation    |" << endl;
                cout << "         | register      |               |               | " << "user register           |" << endl;
                cout << "         | login         |               |               | " << "user login              |" << endl;
                cout << "         | logout        |               |               | " << "user logout             |" << endl;
                cout << "         | dir           |               |               | " << "show directory          |" << endl;
                cout << "         | mkdir         | dirname       |               | " << "create directory        |" << endl;
                cout << "         | cd            | dirname       |               | " << "enter directory         |" << endl;
                cout << "         | rmdir         | dirname       |               | " << "delete directory        |" << endl;
                cout << "         | create        | filename      |               | " << "create file             |" << endl;
                cout << "         | write         | filename      | content       | " << "write to file           |" << endl;
                cout << "         | close         | filename      |               | " << "close file              |" << endl;
                cout << "         | open          | filename      |               | " << "open file               |" << endl;
                cout << "         | read          | filename      | length        | " << "read file content       |" << endl;
                cout << "         | delete        | filename      |               | " << "delete file             |" << endl;
                cout << "         | format        |               |               | " << "format file             |" << endl;
                cout << "         | cl            | username      |               | " << "change user             |" << endl;
                cout << "         | justCopy      | filename      |               | " << "copy file               |" << endl;
                cout << "         | paste         |               |               | " << "paste file              |" << endl;
                cout << "         | cut           | filename      |               | " << "cut file                |" << endl;
                cout << "         | rename        | filename      | newName       | " << "rename filename         |" << endl;
                cout << "         | show          | filename      |               | " << "show file content       |" << endl;
                cout << "         | exit          |               |               | " << "exit exe                |" << endl;
                cout << "         | add           | filename      | newName       | " << "add content for file    |" << endl;
                cout << "         | copy          | filename      | newName       | " << "copy content for file   |" << endl;
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
