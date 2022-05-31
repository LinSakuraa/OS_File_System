//
// Created by 19150 on 2022/5/30.
//
#include "index.h"

// 构造函数
NormalIndex::NormalIndex(int sign):sign(sign){}
// 复制构造函数
NormalIndex::NormalIndex(const NormalIndex& ano)
        :sign(ano.sign)
        , indexSize(ano.indexSize)
        , indexes(ano.indexes)
        , indexTables(ano.indexTables)
{}
// 重载等号运算符
NormalIndex& NormalIndex::operator=(const NormalIndex& ano)
{
    sign = ano.sign;
    indexSize = ano.indexSize;
    indexes = ano.indexes;
    indexTables = ano.indexTables;
    return *this;
}

// 获取第n个索引
int NormalIndex::getIndex(int n)
{
    if(n >= indexSize || n < 0) // n超出范围
        return -1;
    else
        return indexes[n];
}

// 添加磁盘块号为id的新索引
bool NormalIndex::addIndex(int id)
{
    if(id < 0 || indexSize >= indexCapacity) // id超出范围或索引块已满
        return false;
    else
    {
        indexes.push_back(id);
        indexSize++;
        return true;
    }
}

// 获取索引块中索引的数量
int NormalIndex::size() const
{
    return indexSize;
}

// 从索引块中删除n个索引
bool NormalIndex::dropIndex(int n)
{
    if(n > indexSize || n <= 0) // n超出范围
        return false;
    else
    {
        for(int i = indexSize - 1; i > indexSize - 1 - n; i--) // 删除后n个索引的内容
            indexes.pop_back();
        indexSize -= n;
        return true;
    }
}

// 删除最后一个索引
int NormalIndex::dropIndex()
{
    if(indexSize > 0)
    {
        int t = indexes.back();
        indexes.pop_back();
        indexSize--;
        return t;
    }
    return -1;
}

// 从索引块中获取第n个下一级索引表
NormalIndex& NormalIndex::getNxtIndex(int n)
{
    return indexTables[n];
}

// 添加一个下一级索引表
bool NormalIndex::addNxtIndex()
{
    if(indexSize >= indexCapacity)
        return false;
    else {
        auto *t = new NormalIndex(sign - 1);
        indexTables.push_back(*t);
        indexSize++;
        return true;
    }
}

// 删除后n个下一级索引表
bool NormalIndex::dropNxtIndex(int n)
{
    if(n > indexSize || n <= 0) // n超出范围
        return false;
    else
    {
        for(int i = indexSize - 1; i > indexSize - 1 - n; i--) // 删除后n个索引的内容
            indexTables.pop_back();
        indexSize -= n;
        return true;
    }
}

// 删除最后一个下一级索引表
bool NormalIndex::dropNxtIndex()
{
    if(indexSize > 0)
    {
        indexTables.pop_back();
        indexSize--;
        return true;
    }
    return false;
}

// 查询sign值
int NormalIndex::check() const
{
    return sign;
}

// 显示辅助信息
void NormalIndex::show()
{
    cout << "\n";
    for(int i = 0; i < indexSize; i++)
        cout << indexes[i] << " ";
    cout << "\n";
}
void NormalIndex::clear()
{
    indexTables.clear();
    indexes.clear();
    indexSize = 0;
}
vector<int> NormalIndex::getIndexes()
{
    return indexes;
}

// 构造函数
MixIndex::MixIndex(): oneIndirectIndex(0), twoIndirectIndex(1), threeIndirectIndex(2){}
//复制构造函数
MixIndex::MixIndex(MixIndex& ano)
        :indexSize(ano.indexSize)
        , indexes(ano.indexes)
        , oneIndirectIndex(ano.oneIndirectIndex)
        , twoIndirectIndex(ano.twoIndirectIndex)
        , threeIndirectIndex(ano.threeIndirectIndex)
{}
// 重载等号运算符
MixIndex& MixIndex::operator=(const MixIndex& ano)
{
    indexSize = ano.indexSize;
    indexes = ano.indexes;
    oneIndirectIndex = ano.oneIndirectIndex;
    twoIndirectIndex = ano.twoIndirectIndex;
    threeIndirectIndex = ano.threeIndirectIndex;
    return *this;
}
// 添加一个索引
bool MixIndex::addIndex(int id)
{
    if(indexSize >= indexCapacity || id < 0) // 索引已经满了或者id超出范围
        return false;
    else
    {
        if(indexSize < BASIC_IDX) // 直接索引
            return addDirectIndex(id);
        else if(indexSize >= BASIC_IDX && indexSize < BASIC_IDX + IDXT_SIZE) // 一次间接索引
            return addOneIndirectIndex(id);
        else if(indexSize >= BASIC_IDX + IDXT_SIZE && indexSize < BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE) // 两次间接索引
            return addTwoIndirectIndex(id);
        else if(indexSize >= BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE && indexSize < MIX_IDXT_SIZE) // 三次简介索引
            return addThreeIndirectIndex(id);
        else
            return false;
    }
}

