//
// Created by 22325 on 2022/5/30.
//

#include "list.h"

// init node
INodeListInRam::INodeListInRam() {
    for(int i = 0; i < MAXINODETNUM; i++)
        iNodeNum[i] = -1, fileLock[i] = 0;
}

// get empty node
int INodeListInRam::getFreeNode() {
    for(int i = 0; i < MAXINODETNUM; i++) {
        if(!iNodeList[i].check()) // select iNodes without any connections
            return i;
    }
    return -1;
}

// load a node
bool INodeListInRam::loadNode(INode A, int id) {
    int i = getFreeNode();
    if(i == -1) // no empty node
        return false;
    iNodeList[i] = A;
    iNodeNum[i] = id;
    iNodeListSize++;
    return true;
}

// free a node
INode INodeListInRam::freeNode(int id) {
    INode t;
    int i = searchNode(id); // find this node
    if(i == -1)
        return t;
    t = iNodeList[i];
    iNodeList[i].clear(); // free space
    iNodeNum[i] = -1;
    fileLock[i] = 0;
    iNodeListSize--;
    return t;
}

// search index of node
int INodeListInRam::searchNode(int id) {
    for(int i = 0; i < MAXINODETNUM; i++)
        if(iNodeNum[i] == id)
            return i;
    return -1;
}

// get a node
INode& INodeListInRam::getNode(int id) {
    int i = searchNode(id);
    return iNodeList[i];
}

// set lock by user
bool INodeListInRam::setLock(string username, int lock, int id) {
    if(lock < 0 || lock > 2)
        return false;
    int i = searchNode(id);
    if(iNodeList[i].getUser() == username)
        fileLock[i] = lock;
    else
        return false;
    return true;
}

FileOpenItem::FileOpenItem() { }

FileOpenItem::FileOpenItem (FileOpenItem &ano)
        : offSet(ano.offSet)
        , linkN(ano.linkN)
        , flags(ano.flags)
        , mode(ano.mode)
        , id(ano.id) { }

FileOpenItem::FileOpenItem (int offset, int flags, int mode, int id)
        : offSet(offset)
        , linkN(1)
        , flags(flags)
        , mode(mode)
        , id(id) { }

// get node id
int FileOpenItem::getId() {
    return id;
}

// get offset
int FileOpenItem::getOffset() {
    return offSet;
}

// set offset
bool FileOpenItem::setOffset(int offset) {
    this -> offSet = offset;
    return true;
}

// clear FileOpenItem
void FileOpenItem::clear() {
    offSet = 0;
    flags = 0;
    mode = 0;
    linkN = 0;
    id = -1;
}

// get link number
int FileOpenItem::getLink() {
    return linkN;
}

// add link
void FileOpenItem::icrLink() {
    linkN++;
}

// reduce link
void FileOpenItem::dcrLink() {
    linkN--;
}

//show message
void FileOpenItem::show() {
    cout << "offset = " << offSet << ", linkN = " << linkN << ", flag = " << flags << ", mode = " << mode << ", id = " << id;
}

// get mode
int FileOpenItem::getMode() {
    return mode;
}

// reset open size
void FileOpenList::clear() {
    for(int i = 0; i < SYSTEMFILEMAXNUM; i++) {
        if(fileOpenList[i].getLink() > 0)
            fileOpenList[i].clear();
    }
    fileOpenSize = 0;
}

// get size
int FileOpenList::size() {
    return fileOpenSize;
}

// delete link
bool FileOpenList::deleteLink(int id) {
    if(fileOpenList[id].getLink() <= 0)
        return false;
    fileOpenList[id].dcrLink();
    if(fileOpenList[id].getLink() == 0) {
        deleteItem(id);
        return true;
    }
    return false;
}

// add link
bool FileOpenList::addLink(int id) {
    if(fileOpenList[id].getLink() < 0)
        return false;
    fileOpenList[id].icrLink();
    return true;
}

// get offset
int FileOpenList::getOffset(int id) {
    return fileOpenList[id].getOffset();
}

// set offset
void FileOpenList::setOffset(int id, int offset) {
    fileOpenList[id].setOffset(offset);
}

