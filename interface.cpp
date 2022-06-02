//
// Created by 22325 on 2022/5/30.
//

#include "interface.h"

OS os;

void OS::printTitle()
{
    string path;
    if(currentUser.empty()) {
        cout << "$/>";
        return;
    } else {
        Directory* curDir;
        int id;
        string name;
        int pid;
        curDir = fileSys.users.getCurDir();
        Directory* t = curDir;
        id = curDir -> getItem(".");
        pid = curDir -> getItem("..");
        curDir = fileSys.returnToParent();
        while(pid != 0) {
            name = fileSys.superBlock.iNodeList.getInode(pid).dir.getFileName(id);
            path = name + "/" + path;
            id = curDir -> getItem(".");
            pid = curDir -> getItem("..");
            curDir = fileSys.returnToParent();
        }
        if(id != pid) {
            name = fileSys.superBlock.iNodeList.getInode(pid).dir.getFileName(id);
            path = "/" + name + "/" + path;
        } else {
            path = "/";
        }
        cout << "$" << path << ">";
        fileSys.users.setCurDir(t);
        return;
    }
}

void OS::run()
{
    string cmd;
    int i = 1;
    while (true) {
        printTitle();
        cin >> cmd;
        if(cmd == "login") { // login
            string username, password;
            cout << "username:";
            cin >> username;
            cout << "password:";
            cin >> password;
            bool state = fileSys.users.login(username, password);
            if(state)
                cout << "welcome " << username << endl;
            else
                cout << "wrong username or password" << endl;
        } else if(cmd == "register") {
            string username,password = "";
            cout << "username:";
            cin >> username;
            cout << "password:";
            cin >> password;
            bool status = fileSys.users.createUser(username,password);
            if(status)
            {
                fileSys.users.createUser(username, password);
                int j = fileSys.createUserDirectory(username);
                fileSys.users.userList[i].setCurDir(&(fileSys.superBlock.iNodeList.getInode(j).dir));
                i ++;
                fileSys.users.saveUser();
                cout << "register successful !" << endl;
            }
            else cout << "register failed !" << endl;
        } else if(cmd == "cd") { // enter directory
            string index;
            cin >> index;
            fileSys.cd(index);
        } else if(cmd == "mkdir") { // create directory
            string index;
            cin >> index;
            fileSys.directoryCreate(index);
        } else if(cmd == "dir")  { // show dir
            fileSys.showDir();
        } else if(cmd == "create") { // create file
            string filename;
            cin >> filename;
            fileSys.createFile(filename);
        } else if(cmd == "open") { // open file
            string filename;
            int sign, mode;
            cin >> filename;
            cout << "insert[0/1]:";
            cin >> sign;
            if(sign != 0 && sign != 1)
                sign = 0;
            cout << "mode[0/1]:";
            cin >> mode;
            if(mode != 0 && mode != 1)
                mode = 0;
            fileSys.openFile(filename, sign, mode);
        } else if(cmd == "close") { // close file
            string filename;
            cin >> filename;
            fileSys.closeFile(filename);
        } else if(cmd == "read") { // read file
            string filename;
            cin >> filename;
            int len;
            cin >> len;
            cout << fileSys.readFile(filename, len);
        } else if(cmd == "write") { // write file 2 optimizer
            string filename;
            cin >> filename;
            string content;
            getline(cin, content);
            content = content.substr(1);
            fileSys.writeFile(filename, content);
        } else if(cmd == "delete") { // delete file
            string filename;
            cin >> filename;
            fileSys.fileDelete(filename);
        } else if(cmd == "logout") { // log out
            fileSys.users.logout();
        } else if(cmd == "rmdir") { // del directory
            string filename;
            cin >> filename;
            fileSys.directoryDelete(filename);
        } else if(cmd == "format") { //format
            fileSys.formatFileSystem();
        } else if(cmd == "cl") { // change users
            string username;
            cin >> username;
            fileSys.users.switchUser(username);
        } else if(cmd == "justcopy") {// copy
            string filename;
            cin >> filename;
            fileSys.copy(filename);
        } else if(cmd == "paste") { // paste
            fileSys.paste();
        } else if(cmd == "cut") { // cut
            string filename;
            cin >> filename;
            fileSys.cut(filename);
        } else if(cmd == "rename") { // rename
            string filename, newName;
            cin >> filename >> newName;
            fileSys.fileRename(filename, newName);
        } else if(cmd == "fileseek") { // re position pointer
            string filename;
            int offset;
            cin >> filename >> offset;
            fileSys.fSeek(filename, offset);
        } else if(cmd == "showindex") { // show information of files
            string filename;
            cin >> filename;
            fileSys.showFile(filename);
        } else if(cmd == "exit") { // quit
            exit(0);
        } else if(cmd == "help") { // help list
            help(0);
        } else if(cmd == "helpx") {//help list extreme
            help(1);
        }else if(cmd == "save") { // save
            fileSys.saveInodeInfo();
        } else if(cmd == "readInfo") { // read
            fileSys.readInodeInfo();
        } else if(cmd == "copy") { // copy to another file
            string filename, newName;
            cin >> filename >> newName;
            fileSys.copy2(filename, newName);
        } else if(cmd == "show") {
            string filename;
            cin >> filename;
            fileSys.show(filename);
        } else if(cmd == "add") {
            string filename, newName;
            cin >> filename >> newName;
            fileSys.add(filename, newName);
        }
    }
}

void OS::help(int optimizer)
{
    helpList(optimizer);
}