//
// Created by 19150 on 2022/5/30.
//
#include <stack>
#include <iostream>
#include "GlobalVariable.h"

#ifndef OS_BETA_BLOCKSCONTROL_H
#define OS_BETA_BLOCKSCONTROL_H

using namespace std;

class GroupLeader{
private:
    int groupCapacity = GROUP_SIZE;         //50 maximum group capacity
    int groupSize = 0;                      //the size of group
    GroupLeader* nextLeader;                //next leader block
    stack<int> groups;

public:
    GroupLeader();
    GroupLeader(GroupLeader* ano);
    GroupLeader(GroupLeader& ano);
    int size() const;
    int get();                                  //get the block number of a free block
    bool add(int id);
    GroupLeader* self();
    GroupLeader* nextSelf();
    bool setNextLeader(GroupLeader*);           //set next leader block
};

class superGroup{
private:
    GroupLeader* curGroup;
    int groupTotalSize = TOTAL_GROUP_SIZE;      //500  free block

public:
    int getFreeBlock();
    bool addNewBlock(int id);                   //Return a freed block
    int size() const;                           //get the number of free block
    void init();                                //init the superGroup
    void show();                                //show the information
};


#endif //OS_BETA_BLOCKSCONTROL_H
