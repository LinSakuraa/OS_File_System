//
// Created by LinSakura on 2022/5/30.
//
//#include "errorNotify.h"
#include "fileSystem.h"
fileSystem fileSys;
//super block functions
superBlock::superBlock()
{
    superGroup.init();
}
bool superBlock::createFile(const string& fileName, Directory *curDir)
{
    int i = iNodeList.getFreeInodeNum();
    if(i == -1)
    {
        nodeError(1);
        return false;
    }
    int idd = curDir->getItem(".");
    if(!iNodeList.getInode(idd).inodeIsAuthor(currentUser))
    {
        authenticateError(1);
        return false;
    }
    curDir->addItem(fileName, i);
    iNodeDistributeList[i] = true;
    int id = curDir->getItem(".");
    int n = iNodeList.getInode(id).differ();
    iNodeList.getInode(id).updateFileSize();
    while(n > 0)
    {
        int bid = superGroup.getFreeBlock();
        if(bid == -1)
        {
            blockError(1);
            return false;
        }
        iNodeList.getInode(id).addBlock(bid);
        n--;
    }
    while(n < 0)
    {
        int bid = iNodeList.getInode(id).freeBlock();
        superGroup.addNewBlock(bid);
        n++;
    }
    INode newInode(0, getCurrentTime(), getCurrentTime(), currentUser);
    if(iNodeList.addNewINode(newInode, i))
    {
        freeNode--;
    }
    else
        return false;
    return true;
}
void superBlock::deleteFile(const string& fileName, Directory &directory)
{
    int pos = -1;
    pos = directory.getItem(fileName);
    if (pos == -1)
    {
        fileError(1);
        return;
    }
    directory.deleteItem(fileName);
    iNodeDistributeList[pos] = false;
    iNodeList.freeInvalidInode(pos);
    freeNode++;
}
void superBlock::createDirectory(const string &directoryName, INode &dir, Directory *directory, int pos)
{
    iNodeList.iNodeSize++;
    iNodeDistributeList[pos] = true;
    iNodeList.inodeList[pos] = dir;
    directory->addItem(directoryName, pos);
}
void superBlock::deleteDirectory(const string &directoryName, INode &dir, Directory &directory, int pos)
{
    iNodeList.iNodeSize--;
    iNodeDistributeList[pos] = false;
    directory.deleteItem(directoryName);
    iNodeList.freeInvalidInode(pos);
}
void superBlock::createFileAndDirectory()
{

}
void superBlock::deleteFileAndDirectory()
{

}
//file system functions
void fileSystem::createFile(const string &fileName)
{
    Directory* curDir = users.getCurDir();
    if(curDir != nullptr)
    {
        if(curDir->checkItem(fileName))
        {
            cout << "the file '" << fileName << "' has already existed"<<endl;
            return ;
        }
        if(superBlock.createFile(fileName, curDir))
        {
            openFile(fileName, 0, 1);
            cout << "create successfully"<<endl;
            return;
        }
    }
    else
        authenticateError(1);
}
void fileSystem::saveInodeInfo()
{
    ofstream file("inodesInfo.txt");
    if(!file.is_open())
    {
        cout<<"inodesInfo.txt can not open in saveInodeInfo function "<<endl;
        exit(0);
    }
    for(bool i : iNodeDistributeList)
    {
        file<<i<<endl;
    }
    for(int i=0; i < INODENUM; i++)
    {
        if(iNodeDistributeList[i])
        {
            file << superBlock.iNodeList.inodeList[i].saveAsString();
            if(superBlock.iNodeList.inodeList[i].getType() == 1)
            {
                file<<to_string(superBlock.iNodeList.inodeList[i].dir.getNumFileDir())<<endl;
                file<<superBlock.iNodeList.inodeList[i].dir.saveStringDir();
            }
            else if(superBlock.iNodeList.inodeList[i].getType() == 0)
            {
                file << superBlock.iNodeList.inodeList[i].content << endl;
            }
        }
    }
    cout << "save successfully !" << endl;
    file.close();
}
void fileSystem::readInodeInfo()
{
    ifstream file("inodesInfo.txt");
    if(!file.is_open())
    {
        cout << "inodeInfo.txt can not open in readInodeInfo function" << endl;
        return ;
    }
    string line;
    for(bool & i : iNodeDistributeList)
    {
        file>>line;
        if(line == "1")
            i = true;
        else
            i = false;
    }
    for(int i=0; i < INODENUM; i++)
    {
        if(iNodeDistributeList[i])
        {
            string userName, setTime, updateTime;
            int type, i_Nlink, fileLen, diskSize;
            if(i != 0)
            {
                file >> userName;
            }
            cout << userName << " is user name" << endl;
            file >> line;
            type = atoi(line.c_str());
            cout << type << " is type" << endl;
            file >> line;;
            i_Nlink = atoi(line.c_str());
            cout << i_Nlink << " is i_link" << endl;
            file >> line;
            fileLen = atoi(line.c_str());
            cout << fileLen << " is file_len" << endl;
            file >> line;
            diskSize = atoi(line.c_str());
            cout << diskSize << " is disk size" << endl;
            getline(file, setTime);
            getline(file, setTime);
            getline(file, updateTime);
            cout << setTime <<" is set time " << endl;
            cout << updateTime << " is update time" << endl;
            INode A(type, setTime, updateTime, userName, fileLen, diskSize, i_Nlink);
            superBlock.iNodeList.inodeList[i] = A;
            superBlock.iNodeList.iNodeSize++;
            int dirSize;
            if(diskSize > 0){
                while(diskSize--){
                    int id = superBlock.superGroup.getFreeBlock();
                    superBlock.iNodeList.getInode(id).addBlock(id);
                }
            }
            //这一行之前已经存储完了inode的所有基础元素
            if(type == 1)
            {
                file >> line;
                dirSize = atoi(line.c_str());
                cout << "dir size is: " << dirSize<<endl;
                while(dirSize--)
                {
                    file >> line;
                    string key = line;
                    file >> line;
                    string value = line;
                    int val = atoi(value.c_str());
                    superBlock.iNodeList.getInode(i).dir.addItem(key, val);
                }
            }
            else if(type == 0)
            {
                getline(file, superBlock.iNodeList.getInode(i).content);
            }
        }

    }
}
bool fileSystem::openFile(string fileName, int sign, int mode)
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return false;
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return false;
    }
    if(!superBlock.iNodeList.getInode(id).inodeIsAuthor(currentUser))
    {
        authenticateError(1);
        return false;
    }
    if(superBlock.iNodeList.getInode(id).getType() == 1)
    {
        cd(fileName);
        return true;
    }
    int id_ram = iNodeListInRam.searchNode(id);
    if(id_ram == -1)
    {
        if(!iNodeListInRam.loadNode(superBlock.iNodeList.getInode(id), id))
            return false;
    }
    if(userOpenList[currentUser].getFileId(id) != -1)
    {
        cout << "do you want to open it separately? [Y/N]:"<<endl;
        char c;
        cin >> c;
        if(!(c == 'Y' || c == 'y')) // N0
        {
            int id_sys_list = userOpenList[currentUser].getFileId(id);
            if(!userOpenList[currentUser].addItem(id, id_sys_list))
            {
                return false;
            }
            return true;
        }
    }
    iNodeListInRam.getNode(id).addLink();
    int id_sys_list;
    if(sign == 1)
        id_sys_list = fileOpenList.addItem(iNodeListInRam.getNode(id).content.size(), 0, mode, id);
    else
        id_sys_list = fileOpenList.addItem(0, 0, mode, id);
    if(userOpenList.find(currentUser) == userOpenList.end())
    {
        UserOpenList t1(currentUser);
        userOpenList[currentUser] = t1;
    }
    if(!userOpenList[currentUser].addItem(id, id_sys_list))
    {
        return false;
    }
    cout << "open successfully"<<endl;
    return true;
}
bool fileSystem::closeFile(string fileName)
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return false;
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return false;
    }
    if(!superBlock.iNodeList.getInode(id).inodeIsAuthor(currentUser))
    {
        authenticateError(1);
        return false;
    }
    int num = userOpenList[currentUser].count(id);
    if(num > 1)
    {
        cout << "choose a process to continue[1-" << num <<"]:"<<endl;
        int n;
        cin >> n;
        if(n > num || n < 1)
            num = 1;
        num = n ;
    }
    int id_sys_list = userOpenList[currentUser].getFileId(id, num);
    userOpenList[currentUser].deleteItem(id);
    bool sign1 = fileOpenList.deleteLink(id_sys_list);
    bool sign;
    if(sign1)
    {
        sign = iNodeListInRam.getNode(id).delLink();
        if (sign)
        {
            INode t = iNodeListInRam.freeNode(id);
            superBlock.iNodeList.updateInode(id, t);
        }
    }
    cout << "close successfully"<<endl;
    return true;
}
string fileSystem::readFile(string fileName, int len)
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return "";
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return "";
    }
    if(superBlock.iNodeList.getInode(id).getUser() != currentUser)
    {
        authenticateError(1);
        return "";
    }
    int num = userOpenList[currentUser].count(id);
    if(num > 1)
    {
        cout << "choose a process to continue[1-" << num <<"]:"<<endl;
        int n;
        cin >> n;
        if(n > num || n < 1)
            num = 1;
        num = n ;
    }
    int id_sys_list = userOpenList[currentUser].getFileId(id, num);
    if(id_sys_list == -1)
        return "";
    int offset = fileOpenList.getOffset(id_sys_list);
    if(offset + len > iNodeListInRam.getNode(id).content.size())
        len = iNodeListInRam.getNode(id).content.size() - offset;
    fileOpenList.setOffset(id_sys_list, offset + len);
    string out = iNodeListInRam.getNode(id).content.substr(offset, len);
    out += "\n";
    return out;
}
bool fileSystem::writeFile(string fileName, string content)
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return false;
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return false;
    }
    if(superBlock.iNodeList.getInode(id).getUser() != currentUser)
    {
        authenticateError(1);
        return false;
    }
    int num = userOpenList[currentUser].count(id);
    if(num > 1)
    {
        cout << "choose a process to continue[1-" << num <<"]:"<<endl;
        int n;
        cin >> n;
        if(n > num || n < 1)
            num = 1;
        num = n;
    }
    int id_sys_list = userOpenList[currentUser].getFileId(id, num);
    if(id_sys_list == -1)
    {
        fileError(2);
        return false;
    }
    if(fileOpenList.getMode(id_sys_list) == 0)
    {
        fileError(3);
        return false;
    }
    int offset = fileOpenList.getOffset(id_sys_list);
    fileOpenList.setOffset(id_sys_list, offset + content.size());
    string in1 = iNodeListInRam.getNode(id).content.substr(0, offset);
    string in2 = iNodeListInRam.getNode(id).content.substr(offset, iNodeListInRam.getNode(id).content.size() - offset);
    string out = in1 + content + in2;
    iNodeListInRam.getNode(id).content = out;
    int n = (int)ceil((double)(sizeof(out) - iNodeListInRam.getNode(id).size()) / (double) BLOCKSIZE);
    while(n > 0)
    {
        int bid = superBlock.superGroup.getFreeBlock();
        if(bid == -1)
        {
            blockError(1);
            return false;
        }
        iNodeListInRam.getNode(id).addBlock(bid);
        n--;
    }
    while(n < 0)
    {
        int bid = iNodeListInRam.getNode(id).freeBlock();
        superBlock.superGroup.addNewBlock(bid);
        n++;
    }
    iNodeListInRam.getNode(id).updateFileSize();
    return true;
}
void fileSystem::showFileInfo(string fileName)
{

}
Directory* fileSystem::returnToParent()
{
    Directory* t = users.getCurDir();
    int id = t->getItem("..");
    t = &(superBlock.iNodeList.getInode(id).dir);
    users.setCurDir(t);
    return t;
}
void fileSystem::directoryCreate(const string &directoryName)
{
    int parent_id;
    Directory* t = users.getCurDir();
    if(t == nullptr)
    {
        authenticateError(1);
        return;
    }
    parent_id =t->getItem(".");
    int pos = superBlock.iNodeList.getFreeInodeNum();
    if(pos == -1)
    {
        nodeError(1);
        return ;
    }
    int cur_id = pos;
    INode newInode(1, getCurrentTime(), getCurrentTime(), currentUser);
    newInode.dir.init(cur_id, parent_id);
    if(superBlock.iNodeList.getInode(parent_id).inodeIsAuthor(currentUser))
    {
        superBlock.createDirectory(directoryName, newInode, users.getCurDir(), pos);
    }
    else
    {
        authenticateError(1);
        return;
    }
    users.setCurDir(&(superBlock.iNodeList.getInode(cur_id).dir));
}
void fileSystem::directoryDelete(const string &directoryName)
{
    Directory* t = users.getCurDir();
    if(t == nullptr)
    {
        authenticateError(1);
        return;
    }
    int pos = -1;
    pos = users.getCurDir()->getItem(directoryName);
    if(pos == -1)
    {
        directoryError(1);
        return ;
    }
    if(superBlock.iNodeList.getInode(pos).inodeIsAuthor(currentUser))
    {
        INode tmp = superBlock.iNodeList.getInode(pos);
        superBlock.deleteDirectory(directoryName, tmp, *users.getCurDir(), pos);
    }
}
void fileSystem::fileDelete(const string &fileName)
{
    Directory* t = users.getCurDir();
    if(t == nullptr)
    {
        authenticateError(1);
        return;
    }
    int pos = -1;
    pos = t->getItem(fileName);
    if(pos == -1)
    {
        fileError(1);
        return;
    }
    int num = userOpenList[currentUser].count(pos);
    while(num > 0)
    {
        closeFileForDelete(fileName);
        num--;
    }
    if(superBlock.iNodeList.getInode(pos).inodeIsAuthor(currentUser))
    {
        superBlock.deleteFile(fileName, *users.getCurDir());
    }
    else
    {
        authenticateError(1);
    }
}

