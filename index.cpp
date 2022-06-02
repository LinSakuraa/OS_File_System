//
// Created by 19150 on 2022/5/30.
//
#include "index.h"

NormalIndex::NormalIndex(int sign):sign(sign){}

NormalIndex::NormalIndex(const NormalIndex& ano)
        :sign(ano.sign)
        , indexSize(ano.indexSize)
        , indexes(ano.indexes)
        , indexTables(ano.indexTables)
{}

NormalIndex& NormalIndex::operator=(const NormalIndex& ano)
{
    sign = ano.sign;
    indexSize = ano.indexSize;
    indexes = ano.indexes;
    indexTables = ano.indexTables;
    return *this;
}

int NormalIndex::getIndex(int n)                      //Get the nth index
{
    if(n >= indexSize || n < 0)                       //out of the range
        return -1;
    else
        return indexes[n];
}

bool NormalIndex::addIndex(int id)                   //Add a new index with disk block number id
{
    if(id < 0 || indexSize >= indexCapacity)        // the id out of range or index block full
        return false;
    else
    {
        indexes.push_back(id);
        indexSize++;
        return true;
    }
}

int NormalIndex::size() const
{
    return indexSize;
}

bool NormalIndex::dropIndex(int n)
{
    if(n > indexSize || n <= 0)
        return false;
    else
    {
        for(int i = indexSize - 1; i > indexSize - 1 - n; i--)
            indexes.pop_back();
        indexSize -= n;
        return true;
    }
}

int NormalIndex::dropIndex()                    //Delete last index
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

NormalIndex& NormalIndex::getNxtIndex(int n)    //Get the nth lower level index table from the index block
{
    return indexTables[n];
}

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

bool NormalIndex::dropNxtIndex(int n)         //delete the last n indexTables
{
    if(n > indexSize || n <= 0)
        return false;
    else
    {
        for(int i = indexSize - 1; i > indexSize - 1 - n; i--)
            indexTables.pop_back();
        indexSize -= n;
        return true;
    }
}

bool NormalIndex::dropNxtIndex()             //Delete the last lower level index table
{
    if(indexSize > 0)
    {
        indexTables.pop_back();
        indexSize--;
        return true;
    }
    return false;
}

int NormalIndex::check() const             //find the value of sign
{
    return sign;
}

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

MixIndex::MixIndex(): oneIndirectIndex(0), twoIndirectIndex(1), threeIndirectIndex(2){}

MixIndex::MixIndex(MixIndex& ano)
        :indexSize(ano.indexSize)
        , indexes(ano.indexes)
        , oneIndirectIndex(ano.oneIndirectIndex)
        , twoIndirectIndex(ano.twoIndirectIndex)
        , threeIndirectIndex(ano.threeIndirectIndex)
{}

MixIndex& MixIndex::operator=(const MixIndex& ano)
{
    indexSize = ano.indexSize;
    indexes = ano.indexes;
    oneIndirectIndex = ano.oneIndirectIndex;
    twoIndirectIndex = ano.twoIndirectIndex;
    threeIndirectIndex = ano.threeIndirectIndex;
    return *this;
}

bool MixIndex::addIndex(int id)                         //add an index
{
    if(indexSize >= indexCapacity || id < 0)            // Index is full or ID is out of range
        return false;
    else
    {
        if(indexSize < DIRECTINDEXNUM)                       // Direct index
            return addDirectIndex(id);
        else if(indexSize >= DIRECTINDEXNUM && indexSize < DIRECTINDEXNUM + INDEXTABLESIZE) // Primary indirect index
            return addOneIndirectIndex(id);
        else if(indexSize >= DIRECTINDEXNUM + INDEXTABLESIZE && indexSize < DIRECTINDEXNUM + INDEXTABLESIZE + SECONDINDEXSIZE) // Two indirect indexes
            return addTwoIndirectIndex(id);
        else if(indexSize >= DIRECTINDEXNUM + INDEXTABLESIZE + SECONDINDEXSIZE && indexSize < MIXINDEXSIZE) // three indirect index
            return addThreeIndirectIndex(id);
        else
            return false;
    }
}

bool MixIndex::addDirectIndex(int id)                 //Add direct index
{
    indexes.push_back(id);
    indexSize++;
    return true;
}

bool MixIndex::addOneIndirectIndex(int id)              //Add first indirect index
{
    oneIndirectIndex.addIndex(id);
    indexSize++;
    return true;
}

bool MixIndex::addTwoIndirectIndex(int id)                     //Add secondary indirect index
{
    int n = indexSize - DIRECTINDEXNUM - INDEXTABLESIZE;         //Add direct index calculate the number of indexes that exceed the first two parts
    if(n % INDEXTABLESIZE == 0)                             //Just fill the previous secondary address index block
        twoIndirectIndex.addNxtIndex();                  // Create a new index block
    NormalIndex &t = twoIndirectIndex.getNxtIndex(n / INDEXTABLESIZE);
    t.addIndex(id);
    indexSize++;
    return true;
}

