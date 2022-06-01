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
        } else if(cmd == "cd") { // enter directory
            string index;
            cin >> index;
            fileSys.cd(index);
        } else if(cmd == "mkdir") { // create directory
            string index;
            cin >> index;
            fileSys.directoryCreate(index);
        } else if(cmd == "dir")  { // 展示列表
            fileSys.showDir();
        } else if(cmd == "create") { // create file
            string filename;
            cin >> filename;
            fileSys.fileCreate(filename);
        } else if(cmd == "open") { // 打开文件
            string filename;
            int sign, mode;
            cin >> filename;
            cout << "offset[0/1]:";
            cin >> sign;
            if(sign != 0 && sign != 1)
                sign = 0;
            cout << "mode[0/1]:";
            cin >> mode;
            if(mode != 0 && mode != 1)
                mode = 0;
            fileSys.openFile(filename, sign, mode);
        } else if(cmd == "close") { // 关闭文件
            string filename;
            cin >> filename;
            fileSys.closeFile(filename);
        } else if(cmd == "read") { // 读取文件
            string filename;
            cin >> filename;
            int len;
            cin >> len;
            cout << fileSys.readFile(filename, len);
        } else if(cmd == "write") { // 写入文件
            string filename;
            cin >> filename;
            string content;
            getline(cin, content);
            content = content.substr(1);
            fileSys.writeFile(filename, content);
        } else if(cmd == "delete") { // 删除文件
            string filename;
            cin >> filename;
            fileSys.fileDelete(filename);
        } else if(cmd == "logout") { // 登出
            fileSys.users.logout();
        } else if(cmd == "rmdir") { // 删除目录
            string filename;
            cin >> filename;
            fileSys.directoryDelete(filename);
        } else if(cmd == "format") { // 格式化
            fileSys.formatFileSystem();
        } else if(cmd == "cl") { // 改变登录账号
            string username;
            cin >> username;
            fileSys.users.switchUser(username);
        } else if(cmd == "justCopy") {// 复制
            string filename;
            cin >> filename;
            fileSys.copy(filename);
        } else if(cmd == "paste") { // 粘贴
            fileSys.paste();
        } else if(cmd == "cut") { // 剪切
            string filename;
            cin >> filename;
            fileSys.cut(filename);
        } else if(cmd == "rename") { // 文件重命名
            string filename, newName;
            cin >> filename >> newName;
            fileSys.fileRename(filename, newName);
        } else if(cmd == "fileSeek") { // 文件内重定位
            string filename;
            int offset;
            cin >> filename >> offset;
            fileSys.fSeek(filename, offset);
        } else if(cmd == "showIndex") { // 显示文件信息
            string filename;
            cin >> filename;
            fileSys.showFile(filename);
        } else if(cmd == "exit") { // 退出
            exit(0);
        } else if(cmd == "help") { // 帮助
            help(0);
        } else if(cmd == "helpx") {
            help(1);
        }else if(cmd == "save") { // 保存
            fileSys.saveInodeInfo();
        } else if(cmd == "readInfo") { // 读取
            fileSys.readInodeInfo();
        } else if(cmd == "copy") { // 复制到当前目录另一个文件
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