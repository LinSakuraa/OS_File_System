//
// Created by 22325 on 2022/5/30.
//

#include "userControl.h"
#include <bits/stdc++.h>

string currentUser = "";

User::User() { }

User::User(string un, string pw, Directory *dir) : curDir(dir), username(un), password(pw) { }

// get username
string User::getUsername() {
    return username;
}

// get password
string User::getPassword() {
    return password;
}

// user login
string User::login(string un, string pw) {
    if(un == username && pw == password) {
        state = true;
        return username;
    }
    else
        return "";
}

// get current directory
Directory* User::getCurDir() {
    return curDir;
}

// set current directory
void User::setCurDir(Directory *dir) {
    curDir = dir;
}

// check state
bool User::check() {
    return state;
}

// user logout
void User::logout() {
    state = false;
}

// delete user
void User::clear() {
    curDir = nullptr;
    username = "";
    password = "";
    state = false;
}

// calculate file size
int Users::calculateFileSize(const string& filename) {
    ifstream file(filename);
    if(!file.is_open())
        return -1;
    string t;
    file >> noskipws >> t;
    return t.size();
}

// read user list
void Users::readUserList() {
    ifstream file("users.txt");
    if(!file.is_open()) {
        cout << "users.txt can not open in checkIsAuthor function " << endl;
        exit(0);
    }
    string line;
    int cnt;
    getline(file, line);
    userListSize = atoi(line.c_str()); // number of users
    for(int i = 0; i < userListSize; i++) {
        getline(file, line);
        string username = line;
        getline(file, line);
        string password = line;
        User tmp(username, password);
        userList.push_back(tmp); // save to vector
    }
}

// determine whether the user exists
bool Users::isExistedAuthor(const string &username) {
    for(auto & i : userList){
        if(i.getUsername() == username) {
            return true;
        }
    }
    return false;
}

// create a new user
bool Users::createUser(const string& username, const string& password) {
    if(!isExistedAuthor(username)) {
        User tmp(username, password);
        userList.push_back(tmp);
        userListSize++;
        return true;
    } else {
        return false;
    }
}

// save user
void Users::saveUser() {
    ofstream file("users.txt");
    if(!file.is_open()) {
        cout << "users.txt can not open in createUser function " << endl;
        exit(0);
    }
    file << userList.size() << endl; // write size first
    for(auto & i : userList) {
        file << i.getUsername() << endl;
        file << i.getPassword() << endl;
    }
    file.close();
}

// user login
bool Users::login(string username, string password) {
    for(int i = 0; i < userListSize; i ++) {
        if(!userList[i].login(username, password).empty()) {
            currentUser = username;

            return true;
        }
    }
    return false;
}

// search user
int Users::searchUser(string username) {
    for(int i = 0; i < userListSize; i ++) {
        if(userList[i].getUsername() == username)
            return i;
    }
    return -1;
}

// switch user
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

// logout
void Users::logout() {
    int i = searchUser(currentUser);
    if(i == -1)
        return;
    userList[i].logout();
    currentUser = "";
}

// get current directory
Directory* Users::getCurDir() {
    int i = searchUser(currentUser);
    if(i == -1)
        return nullptr;
    return userList[i].getCurDir();
}

// set current directory
void Users::setCurDir(Directory* dir) {
    int i = searchUser(currentUser);
    if(i == -1)
        return;
    userList[i].setCurDir(dir);
}

// check user
bool Users::check() {
    for(auto& it:userList) {
        if(it.check())
            return true;
    }
    return false;
}

// show directory
void Users::showDir() {
    Directory* curDir = getCurDir();
    curDir -> show();
}