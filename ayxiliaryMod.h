//
// Created by 19150 on 2022/5/30.
//

#include <bits/stdc++.h>
#include "userControl.h"

#ifndef OS_FILE_SYSTEM_HELP_H
#define OS_FILE_SYSTEM_HELP_H

using namespace std;

extern int statistic(string filename);

extern string getCurrentTime();

//获取本地电脑主机名的函数，用来最后进行菜单的显示
extern string getLocalIpName();


#endif //OS_FILE_SYSTEM_HELP_H
