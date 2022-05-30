//
// Created by 19150 on 2022/5/30.
//

#include "inode.h"
using namespace std;

INode cache;
string cacheFilename;

//模拟i结点的位示图，用来分配的时候查看i结点的空闲情况
bool iNodeDistributeList[INODE_NUM];

INode::INode() {}
//复制构造函数，方便i结点表拷贝到内存的i结点表中
INode::INode(const INode &A) {
    type = A.type;
    dir = A.dir;
    fileLen = A.fileLen;
    diskSize = A.diskSize;
    setTime = A.setTime;
    username = A.username;
    updateTime = A.updateTime;
    iNlink = A.iNlink;
    content =A.content;
    indexT = A.indexT;
}
//赋值构造函数
INode::INode(int type,string setTime,string updateTime,string updateTime, string username, int fileLen, int diskSize, int iNlink, string content)
       :type(type),setTime(setTime),updateTime(updateTime),username(username),fileLen(fileLen),diskSize(diskSize),i_Nlink(iNlink),content(content)
{

}
//重载等号运算符，返回一个引用对象
INode& INode::operator=(const INode &B) {
    this->dir = B.dir;
    this->type = B.type;
    this->fileLen = B.fileLen;
    this->diskSize = B.diskSize;
    this->setTime = B.setTime;
    this->username = B.username;
    this->updateTime = B.updateTime;
    this->iNlink = B.Nlink;
    this->content =B.content;
    this->indexT = B.indexT;
    return *this;
}
//更新i结点的相关信息
void INode::updateFileSize() {
    if(type == 1)
        fileLen = sizeof(dir);
    else
        fileLen = sizeof(content);
}
//返回文件大小
int INode::size() const {
    return fileLen;
}
int INode::disksize(){
    return diskSize;
}
//返回文件变化情况
int INode::differ() {
    int t;
    if(type == 1)
    {
        t = sizeof(dir)-fileLen;
    }
    else
    {
        t = sizeof(content)-fileLen;
    }
    fileLen = t;
    return (int)ceil((double)t/(double)BLOCK_SIZE);
}
//移除一个块
bool INode::freeBlock() {
    return indexT.dropIndex();
}
//添加一个块
bool INode::addBlock(int id) {
    return (indexT.addIndex(id))?(diskSize++,true):(false);
}
//获取块数
int INode::num() {
    return diskSize;
}
//获取用户名
string INode::getUser() {
    return username;
}
//清空inode
void INode::clear() {
    type = -1;
    dir.clear();
    fileLen = 0;
    diskSize = 0;
    username = "";
    iNlink = 0;
    content = "";
    indexT.clear();
}
//更新i结点
void INode::updateInode(int id,INode ano) {
    inodeList[id] = ano;
}
//计算i结点对应文件内容的大小
int INode::calculateFileSize(const int &filename) {
    ifstream file(filename);
    if(!file.is_open())
        return -1;
    string t;
    file >> noskipws >>t;
    return t.size();
}
//获取硬连接数
int INode::check() {
    return iNlink;
}
//获取i结点表的空闲相编号
int INodeList::getFreeInodeNum() {
    for(int i=0;i<INODE_NUM;i++)
    {
        if(!iNodeDistributeList[i])
            return i;
    }
    return -1;
}
//添加新的i结点
bool INodeList::addNewInode(INode A, int i) {
    iNodeList[i] = A;
    iNodeSize--;
}

//释放无效结点
void INodeList::FreeInvalidInode(int pos) {
    iNodeDistributeList[pos] = false;
    iNodeList[pos].clear();
    iNodeSize--;
}

INode& INodeList::getInode(int id) {
    return iNodeList[id];
}
void INode::show()
{
    indexT.show();
}

//显示所有i结点的信息
void INodeList::getSpecificInodeInfo(int pos) {
    cout << "the relevent info are as below " << endl;
    cout << "the size of inode is" << iNodeList[pos].size()<<endl;
    cout << "the type of inode is" << iNodeList[pos].getType()<<endl;
    cout << "the inode belongs to " << iNodeList[pos].getUser()<<endl;
    cout << "the length of inode is" << iNodeList[pos].size()<<endl;
    cout << "the block num of inode is" << iNodeList[pos].num()<<endl;
    if(iNodeList[pos].getType()==0)
    {
        cout<<"the content of the file is :" <<iNodeList[pos].content << endl;
    }
    else if(iNodeList[pos].getType()==1)
    {
        cout<<"the component of the file is :" << endl;
        iNodeList[pos].dir.show();
    }
    cout << endl;
}

string INode::saveAsString() {
    string ans="";
    ans = username + "\n" + to_string(type) + "\n" +to_string(iNlink)+ "\n"+to_string(fileLen)+"\n"+to_string(diskSize)+ "\n"+setTime +updateTime;
    return ans;
}

int INode::getType() {
    return type;
}

void INode::addLink() {
    iNlink++;
}
bool INode::delLink() {
    iNlink--;
    if(iNlink==0)
        return true;
    else
        return false;
}

void INode::saveDiskNumber(const vector<int> &numberSet) {
    for(int i:numberSet){
        indexT.addIndex(i);
    }
}

bool INode::inodeIsAuthor(int username) {
    if(getUser()== username)
        return true;
    else
        return false;
}