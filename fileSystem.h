//
// Created by LinSakura on 2022/5/30.
//
//main module of the project
#include "fileDirectory.h"
#include "nodeList.h"
#include "GlobalVariable.h"
#include "index.h"
#include "userControl.h"
#include "supportControl.h"
#include "list.h"
#include "blocksControl.h"
#include "errorNotify.h"
#ifndef OS_BETA_FILESYSTEM_H
#define OS_BETA_FILESYSTEM_H
class superBlock{
private:

public:
    int iNodeNum = INODE_NUM;           //total num of i node
    int freeNode = INODE_NUM;           //num of free i node
    int disk = TOTAL_GROUP_SIZE;        //total num of disk blocks
    int freeDisk = TOTAL_GROUP_SIZE;    //num of free disk blocks
    INodeList iNodeList;
    superGroup superGroup;
    //functions
    superBlock();
    bool createFile(const string& fileName,Directory* curDir);                                      //create file in superBlock
    void deleteFile(const string& fileName,Directory& directory);                                   //delete file in superBlock
    void createDirectory(const string &directoryName, INode &dir, Directory* directory, int pos);   //create directory in superBlock
    void deleteDirectory(const string& directoryName, INode& dir, Directory& directory, int pos);   //delete directory in superBlock
    void createFileAndDirectory();                                                                  //create i node for files & directories
    void deleteFileAndDirectory();                                                                  //delete i node for files & directories
};

class fileSystem{
private:
public:
    superBlock superBlock;                  //superBlock
    Users users;                            //user list
    INodeListInRam iNodeListInRam;          //i node list
    FileOpenList fileOpenList;
    map<string,UserOpenList> userOpenList;
    //functions
    void createFile(const string& fileName);                    //create file
    void saveInodeInfo();                                       //save all i node when user quit
    void readInodeInfo();                                       //read all i node when user login
    bool openFile(string fileName, int sign = 0, int mode = 0); //open file
    bool closeFile(string fileName);                            //close file(only this directory)
    string readFile(string fileName, int len);                  //read file
    bool writeFile(string fileName, string content);            //write file
    void showFileInfo(string fileName);                         //show the information of file
    Directory* returnToParent();                                //return to the parent directory
    void directoryCreate(const string &directoryName);          //create a directory
    void directoryDelete(const string &directoryName);          //delete the directory
    void fileDelete(const string &fileName);                    //delete file
    void createRootDirectory();                                 //create root directory
    int createUserDirectory(string userName);                   //create user directory
    void init();                                                //init all
    void cd(string directoryName);                              //output the directory
    void formatFileSystem();                                    //format the file system
    void showDir();                                             //show the directory of file
    bool closeFileForDelete(string fileName);                   //close one file and free its i node
    void copy(string fileName);                                 //copy file
    void paste();                                               //paste file
    void fileRename(string &fileName, string &newName);         //rename the file
    void cut(string fileName);                                  //cut file
    //not for user direct using
    void fSeek(string fileName, int offset);                    //set the pointer
    void showFile(string fileName);                             //show the file name
    void copy2(string fileName, string newName);                //copy x
    void show(string fileName);                                 //show x
    void add(string fileName, string newName);                  //add file
};
extern fileSystem fileSys;
#endif //OS_BETA_FILESYSTEM_H
