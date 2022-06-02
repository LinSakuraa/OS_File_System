//
// Created by 22325 on 2022/5/30.
//

#ifndef OS_BETA_GLOBALVARIABLE_H
#define OS_BETA_GLOBALVARIABLE_H

#define BLOCKSIZE 64 // 磁盘块大小
#define DIRECTINDEXNUM 10 // 混合索引表中直接索引的数量
#define INDEXTABLESIZE (BLOCKSIZE/4) // 普通索引表大小
#define SECONDINDEXSIZE (INDEXTABLESIZE * INDEXTABLESIZE) // 二次间接索引表大小
#define THIRDINDEXSIZE (SECONDINDEXSIZE * INDEXTABLESIZE) // 三次间接索引表大小
#define MIXINDEXSIZE (DIRECTINDEXNUM + INDEXTABLESIZE + SECONDINDEXSIZE + THIRDINDEXSIZE) // 混合索引表大小

#define GROUPSIZE 50 // 成组链接法每一组的大小
#define TOTALGROUPSIZE 500 // 成组链接法初始化时的总大小

#define INODENUM 128    // i结点个数
#define MAXINODETNUM 64 // 内存中i结点表可容纳i结点的最大数量
#define SYSTEMFILEMAXNUM 64  // 系统文件打开表最大数量
#define USERFILEMAXNUM 5 // 用户文件打开表最大数量


#endif //OS_BETA_GLOBALVARIABLE_H