// 添加直接索引
bool MixIndex::addDirectIndex(int id)
{
    indexes.push_back(id);
    indexSize++;
    return true;
}

// 添加一次间接索引
bool MixIndex::addOneIndirectIndex(int id)
{
    oneIndirectIndex.addIndex(id);
    indexSize++;
    return true;
}

// 添加二次间接索引
bool MixIndex::addTwoIndirectIndex(int id)
{
    // 计算超出前两部分索引的数量
    int n = indexSize - BASIC_IDX - IDXT_SIZE;
    if(n % IDXT_SIZE == 0) //正好填满前一个二次间址索引块
        twoIndirectIndex.addNxtIndex(); // 新建一个索引块

    //获得当前未满的二次间址索引块
    NormalIndex &t = twoIndirectIndex.getNxtIndex(n / IDXT_SIZE);
    t.addIndex(id);
    indexSize++;
    return true; // 添加索引
}

// 添加三次间接索引
bool MixIndex::addThreeIndirectIndex(int id)
{
    // 计算超出前三部分索引的数量
    int n = indexSize - BASIC_IDX - IDXT_SIZE - ONE_IDXT_SIZE;
    if(n % ONE_IDXT_SIZE == 0) // 正好填满前一个二次间址索引块
        threeIndirectIndex.addNxtIndex(); // 新建一个二次间址索引块

    // 获得当前未满的二次间址索引块
    NormalIndex &t = threeIndirectIndex.getNxtIndex(n / ONE_IDXT_SIZE);
    if(n % IDXT_SIZE == 0) // 正好填满一个三次间址索引块
        t.addNxtIndex(); // 新建一个三次间址索引块

    // 获得当前未满的三次间址索引块
    NormalIndex &nxt = t.getNxtIndex((n % ONE_IDXT_SIZE) / IDXT_SIZE);
    nxt.addIndex(id);
    indexSize++;
    return true; // 添加索引
}

// 返回索引数量
int MixIndex::size() const
{
    return indexSize;
}
// 获取所有索引编号
vector<int> MixIndex::getIndexes()
{
    vector<int> t;
    // 获取直接索引编号
    for(int i = 0; i < min(indexSize, BASIC_IDX); i++)
        t.push_back(indexes[i]);
    // 获取一次间接索引编号
    if(indexSize >= BASIC_IDX)
    {
        vector<int> t1 = oneIndirectIndex.getIndexes();
        for(int i = 0; i < t1.size(); i++)
            t.push_back(t1[i]);
    }
    // 获取两次间接索引编号
    if(indexSize >= BASIC_IDX + IDXT_SIZE)
    {
        int n = twoIndirectIndex.size();
        for(int i = 0; i < n; i++)
        {
            vector<int> t1 = twoIndirectIndex.getNxtIndex(i).getIndexes();
            for(int j = 0; j < t1.size(); j++)
                t.push_back(t1[j]);
        }

    }
    // 获取三次间接索引编号
    if(indexSize >= BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE)
    {
        int n = threeIndirectIndex.size();
        for(int i = 0; i < n; i++)
        {
            int m = threeIndirectIndex.getNxtIndex(i).size();
            for(int j = 0; j < m; j++)
            {
                vector<int> t1 = threeIndirectIndex.getNxtIndex(i).getNxtIndex(j).getIndexes();
                for(int k = 0; k < t1.size(); k++)
                    t.push_back(t1[k]);
            }

        }
    }
    return t;
}

