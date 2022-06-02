//
// Created by 19150 on 2022/5/30.
//

#include "blocksControl.h"

GroupLeader::GroupLeader():nextLeader(nullptr){}

GroupLeader::GroupLeader(GroupLeader* ano):nextLeader(ano){}

GroupLeader::GroupLeader(GroupLeader& ano)        //copy
{
    this->groupSize = ano.groupSize;
    this->nextLeader = ano.nextLeader;
    this->groups = ano.groups;
}

int GroupLeader::size() const
{
    return groupSize;
}

int GroupLeader::get()             //get the block number of a free block
{
    int t = groups.top();
    groups.pop();
    groupSize--;
    return t;
}

bool GroupLeader::add(int id)
{
    if(groupSize >= groupCapacity)
        return false;
    else
    {
        groups.push(id);
        groupSize++;
        return true;
    }
}

GroupLeader* GroupLeader::self()                //return the self
{
    return this;
}

GroupLeader* GroupLeader::nextSelf()
{
    return nextLeader;
}

bool GroupLeader::setNextLeader(GroupLeader* ano)       //Set next leader block
{
    nextLeader = ano;
    return true;
}

int superGroup::getFreeBlock()
{
    if(groupTotalSize <= 0)
        return -1;
    if(curGroup->size() <= 0)
        curGroup = curGroup->nextSelf();
    int t = curGroup->get();
    groupTotalSize--;
    return t;
}

bool superGroup::addNewBlock(int id)            //Return a freed block
{
    if(curGroup->size() >= 50)
    {
        GroupLeader* t = curGroup->self();
        curGroup = new GroupLeader(t);
    }
    curGroup->add(id);
    groupTotalSize++;
    return true;
}

int superGroup::size() const                    //get the number of free block
{
    return groupTotalSize;
}

void superGroup::init()                         //init the superGroup
{
    for(int i = 0; i < TOTALGROUPSIZE / GROUPSIZE; i++)   // 500/50
    {
        GroupLeader* t;
        if(i == 0)              // Assign the first declared leader block to the current superblock
        {
            t = new GroupLeader;
            curGroup = t;
        }
        else
        {
            auto *t1 = new GroupLeader;         // new groupLeader
            t->setNextLeader(t1);
            t = t1;
        }
        for(int j = GROUPSIZE; j >= 1 ; j--)
        {
            t->add(i * GROUPSIZE + j);
        }
    }
}

void superGroup::show()                         //show the information
{
    int i;
    cout << "the size of superGroup" << curGroup->size() << endl;
    cout << "the free block in the top of stack" << (i = getFreeBlock()) << endl;
    addNewBlock(i);
}
