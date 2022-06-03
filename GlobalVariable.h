//
// Created by 22325 on 2022/5/30.
//

#ifndef OS_BETA_GLOBALVARIABLE_H
#define OS_BETA_GLOBALVARIABLE_H

#define BLOCKSIZE 128                                                                            // block size
#define DIRECTINDEXNUM 16                                                                     // direct index num of mix index table
#define INDEXTABLESIZE (BLOCKSIZE/4)                                                            // common index table size
#define SECONDINDEXSIZE (INDEXTABLESIZE * INDEXTABLESIZE)                                       // second index size
#define THIRDINDEXSIZE (SECONDINDEXSIZE * INDEXTABLESIZE)                                       // third index size
#define MIXINDEXSIZE (DIRECTINDEXNUM + INDEXTABLESIZE + SECONDINDEXSIZE + THIRDINDEXSIZE)       // mix index size

#define GROUPSIZE 50                                                                            // group size
#define TOTALGROUPSIZE 500                                                                      // total group size

#define INODENUM 128                                                                            // number of inode
#define MAXINODETNUM 64                                                                         // max inode num in ram
#define SYSTEMFILEMAXNUM 64                                                                     // max system open table num
#define USERFILEMAXNUM 5                                                                        // max user open table num


#endif //OS_BETA_GLOBALVARIABLE_H
