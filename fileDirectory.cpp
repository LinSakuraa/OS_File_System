//
// Created by LinSakura on 2022/5/30.
//
#include "fileDirectory.h"
using namespace std;

Directory::Directory() {}
Directory::Directory(Directory & ano) :directory(ano.directory){}
Directory& Directory::operator=(const Directory & ano)
{
    directory = ano.directory;
    return *this;
}
void Directory::show()
{
    for(auto& it: directory)
    {
        cout << it.first << " " << it.second << endl;
    }
}
bool Directory::addItem(string fileName, int id)
{
    if(directory.find(fileName) != directory.end())
        return false;
    else
        directory[fileName] = id;
    return true;
}
bool Directory::deleteItem(string fileName)
{
    if(directory.find(fileName) == directory.end())
        return false;
    else
        directory.erase(fileName);
    return true;
}
bool Directory::checkItem(string fileName)
{
    return directory.find(fileName) != directory.end();
}
int Directory::getItem(string fileName)
{
    if(directory.find(fileName) == directory.end())
        return -1;
    else
        return directory[fileName];
}
bool Directory::renameItem(string fileName, string newName)
{
    if(directory.find(fileName) == directory.end() || directory.find(newName) != directory.end())
        return false;
    else
    {
        directory[newName] = directory[fileName];
        directory.erase(fileName);
        return true;
    }
}
int Directory::size()
{
    return directory.size();
}
void Directory::init(int idSelf, int idParent)
{
    directory["."] = idSelf;
    directory[".."] = idParent;
}
void Directory::clear()
{
    directory.clear();
}
int Directory::getNumFileDir()
{
    map<string, int>::iterator iter;
    int count = 0;
    for(iter=directory.begin(); iter!=directory.end(); iter++){
        count++;
    }
    return count;
}
string Directory::saveStringDir()
{
    map<string, int>::iterator iter;
    string temp;
    for(iter=directory.begin(); iter!=directory.end(); iter++){
        temp += (iter->first + '\n');
        temp += (to_string(iter->second) + '\n');
    }
    return temp;
}
string Directory::getFileName(int id)
{
    for(auto& it:directory)
    {
        if(it.second == id)
            return it.first;
    }
    return "";
}
string Directory::getFileNameMap(int pos)
{
    int count = 0;
    for(auto &it : directory)
    {
        if(count == pos){
            return it.first;
        }
        else{
            count++;
        }
    }
    return "";
}
