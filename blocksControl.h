//
// Created by 19150 on 2022/5/30.
//
#include <stack>
#include <iostream>
#include "GlobalVariable.h"

#ifndef OS_FILE_SYSTEM_GCM_H
#define OS_FILE_SYSTEM_GCM_H

using namespace std;

// 组长块
class GroupLeader{
private:
    // 组容量上限
    int groupCapacity = groupSize;
    // 组实际大小
    int groupSize = 0;
    // 下一个组长块
    GroupLeader* nextLeader;
    // 组内的块
    stack<int> groups;

public:
    // 构造函数
    GroupLeader();
    // 使用指针构造函数
    GroupLeader(GroupLeader* ano);
    // 复制构造函数
    GroupLeader(GroupLeader& ano);
    // 获取当前组大小
    int size() const;
    // 获取一个空闲块的块号
    int get();
    // 往组里添加一个空闲块
    bool add(int id);
    //返回自身的地址
    GroupLeader* self();
    //返回下一个组长块地址
    GroupLeader* nextSelf();
    // 设置组长块;
    bool setNextLeader(GroupLeader*);
};

//超级块
class superGroup{
private:
    // 超级栈中当前的组长块
    GroupLeader* curGroup;
    // 当前总共可用的空闲块
    int groupTotalSize = totalSize;

public:
    // 获得一个空闲块
    int getFreeBlock();
    // 返回一个被释放的块
    bool addNewBlock(int id);
    // 返回剩余总块数
    int size() const;
    // 初始化超级栈
    void init();
    // 显示信息
    void show();
};


#endif //OS_FILE_SYSTEM_GCM_H
