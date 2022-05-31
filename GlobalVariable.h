//
// Created by 22325 on 2022/5/30.
//

#ifndef OS_BETA_GLOBALVARIABLE_H
#define OS_BETA_GLOBALVARIABLE_H

#define blockSize 64 // 磁盘块大小
#define directIndexNum 10 // 混合索引表中直接索引的数量
#define indexSize (blockSize/4) // 普通索引表大小
#define secondIndexSize (indexSize*indexSize) // 二次间接索引表大小
#define thirdIndexSize (secondIndexSize*indexSize) // 三次间接索引表大小
#define mixIndexSize (directIndexNum+indexSize+secondIndexSize+thirdIndexSize) // 混合索引表大小

#define groupSize 50 // 成组链接法每一组的大小
#define totalSize 500 // 成组链接法初始化时的总大小

#define nodeNum 128    // i结点个数
#define maxNodeNum 64 // 内存中i结点表可容纳i结点的最大数量
#define maxTableNumSystem 64  // 系统文件打开表最大数量
#define maxTableNumUser 5 // 用户文件打开表最大数量

#endif //OS_BETA_GLOBALVARIABLE_H
