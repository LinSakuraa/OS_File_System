//
// Created by 19150 on 2022/5/30.
//

#include <bits/stdc++.h>
#include "GlobalVariable.h"
#include "fileDirectory.h"
#include "index.h"


#ifndef OS_BETA_NODELIST_H
#define OS_BETA_NODELIST_H

extern bool iNodeDistributeList[INODE_NUM];

class INode{
private:
    string username = "";    //the user of i
    int type = -1;           //init the type (file 0,directory 1)
    int iNlink = 0;          //the count of inode
    int fileLen = 0;          // the length of file
    int diskSize = 0;        //the count of disk of the inode of the file
    string setTime = "";
    string updateTime = "";
    MixIndex indexT;          //the mixIndex

public:
    Directory dir;            //if directory is new dir,if file is null
    string content;           //the content of file
    INode();
    INode(const INode &A);    //copy inode to inodeList
    INode(int type, string setTime, string updateTime, string username, int fileLen = 0, int diskSize = 0, int i_Nlink = 0, string content = "");
    INode &operator=(const INode& B);
    void updateFileSize();    //update the fileSize of inode
    int size() const;
    int disksize();           //the diskSize
    int differ();             //the change
    int freeBlock();          //free a block
    bool addBlock(int id);    //add a block
    int num();                //the num of disk
    string getUser();
    void clear();
    void show();
    void addLink();
    bool delLink();
    int check();
    int calculateFileSize(const string& filename);
    bool inodeIsAuthor(string username);    //judge user permissions
    string saveAsString();
    int getType();                          //get the type of inode
    void saveDiskNumber(const vector<int>& numberSet);   //save diskNumber to IndexT
};

class INodeList{
public:
    int iNodeSize = 0;                   //number of inodes used
    INode inodeList[INODE_NUM];
    int getFreeInodeNum();               //get empty inode number
    bool addNewINode(INode A, int i);
    void freeInvalidInode(int pos);
    void getSpecificInodeInfo(int pos);
    void updateInode(int id, INode ano);   //update the inode
    INode& getInode(int id);              //get the inode
};

extern INode cache;
extern string cacheFilename;

#endif //OS_BETA_NODELIST_H
