//
// Created by 22325 on 2022/5/30.
//

#include <iostream>
#include <conio.h>
#include <bits/stdc++.h>
#include "index.h"
#include "blocksControl.h"
#include "fileDirectory.h"
#include "ayxiliaryMod.h"
#include "list.h"
#include "fileSystem.h"
#include "userControl.h"
#include "nodeList.h"

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