// 展示信息
void MixIndex::show()
{
    cout << "——————————————————————————————\n";
    cout << "直接索引区：\n";
    for(int i = 0; i < min(indexSize, BASIC_IDX); i++)
        cout << indexes[i] << " ";
    cout << "\n";
    cout << "一次间接索引区：";
    if(indexSize >= BASIC_IDX)
    {
        oneIndirectIndex.show();
    }
    cout << "二次间接索引区：";
    if(indexSize >= BASIC_IDX + IDXT_SIZE)
    {
        int n = twoIndirectIndex.size();
        for(int i = 0; i < n; i++)
            twoIndirectIndex.getNxtIndex(i).show();
    }
    cout << "三次间接索引区：";
    if(indexSize >= BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE)
    {
        int n = threeIndirectIndex.size();
        for(int i = 0; i < n; i++)
        {
            int m = threeIndirectIndex.getNxtIndex(i).size();
            for(int j = 0; j < m; j++)
                threeIndirectIndex.getNxtIndex(i).getNxtIndex(j).show();
        }
    }
    cout << "\n——————————————————————————————\n";
}

// 删除一个索引
int MixIndex::dropIndex()
{
    if(indexSize <= 0)
        return -1;
    else if(indexSize <= BASIC_IDX) // 直接索引
        return dropDirectIndex();
    else if(indexSize <= BASIC_IDX + IDXT_SIZE) // 一次间接索引
        return dropOneInDirectIndex();
    else if(indexSize <= BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE) // 二次间接索引
        return dropTwoInDirectIndex();
    else
        return dropThreeInDirectIndex(); // 三次间接索引
}

// 删除直接索引
int MixIndex::dropDirectIndex()
{
    int t = indexes.back();
    indexes.pop_back();
    indexSize--;
    return t;
}

// 删除一次间接索引
int MixIndex::dropOneInDirectIndex()
{
    int t = oneIndirectIndex.dropIndex();
    indexSize--;
    return t;
}

// 删除二次间接索引
int MixIndex::dropTwoInDirectIndex()
{
    // 计算超出前两部分的索引数量
    int n = indexSize - BASIC_IDX - IDXT_SIZE - 1; //这里的减一非常重要！因为size是从1开始的，下标则是从0开始，导致除法时跳到下一个块
    // 找到对应的二次间址索引块
    int t = twoIndirectIndex.getNxtIndex(n / IDXT_SIZE).dropIndex();
    indexSize--;
    // 此处n的含义应是数量而不是下标，因此要把减一补偿回来
    if((n + 1) % IDXT_SIZE == 1) // 如果删除的正好是二次间址索引块的最后一个索引
        twoIndirectIndex.dropNxtIndex(); // 把索引块也删除
    return t;
}

// 删除三次间接索引
int MixIndex::dropThreeInDirectIndex()
{
    // 计算超出前三部分的索引数量
    int n = indexSize - BASIC_IDX - IDXT_SIZE - ONE_IDXT_SIZE - 1;// 同上，一定要减一
    // 找到对应的三次间址索引块
    int t = threeIndirectIndex.getNxtIndex(n / ONE_IDXT_SIZE).getNxtIndex((n % ONE_IDXT_SIZE) / IDXT_SIZE).dropIndex();
    indexSize--;
    // 此处n的含义应是数量而不是下标，因此要把减一补偿回来
    if(((n + 1) % ONE_IDXT_SIZE) % IDXT_SIZE == 1) // 如果删除的正好是三次间址索引块的最后一个索引
        threeIndirectIndex.getNxtIndex(n / ONE_IDXT_SIZE).dropNxtIndex(); // 把这个三次间址索引块也删除
    // 此处n的含义应是数量而不是下标，因此要把减一补偿回来
    if((n + 1) % ONE_IDXT_SIZE == 1) // 如果删除的正好是二次间址索引块的最后一个索引
        threeIndirectIndex.dropNxtIndex(); // 把这个二次间址索引块也删除
    return t;
}

//清空
void MixIndex::clear()
{
    indexes.clear();
    oneIndirectIndex.clear();
    twoIndirectIndex.clear();
    threeIndirectIndex.clear();
    indexSize = 0;
}