bool MixIndex::addThreeIndirectIndex(int id)            //Add third indirect index
{
    int n = indexSize - DIRECTINDEXNUM - INDEXTABLESIZE - SECONDINDEXSIZE;     //Add direct index calculate the number of indexes that exceed the first three parts
    if(n % SECONDINDEXSIZE == 0)
        threeIndirectIndex.addNxtIndex();                 // Create a new secondary inter address index block
    NormalIndex &t = threeIndirectIndex.getNxtIndex(n / SECONDINDEXSIZE);
    if(n % INDEXTABLESIZE == 0)                                // Exactly one triple address index block is filled
        t.addNxtIndex();                                  // Create a new triple address index block
    NormalIndex &nxt = t.getNxtIndex((n % SECONDINDEXSIZE) / INDEXTABLESIZE);
    nxt.addIndex(id);
    indexSize++;
    return true;
}

int MixIndex::size() const                  //return the size of index
{
    return indexSize;
}

vector<int> MixIndex::getIndexes()          // Get all the index id
{
    vector<int> t;                            //return an id vector
    for(int i = 0; i < min(indexSize, DIRECTINDEXNUM); i++)       //Get direct index id
        t.push_back(indexes[i]);
    if(indexSize >= DIRECTINDEXNUM)                     //Get once indirect index id
    {
        vector<int> t1 = oneIndirectIndex.getIndexes();
        for(int i = 0; i < t1.size(); i++)
            t.push_back(t1[i]);
    }
    if(indexSize >= DIRECTINDEXNUM + INDEXTABLESIZE)          //Get twice indirect index id
    {
        int n = twoIndirectIndex.size();
        for(int i = 0; i < n; i++)
        {
            vector<int> t1 = twoIndirectIndex.getNxtIndex(i).getIndexes();
            for(int j = 0; j < t1.size(); j++)
                t.push_back(t1[j]);
        }
    }
    if(indexSize >= DIRECTINDEXNUM + INDEXTABLESIZE + SECONDINDEXSIZE)    //Get three indirect index id
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

void MixIndex::show()                                 //show the information
{
    cout << "——————————————————————————————\n";
    cout << "the direct index area：\n";
    for(int i = 0; i < min(indexSize, DIRECTINDEXNUM); i++)
        cout << indexes[i] << " ";
    cout << "\n";
    cout << "Primary indirect index area：";
    if(indexSize >= DIRECTINDEXNUM)
    {
        oneIndirectIndex.show();
    }
    cout << "Secondary indirect index area：";
    if(indexSize >= DIRECTINDEXNUM + INDEXTABLESIZE)
    {
        int n = twoIndirectIndex.size();
        for(int i = 0; i < n; i++)
            twoIndirectIndex.getNxtIndex(i).show();
    }
    cout << "Cubic indirect index area：";
    if(indexSize >= DIRECTINDEXNUM + INDEXTABLESIZE + SECONDINDEXSIZE)
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

int MixIndex::dropIndex()                   //delete an index
{
    if(indexSize <= 0)
        return -1;
    else if(indexSize <= DIRECTINDEXNUM) // Direct index
        return dropDirectIndex();
    else if(indexSize <= DIRECTINDEXNUM + INDEXTABLESIZE) // one
        return dropOneInDirectIndex();
    else if(indexSize <= DIRECTINDEXNUM + INDEXTABLESIZE + SECONDINDEXSIZE) // two
        return dropTwoInDirectIndex();
    else
        return dropThreeInDirectIndex(); // three
}

int MixIndex::dropDirectIndex()                     //delete the direct index
{
    int t = indexes.back();
    indexes.pop_back();
    indexSize--;
    return t;
}

int MixIndex::dropOneInDirectIndex()                //delete the first indirect index
{
    int t = oneIndirectIndex.dropIndex();
    indexSize--;
    return t;
}

int MixIndex::dropTwoInDirectIndex()                //delete the second indirect index
{
    int n = indexSize - DIRECTINDEXNUM - INDEXTABLESIZE - 1; //Size starts with 1 and subscript starts with 0
    int t = twoIndirectIndex.getNxtIndex(n / INDEXTABLESIZE).dropIndex();
    indexSize--;
    if((n + 1) % INDEXTABLESIZE == 1)                // n is number,+1 Last index of the secondary inter address index block
        twoIndirectIndex.dropNxtIndex();         // delete the second index block
    return t;
}

int MixIndex::dropThreeInDirectIndex()               //delete third indirect index
{
    int n = indexSize - DIRECTINDEXNUM - INDEXTABLESIZE - SECONDINDEXSIZE - 1;     //find the third index
    int t = threeIndirectIndex.getNxtIndex(n / SECONDINDEXSIZE).getNxtIndex((n % SECONDINDEXSIZE) / INDEXTABLESIZE).dropIndex();
    indexSize--;
    if(((n + 1) % SECONDINDEXSIZE) % INDEXTABLESIZE == 1) // n is number,+1 Last index of the third inter address index block
        threeIndirectIndex.getNxtIndex(n / SECONDINDEXSIZE).dropNxtIndex(); // delete the third index block
    if((n + 1) % SECONDINDEXSIZE == 1) // n is number,+1 Last index of the secondary inter address index block
        threeIndirectIndex.dropNxtIndex(); // delete the second index block
    return t;
}

void MixIndex::clear()
{
    indexes.clear();
    oneIndirectIndex.clear();
    twoIndirectIndex.clear();
    threeIndirectIndex.clear();
    indexSize = 0;
}