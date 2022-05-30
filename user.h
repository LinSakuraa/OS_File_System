//
// Created by 22325 on 2022/5/30.
//

#include "iostream"
#include "fstream"
#include "vector"
#include "string"
#include "inode.h"
#include "directory.h"

#ifndef OS_BETA_USER_H
#define OS_BETA_USER_H

using namespace std;

class User {
private:
    User();

    Directory* curDir = nullptr;
    string username;
    string password;
    bool state = false;

public:
    User(string basicString, string string1);

    User(string un, string pw, Directory* dir = nullptr);

    // 获取用户名
    string getUsername();

    // 获取密码
    string getPassword();

    // 登录
    string login(string un, string pw);

    // 读取当前目录
    Directory* getCurDir();

    // 修改当前目录项
    void setCurDir(Directory* dir);

    // 检查状态
    bool check();

    // 登出
    void logout();

    void clear();
};

extern string currentUser;

//把user的表读到用户数组中去
class Users {
private:
    int userListSize = 0;

public:
    vector<User> userList;

    //计算文件大小
    int calculateFileSize(const string& filename);

    // 读取用户列表
    void readUserList();

    //是否存在该用户
    bool isExistedAuthor(const string& username);

    //新建用户
    void createUser(const string& username, const string& password);

    // 保存用户
    void saveUser();

    // 用户登录
    bool login(string username, string password);

    // 搜索用户
    int searchUser(string username);

    // 切换用户
    bool switchUser(string username);

    // 用户登出
    void logout();

    // 获取当前目录
    Directory* getCurDir();

    // 设置当前目录
    void setCurDir(Directory* dir);

    bool check();

    void showDir();
};

#endif //OS_BETA_USER_H