//
// Created by 19150 on 2022/5/30.
//

#include <bits/stdc++.h>
#include "GlobalVariable.h"

#ifndef OS_BETA_INDEX_H
#define OS_BETA_INDEX_H

using namespace std;

class NormalIndex{
private:
    const int indexCapacity = INDEXTABLESIZE;    //(64)/4
    int sign;
    int indexSize = 0;
    vector<int> indexes;
    vector<NormalIndex> indexTables;

public:
    NormalIndex(int sign = 0);

    NormalIndex(const NormalIndex&);

    NormalIndex& operator=(const NormalIndex&);
    int getIndex(int n);                        //get the n of index
    bool addIndex(int id);                      //add new index
    int size() const;                           //get the number of index
    bool dropIndex(int n);
    int dropIndex();                            //Delete the last index
    NormalIndex& getNxtIndex(int n);
    bool addNxtIndex();
    bool dropNxtIndex(int n);
    bool dropNxtIndex();
    int check() const;                          //find the sign
    void show();
    void clear();
    vector<int> getIndexes();                   //get all the indexes
};


// 混合索引表
class MixIndex{
private:
    const int indexCapacity = MIXINDEXSIZE;
    int indexSize = 0;
    vector<int> indexes;
    NormalIndex oneIndirectIndex;
    NormalIndex twoIndirectIndex;
    NormalIndex threeIndirectIndex;

    bool addDirectIndex(int id);                 //add the DirectIndex
    bool addOneIndirectIndex(int id);            //Add first indirect index
    bool addTwoIndirectIndex(int id);            //Add secondary indirect index
    bool addThreeIndirectIndex(int id);          //Add third indirect index
    int dropDirectIndex();                       //delete the direct index
    int dropOneInDirectIndex();                  //delete the first indirect index
    int dropTwoInDirectIndex();                  //delete the second indirect index
    int dropThreeInDirectIndex();                //delete the third indirect index

public:
    MixIndex();
    MixIndex(MixIndex&);
    MixIndex& operator=(const MixIndex&);
    bool addIndex(int id);
    int size() const;                             //get the size of index
    void show();
    int dropIndex();
    void clear();
    vector<int> getIndexes();                     //get all the index
};

#endif //OS_BETA_INDEX_H
