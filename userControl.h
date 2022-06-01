//
// Created by 22325 on 2022/5/30.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nodeList.h"
#include "fileDirectory.h"

#ifndef OS_BETA_USERCONTROL_H
#define OS_BETA_USERCONTROL_H

using namespace std;

class User {
private:
    Directory* curDir = nullptr;
    string username;
    string password;
    bool state = false;

public:
    User();
    User(string un, string pw, Directory* dir = nullptr);
    string getUsername();                       // get username
    string getPassword();                       // get password
    string login(string un, string pw);         // user login
    Directory* getCurDir();                     // get current directory
    void setCurDir(Directory* dir);             // set current directory
    bool check();                               // check state
    void logout();                              // user logout
    void clear();                               // delete user
};

extern string currentUser;

class Users {
private:
    int userListSize = 0;

public:
    vector<User> userList;
    int calculateFileSize(const string& filename);                      // calculate file size
    void readUserList();                                                // read user list
    bool isExistedAuthor(const string& username);                       // determine whether the user exists
    void createUser(const string& username, const string& password);    // create a new user
    void saveUser();                                                    // save user
    bool login(string username, string password);                       // user login
    int searchUser(string username);                                    // search user
    bool switchUser(string username);                                   // switch user
    void logout();                                                      // logout
    Directory* getCurDir();                                             // get current directory
    void setCurDir(Directory* dir);                                     // set current directory
    bool check();                                                       // check user
    void showDir();                                                     // show directory
};

#endif //OS_BETA_USERCONTROL_H
