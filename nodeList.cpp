//
// Created by 19150 on 2022/5/30.
//

#include "nodeList.h"
using namespace std;

INode cache;
string cacheFilename;

bool iNodeDistributeList[INODE_NUM];

INode::INode(){}

INode::INode(const INode &A)
{
    type = A.type;
    dir = A.dir;
    fileLen = A.fileLen;
    diskSize = A.diskSize;
    setTime = A.setTime;
    username = A.username;
    updateTime = A.updateTime;
    iNlink = A.iNlink;
    content = A.content;
    indexT = A.indexT;
}

INode::INode(int type, string setTime, string updateTime, string username, int fileLen, int diskSize, int i_Nlink, string content)
        : type(type)
        , setTime(setTime)
        , updateTime(updateTime)
        , username(username)
        , fileLen(fileLen)
        , diskSize(diskSize)
        , iNlink(i_Nlink)
        , content(content)
{

}

INode& INode::operator=(const INode& B)
{
    this->dir = B.dir;
    this->type = B.type;
    this->updateTime = B.updateTime;
    this->setTime = B.setTime;
    this->diskSize = B.diskSize;
    this->fileLen = B.fileLen;
    this->username = B.username;
    this->iNlink = B.iNlink;
    this->content = B.content;
    this->indexT = B.indexT;
    return *this;
}

void INode::updateFileSize()      //update the size of file
{
    if(type == 1)
        fileLen = sizeof(dir);
    else
        fileLen = sizeof(content);
}

int INode::size() const
{
    return fileLen;
}
int INode::disksize()
{
    return diskSize;
}

int INode::differ()          //the change of inode
{
    int t;
    if(type == 1)
    {
        t = sizeof(dir) - fileLen;
    }

    else
        t = sizeof(content) - fileLen;
    fileLen = t;
    return (int)ceil((double)t / (double)BLOCK_SIZE);
}

int INode::freeBlock()                //remove a block
{
    return indexT.dropIndex();
}

bool INode::addBlock(int id)
{
    return (indexT.addIndex(id)) ? (diskSize++, true):(false);
}

int INode::num()                     //get the number of disk
{
    return diskSize;
}

string INode::getUser()
{
    return username;
}

void INode::clear()
{
    type = -1;
    dir.clear();
    fileLen = 0;
    diskSize = 0;
//  setTime = ;
    username = "";
    iNlink = 0;
    content = "";
    indexT.clear();
}

void INodeList::updateInode(int id, INode ano)
{
    inodeList[id] = ano;
}

int INode::calculateFileSize(const string& filename)        //calculate the size of File
{
    ifstream file(filename);
    if(!file.is_open())
        return -1;
    string t;
    file >> noskipws >> t;
    return t.size();
}

int INode::check()                         //the count of one inode
{
    return iNlink;
}

int INodeList::getFreeInodeNum()
{
    for(int i=0; i<INODE_NUM; i++)
    {
        if(!iNodeDistributeList[i])
            return i;
    }
    return -1;
}

bool INodeList::addNewINode(INode A, int i) {
    inodeList[i] = A;
    iNodeSize++;
    return true;
}

void INodeList::freeInvalidInode(int pos) {
    iNodeDistributeList[pos] = false;
    inodeList[pos].clear();
    iNodeSize--;
}

INode& INodeList::getInode(int id)
{
    return inodeList[id];
}

void INode::show()
{
    indexT.show();
}


void INodeList::getSpecificInodeInfo(int pos) {              //show the information of inodeList
    cout << "the relevant info are as below:" << endl;
    cout << "the size of inode is " << inodeList[pos].size() << endl;
    cout << "the type of inode is" << inodeList[pos].getType() << endl;
    cout << "the inode belongs to " << inodeList[pos].getUser() << endl;
    cout << "the length of inode is" << inodeList[pos].size() << endl;
    cout << "the block num of inode is" << inodeList[pos].num() << endl;
    if(inodeList[pos].getType() == 0){
        cout << "the content of the file is: " << inodeList[pos].content << endl;
    }
    else if(inodeList[pos].getType() == 1){
        cout << "the component of the directory is: " << endl;
        inodeList[pos].dir.show();
    }
    cout << endl;
}

string INode::saveAsString() {
    string ans = "";
    ans =  username + "\n" + to_string(type) + "\n" +
           to_string(iNlink) + "\n" + to_string(fileLen) + "\n"
           + to_string(diskSize) + "\n" + setTime + updateTime;
    return ans;
}

int INode::getType() {
    return type;
}

void INode::addLink()
{
    iNlink++;
}

bool INode::delLink()
{
    iNlink--;
    if(iNlink == 0)
        return true;
    else
        return false;
}

void INode::saveDiskNumber(const vector<int>& numberSet) {
    for(int i : numberSet){
        indexT.addIndex(i);
    }
}

bool INode::inodeIsAuthor(string username) {
    if(getUser() == username)
        return true;
    return false;
}