// delete item
bool FileOpenList::deleteItem(int id) {
    if(fileOpenList[id].getLink() < 0)
        return false;
    fileOpenList[id].clear();
    fileOpenSize--;
    return true;
}

// add item
int FileOpenList::addItem(int offset, int flags, int mode, int id) {
    int i = getFreeItem();
    if(i == -1)
        return -1;
    FileOpenItem t(offset, flags, mode, id);
    fileOpenList[i] = t;
    fileOpenSize++;
    return i;
}

// get id of item
int FileOpenList::getItemINode(int id) {
    if(fileOpenList[id].getLink() == 0)
        return -1;
    else
        return fileOpenList[id].getId();
}

// get free item
int FileOpenList::getFreeItem() {
    for(int i = 0; i < SYSTEMFILEMAXNUM; i++)
        if(fileOpenList[i].getLink() == 0)
            return i;
    return -1;
}

// show message
void FileOpenList::show() {
    cout << "\n——————————————————————\n";
    for(int i = 0; i < fileOpenCapacity; i ++)
        if(fileOpenList[i].getLink() != 0)
        {
            fileOpenList[i].show();
            cout << endl;
        }
    cout << "\n——————————————————————\n";
}

// get mode
int FileOpenList::getMode(int id) {
    return fileOpenList[id].getMode();
}

UserOpenItem::UserOpenItem() { }

UserOpenItem::UserOpenItem(int descriptor, int id) : descriptor(descriptor), id(id) { }

// reset
void UserOpenItem::clear() {
    descriptor = -1;
    id = -1;
}

// set descriptor and id
void UserOpenItem::set(int descriptor, int id) {
    this -> descriptor = descriptor;
    this -> id = id;
}

// get descriptor
int UserOpenItem::getDescriptor() {
    return descriptor;
}

// get id
int UserOpenItem::getId() {
    return id;
}

//check availability
bool UserOpenItem::check() {
    if(descriptor < 0)
        return false;
    return true;
}

UserOpenList::UserOpenList() { }

UserOpenList::UserOpenList(string username) : username(username) { }

// reset
void UserOpenList::clear() {
    username = "";
    for(int i = 0; i < USERFILEMAXNUM; i++)
        iNodeToFile[i].clear();
}

// get file id
int UserOpenList::getFileId(int iNodeId, int num) {
    int id = searchId(iNodeId, num);
    if(id == -1)
        return -1;
    return iNodeToFile[id].getId();
}

// search id
int UserOpenList::searchId(int iNodeId, int num) {
    for(int i = 0; i < USERFILEMAXNUM; i++)
        if(iNodeToFile[i].getDescriptor() == iNodeId) {
            num--;
            if(num == 0)
                return i;
        }
    return -1;
}

// search free id
int UserOpenList::searchFreeItem() {
    for(int i = 0; i < USERFILEMAXNUM; i++)
        if(iNodeToFile[i].check() == false)
            return i;
    return -1;
}

// add a item
bool UserOpenList::addItem(int iNodeId, int fileId) {
    int id = searchFreeItem();
    if(id == -1)
        return false;
    UserOpenItem t(iNodeId, fileId);
    iNodeToFile[id] = t;
    return true;
}

// delete a item
bool UserOpenList::deleteItem(int iNodeId) {
    int id = searchId(iNodeId);
    if(id == -1)
        return false;
    iNodeToFile[id].clear();
    return true;
}

// get username
string UserOpenList::getUserName() {
    return username;
}

// count times a file opened
int UserOpenList::count(int iNodeId) {
    int count = 0;
    for(int i = 0; i < USERFILEMAXNUM; i++)
        if(iNodeToFile[i].getDescriptor() == iNodeId)
            count++;
    return count;
}

// show message
void UserOpenList::show() {
    for(int i = 0; i < USERFILEMAXNUM; i++)
        if(iNodeToFile[i].getDescriptor() != -1 && iNodeToFile[i].getId() != -1)
            cout << iNodeToFile[i].getDescriptor() << " " << iNodeToFile[i].getId() << endl;
}