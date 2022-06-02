//
// Created by 22325 on 2022/5/30.
//

#include <bits/stdc++.h>
#include "GlobalVariable.h"
#include "nodeList.h"

#ifndef OS_BETA_LIST_H
#define OS_BETA_LIST_H

using namespace std;

class INodeListInRam {

private:
    const int iNodeListCapacity = MAXINODETNUM;    // inode capacity
    int iNodeListSize = 0;                          // inode list size
    INode iNodeList[MAXINODETNUM];                 // arraylist of inode
    int iNodeNum[MAXINODETNUM];
    int fileLock[MAXINODETNUM];                    // file lock: 1 read lock, 2 write lock

public:
    INodeListInRam();                               // init node
    int getFreeNode();                              // get null node
    bool loadNode(INode A, int id);                 // load a node
    INode freeNode(int id);                         // free a node
    int searchNode(int id);                         // search id of node
    INode& getNode(int id);                         // get a node
    bool setLock(string username, int lock, int id);// set lock by user
};

// FileOpenItem
class FileOpenItem {

private:
    unsigned offSet = 0;
    int linkN = 0;                                  // number of link
    int flags = 0;                                  // parameter of open file
    int mode = 0;                                   // parameter of create file
    int id = -1;                                    // node id

public:
    FileOpenItem();
    FileOpenItem(FileOpenItem& ano);
    FileOpenItem(int offset, int flags, int mode, int id);
    int getId();                                    // get node id
    int getOffset();                                // get offset
    bool setOffset(int offset);                     // set offset
    void clear();                                   // clear FileOpenItem
    int getLink();                                  // get link number
    void icrLink();                                 // add link
    void dcrLink();                                 // reduce link
    void show();                                    // show message
    int getMode();                                  // get mode
};

// system file open list
class FileOpenList {

private:
    const int fileOpenCapacity = SYSTEMFILEMAXNUM;       // max size of system file list
    int fileOpenSize = 0;                       // current number of system file list
    FileOpenItem fileOpenList[SYSTEMFILEMAXNUM];         // file item

public:
    void clear();                                           // reset open size
    int size();                                             // get size
    bool deleteLink(int id);                                // delete link
    bool addLink(int id);                                   // add link
    int getOffset(int id);                                  // get offset
    void setOffset(int id, int offset);                     // set offset
    bool deleteItem(int id);                                // delete item
    int addItem(int offset, int flags, int mode, int id);   // add item
    int getItemINode(int id);                               // get id of item
    int getFreeItem();                                      // get free item
    void show();                                            // show message
    int getMode(int id);                                    // get mode
};

class UserOpenItem {

private:
    int descriptor = -1;        // inode id
    int id = -1;                // corresponding id to the system file open table

public:
    UserOpenItem();
    UserOpenItem(int descriptor, int id);
    void clear();                           // reset
    void set(int descriptor, int id);       // set descriptor and id
    int getDescriptor();                    // get descriptor
    int getId();                            // get id
    bool check();                           // check availability
};

class UserOpenList {

private:
    string username;
    UserOpenItem iNodeToFile[USERFILEMAXNUM];

public:
    UserOpenList();
    explicit UserOpenList(string username);
    void clear();                                   // reset
    int getFileId(int iNodeId, int num = 1);        // get file id
    int searchId(int iNodeId, int num = 1);         // search id
    int searchFreeItem();                           // search free id
    bool addItem(int iNodeId, int fileId);          // add a item
    bool deleteItem(int iNodeId);                   // delete a item
    string getUserName();                           // get username
    int count(int iNodeId);                         //count times a file opened
    void show();                                    // show message
};

#endif //OS_BETA_LIST_H
