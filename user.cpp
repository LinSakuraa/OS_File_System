//
// Created by 22325 on 2022/5/30.
//

#include "user.h"
#include <bits/stdc++.h>

string currentUser = "";

User::User() { }

User::User(string un, string pw, Directory *dir) : curDir(dir), username(un), password(pw) { }

// 获取用户名
string User::getUsername() {
    return username;
}

// 获取密码
string User::getPassword() {
    return password;
}

// 登录
string User::login(string un, string pw) {
    if(un == username && pw == password) {
        state = true;
        return username;
    }
    else
        return "";
}

// 获取当前目录
Directory* User::getCurDir() {
    return curDir;
}

//设置当前目录
void User::setCurDir(Directory *dir) {
    curDir = dir;
}

// 检查状态
bool User::check() {
    return state;
}

// 登出
void User::logout() {
    state = false;
}

// 删除对应用户
void User::clear() {
    curDir = nullptr;
    username = "";
    password = "";
    state = false;
}

//计算文件大小
int Users::calculateFileSize(const string& filename) {
    ifstream file(filename);
    if(!file.is_open())
        return -1;
    string t;
    file >> noskipws >> t;
    return t.size();
}

// 读取用户列表
void Users::readUserList() {
    ifstream file("users.txt");
    if(!file.is_open()) {
        cout << "users.txt can not open in checkIsAuthor function " << endl;
        exit(0);
    }
    string line;
    int cnt;
    getline(file, line);
    userListSize = atoi(line.c_str());
    for(int i = 0; i < userListSize; i++) {
        getline(file, line);
        string username = line;
        getline(file, line);
        string password = line;
        User tmp(username, password);
        userList.push_back(tmp);
    }
}

//是否存在该用户
bool Users::isExistedAuthor(const string &username) {
    for(auto & i : userList){
        if(i.getUsername() == username){
            return true;
        }
    }
    return false;
}

//新建用户
void Users::createUser(const string& username, const string& password) {
    if(!isExistedAuthor(username)) {
        User tmp(username, password);
        userList.push_back(tmp);
        userListSize++;
    }
}

// 保存用户
void Users::saveUser() {
    ofstream file("users.txt");
    if(!file.is_open()) {
        cout << "users.txt can not open in createUser function " << endl;
        exit(0);
    }
    file << userList.size() << endl;
    for(auto & i : userList) {
        file << i.getUsername() << endl;
        file << i.getPassword() << endl;
    }
    file.close();
}

//用户登录
bool Users::login(string username, string password) {
    for(int i = 0; i < userListSize; i ++) {
        if(!userList[i].login(username, password).empty()) {
            currentUser = username;
            // 加载目录一类的
            return true;
        }
    }
    return false;
}

// 搜索用户
int Users::searchUser(string username) {
    for(int i = 0; i < userListSize; i ++) {
        if(userList[i].getUsername() == username)
            return i;
    }
    return -1;
}

// 切换用户
bool Users::switchUser(string username) {
    int i = searchUser(username);
    if(i == -1)
        return false;
    if(userList[i].check()) {
        currentUser = userList[i].getUsername();
        return true;
    }
    else
        return false;
}

// 登出
void Users::logout() {
    int i = searchUser(currentUser);
    if(i == -1)
        return;
    userList[i].logout();
    currentUser = "";
}

// 获取当前目录
Directory* Users::getCurDir() {
    int i = searchUser(currentUser);
    if(i == -1)
        return nullptr;
    return userList[i].getCurDir();
}

// 设置当前目录
void Users::setCurDir(Directory* dir) {
    int i = searchUser(currentUser);
    if(i == -1)
        return;
    userList[i].setCurDir(dir);
}

bool Users::check() {
    for(auto& it:userList) {
        if(it.check())
            return true;
    }
    return false;
}

void Users::showDir() {
    Directory* curDir = getCurDir();
    curDir -> show();
}