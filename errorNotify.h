//
// Created by LinSakura on 2022/5/31.
//
#include <iostream>
#ifndef OS_BETA_ERRORNOTIFY_H
#define OS_BETA_ERRORNOTIFY_H
using namespace std;
extern void authenticateError(int optimizer);
extern void nodeError(int optimizer);
extern void blockError(int optimizer);
//void fileExistError();
extern void directoryError(int optimizer);
extern void cdError(int optimizer);
extern void clipboardError(int optimizer);
//void clipboardError2();
//void copyError();
//void cutError();
extern void pointerError(int optimizer);
extern void fileError(int optimizer);

#endif //OS_BETA_ERRORNOTIFY_H