void fileSystem::createRootDirectory()
{
    superBlock.iNodeList.iNodeSize++;
    iNodeDistributeList[0] = true;
    INode RootInode(1, getCurrentTime(), getCurrentTime(), currentUser);
    RootInode.dir.init(0, 0);
    superBlock.iNodeList.inodeList[0] = RootInode;
}
int fileSystem::createUserDirectory(string userName)
{
    int parent_id = 0;
    int pos = superBlock.iNodeList.getFreeInodeNum();
    if(pos == -1){
        nodeError(1);
        return -1;
    }
    int cur_id = pos;
    INode newInode(1, getCurrentTime(), getCurrentTime(), userName);
    newInode.dir.init(cur_id, parent_id);
    superBlock.iNodeList.iNodeSize++;
    iNodeDistributeList[pos] = true;
    superBlock.iNodeList.inodeList[pos] = newInode;
    superBlock.iNodeList.getInode(0).dir.addItem(userName, pos);
    return pos;
}
void fileSystem::init()
{
    createRootDirectory();
    users.createUser("admin", "admin");
    int i = createUserDirectory("admin");
    users.userList[0].setCurDir(&(superBlock.iNodeList.getInode(i).dir));
}
void fileSystem::cd(string directoryName)
{
    Directory* t = users.getCurDir();
    if(t == nullptr)
    {
        authenticateError(1);
        return;
    }
    int id = t->getItem(directoryName);
    if(id == -1)
    {
        directoryError(2);
        return;
    }
    if(superBlock.iNodeList.getInode(id).getType() == 0)
    {
        cdError(1);
        return;
    }
    users.setCurDir(&(superBlock.iNodeList.getInode(id).dir));
}
void fileSystem::formatFileSystem()
{
    superBlock.iNodeList.iNodeSize = 0;
    currentUser = "";
    for(bool & i : iNodeDistributeList)
    {
        i = false;
    }
    for(int i=0; i<superBlock.iNodeList.inodeList->size(); i++)
    {
        superBlock.iNodeList.inodeList[i].freeBlock();
    }
    for(auto & i : users.userList)
    {
        i.clear();
    }

    for(int i=0; i<users.userList.size(); i++)
    {
        iNodeListInRam.freeNode(i);
    }
}
void fileSystem::showDir()
{
    Directory* curDir = users.getCurDir();
    if(curDir == nullptr)
    {
        authenticateError(1);
        return;
    }
    for(int i=2; i<curDir->size(); i++){
        string fileName = curDir->getFileNameMap(i);
        if(fileName == "")
            return;
        int inodeId = curDir->getItem(fileName);
        cout << fileName << "   " << superBlock.iNodeList.inodeList[inodeId].getUser() << "   ";
        if(superBlock.iNodeList.inodeList[inodeId].getType() == 0)
        {
            cout << "file" << "   " << endl;
        }
        else{
            cout << "directory" << "   " << endl;
        }
    }
}
bool fileSystem::closeFileForDelete(string fileName)
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return false;
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return false;
    }
    int id_sys_list = userOpenList[currentUser].getFileId(id);
    userOpenList[currentUser].deleteItem(id);
    bool sign1 = fileOpenList.deleteLink(id_sys_list);
    bool sign;
    if(sign1)
    {
        sign = iNodeListInRam.getNode(id).delLink();
        if (sign)
        {
            INode t = iNodeListInRam.freeNode(id);
            superBlock.iNodeList.updateInode(id,t);
        }
    }
    return true;
}
void fileSystem::copy(string fileName)
{
    if(cacheFilename != "" || cache.getType() != -1)
    {
        clipboardError(1);
        return;
    }
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return;
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return;
    }
    if(superBlock.iNodeList.getInode(id).getType() == 1)
    {
        clipboardError(2);
        return;
    }
    if(!superBlock.iNodeList.getInode(id).inodeIsAuthor(currentUser))
    {
        authenticateError(1);
        return;
    }
    int num = userOpenList[currentUser].count(id);
    while(num > 0)
    {
        closeFileForDelete(fileName);
        num--;
    }
    INode t = superBlock.iNodeList.getInode(id);
    cacheFilename = fileName;
    cache = t;
}
void fileSystem::paste()
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return;
    }
    if(cacheFilename == "" || cache.getType() == -1){
        clipboardError(4);
        return;
    }
    if(dir->checkItem(cacheFilename))
    {
        cout << "the file '" << cacheFilename << "' has already existed"<<endl;
        return ;
    }
    int i = superBlock.iNodeList.getFreeInodeNum();
    if(i == -1)
    {
        nodeError(1);
        return;
    }
    dir->addItem(cacheFilename, i);
    iNodeDistributeList[i] = true;
    int id = dir->getItem(".");
    int n = superBlock.iNodeList.getInode(id).differ();
    superBlock.iNodeList.getInode(id).updateFileSize();
    while(n > 0)
    {
        int bid = superBlock.superGroup.getFreeBlock();
        if(bid == -1)
        {
            blockError(1);
            return;
        }
        superBlock.iNodeList.getInode(id).addBlock(bid);
        n--;
    }
    while(n < 0)
    {
        int bid = superBlock.iNodeList.getInode(id).freeBlock();
        superBlock.superGroup.addNewBlock(bid);
        n++;
    }
    if(superBlock.iNodeList.addNewINode(cache, i))
    {
        superBlock.freeNode--;
    }
    openFile(cacheFilename, 0, 1);
    cout << "paste successfully"<<endl;
    cache.clear();
    cacheFilename = "";
}
void fileSystem::fileRename(string &fileName, string &newName)
{
    Directory* curDir = users.getCurDir();
    if(curDir == nullptr)
    {
        authenticateError(1);
        return;
    }
    int pos = curDir->getItem(fileName);
    if(pos == -1)
    {
        fileError(1);
        return;
    }
    if(!superBlock.iNodeList.getInode(pos).inodeIsAuthor(currentUser))
    {
        authenticateError(1);
        return;
    }
    curDir->renameItem(fileName,newName);
}
void fileSystem::cut(string fileName)
{
    if(cacheFilename != "" || cache.getType() != -1)
    {
        clipboardError(1);
        return;
    }
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return;
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return;
    }
    if(!superBlock.iNodeList.getInode(id).getType() == 1)
    {
        clipboardError(3);
        return;
    }
    if(!superBlock.iNodeList.getInode(id).inodeIsAuthor(currentUser))
    {
        authenticateError(1);
        return;
    }
    int num = userOpenList[currentUser].count(id);
    while(num > 0)
    {
        closeFileForDelete(fileName);
        num--;
    }
    INode t = superBlock.iNodeList.getInode(id);
    cacheFilename = fileName;
    cache = t;
    fileDelete(fileName);
}
void fileSystem::fSeek(string fileName, int offset)
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return;
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return;
    }
    if(superBlock.iNodeList.getInode(id).getUser() != currentUser)
    {
        authenticateError(1);
        return;
    }
    if(superBlock.iNodeList.getInode(id).getType() == 1)
    {
        pointerError(1);
        return;
    }
    int num = userOpenList[currentUser].count(id);
    if(num > 1)
    {
        cout << "choose a process to continue[1-" << num <<"]:"<<endl;
        int n;
        cin >> n;
        if(n > num || n < 1)
            num = 1;
        num = n;
    }
    int id_sys_list = userOpenList[currentUser].getFileId(id, num);
    if(id_sys_list == -1)
    {
        fileError(4);
        return;
    }
    if(offset > iNodeListInRam.getNode(id).content.size())
        offset = iNodeListInRam.getNode(id).content.size();
    if(offset < 0)
        offset = 0;
    fileOpenList.setOffset(id_sys_list, offset);
}
void fileSystem::showFile(string fileName)
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return;
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return;
    }
    if(superBlock.iNodeList.getInode(id).getUser() != currentUser)
    {
        authenticateError(1);
        return;
    }
    INode t;
    if(iNodeListInRam.searchNode(id) == -1)
        t = superBlock.iNodeList.getInode(id);
    else
        t = iNodeListInRam.getNode(id);
    int size = t.disksize();
    cout << "the file " << fileName << " has use " << size << " blocks"<<endl;
    t.show();
}
void fileSystem::copy2(string fileName, string newName)
{
    if(cacheFilename != "" || cache.getType() != -1)
    {
        clipboardError(1);
        return;
    }
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return;
    }
    int id_a = dir->getItem(fileName);
    if(id_a == -1)
    {
        fileError(1);
        return;
    }
    if(superBlock.iNodeList.getInode(id_a).getType() == 1)
    {
        clipboardError(2);
        return;
    }
    if(!superBlock.iNodeList.getInode(id_a).inodeIsAuthor(currentUser))
    {
        authenticateError(1);
        return;
    }
    int num = userOpenList[currentUser].count(id_a);
    while(num > 0)
    {
        closeFileForDelete(fileName);
        num--;
    }

    if(dir->checkItem(newName))
    {
        int id_b = dir->getItem(newName);
        int num1 = userOpenList[currentUser].count(id_b);
        while(num1 > 0)
        {
            closeFileForDelete(newName);
            num1--;
        }
        superBlock.iNodeList.getInode(id_b).content = superBlock.iNodeList.getInode(id_a).content;
        openFile(newName, 0, 1);
    }
    else
    {
        int i = superBlock.iNodeList.getFreeInodeNum();
        if(i == -1)
        {
            nodeError(1);
            return;
        }
        dir->addItem(newName, i);
        iNodeDistributeList[i] = true;
        superBlock.iNodeList.addNewINode(superBlock.iNodeList.getInode(id_a), i);
        superBlock.freeNode--;
        openFile(newName, 0, 1);
    }
    cout << "copy successfully!"<<endl;
}
void fileSystem::show(string fileName)
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return;
    }
    int id = dir->getItem(fileName);
    if(id == -1)
    {
        fileError(1);
        return;
    }
    if(superBlock.iNodeList.getInode(id).getUser() != currentUser)
    {
        authenticateError(1);
        return;
    }
    if(superBlock.iNodeList.getInode(id).getType() == 1)
    {
        return;
    }
    if(iNodeListInRam.searchNode(id) != -1)
        cout << iNodeListInRam.getNode(id).content << endl;
    else
        cout << superBlock.iNodeList.getInode(id).content << endl;
}
void fileSystem::add(string fileName, string newName)
{
    Directory* dir = users.getCurDir();
    if(dir == nullptr)
    {
        authenticateError(1);
        return;
    }
    int id_a = dir->getItem(fileName);
    if(id_a == -1)
    {
        fileError(1);
        return;
    }
    if(superBlock.iNodeList.getInode(id_a).getType() == 1)
    {
        clipboardError(2);
        return;
    }
    if(!superBlock.iNodeList.getInode(id_a).inodeIsAuthor(currentUser))
    {
        authenticateError(1);
        return;
    }
    int num = userOpenList[currentUser].count(id_a);
    while(num > 0)
    {
        closeFileForDelete(fileName);
        num--;
    }

    if(dir->checkItem(newName))
    {
        int id_b = dir->getItem(newName);
        int num1 = userOpenList[currentUser].count(id_b);
        while(num1 > 0)
        {
            closeFileForDelete(newName);
            num1--;
        }
        superBlock.iNodeList.getInode(id_b).content += superBlock.iNodeList.getInode(id_a).content;
        openFile(newName, 0, 1);
    }
    else
    {
        int i = superBlock.iNodeList.getFreeInodeNum();
        if(i == -1)
        {
            nodeError(1);
            return;
        }
        dir->addItem(newName, i);
        iNodeDistributeList[i] = true;
        superBlock.iNodeList.addNewINode(superBlock.iNodeList.getInode(id_a), i);
        superBlock.freeNode--;
        openFile(newName, 0, 1);
    }
}