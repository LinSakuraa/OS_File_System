//
// Created by LinSakura on 2022/5/30.
//
#include <bits/stdc++.h>
#ifndef OS_BETA_DIRECTORY_H
#define OS_BETA_DIRECTORY_H
using namespace std;
class Directory{
private:
    map<string,int> directory;  //map of directory
public:
    Directory();
    Directory(Directory&);      //init functions
    Directory& operator = (const Directory&);
    //directory function
    void show();                                        //show
    bool addItem(string fileName,int id);               //add an item
    bool deleteItem(string fileName);                   //delete an item
    bool checkItem(string fileName);                    //check if an item already exist
    int getItem(string fileName);                       //get the int of an item
    bool renameItem(string fileName,string newName);    //rename an item
    int size();                                         //the size of item
    void init(int idSelf,int idParent);                 //init the dir
    void clear();                                       //clear the dir
    int getNumFileDir();                                //get the num of files in dir
    string saveStringDir();                             //change vector to string
    string getFileName(int id);                         //get the file name of id
    string getFileNameMap(int pos);                     //get the file name of position in map
};
#endif //OS_BETA_DIRECTORY_H
