//
// Created by 22325 on 2022/5/30.
//

#include "iostream"
#include "conio.h"
#include "bits/stdc++.h"
#include "memory.h"
#include "gcm.h"
#include "directory.h"
#include "help.h"
#include "ram.h"
#include "fileSys.h"
#include "user.h"
#include "inode.h"

using namespace std;

#ifndef OS_BETA_OS_H
#define OS_BETA_OS_H

class OS {
public:
    void printTitle();
    void run();
    void help();
};

extern OS os;

#endif //OS_BETA_OS_H
