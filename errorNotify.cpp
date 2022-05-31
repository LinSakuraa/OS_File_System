//
// Created by LinSakura on 2022/5/31.
//
#include "errorNotify.h"
#include "userControl.h"
using namespace std;
extern void errorNotifyInit(int optimizer)
{
    switch (optimizer)
    {
        case 0:
        {
            authenticateError(0);
            nodeError(0);
            blockError(0);
            directoryError(0);
            cdError(0);
            clipboardError(0);
            pointerError(0);
            fileError(0);
        }break;
        case 1: cout<<"ENS Ready!"<<endl;
            break;
    }
}
extern void authenticateError(int optimizer)
{
    switch (optimizer)
    {
        case 0: cout<<"AEN Ready!"<<endl;
            break;
        case 1: cout<<"you are not authenticated!"<<endl;
            break;
        case 2: cout<<"error CODE: "<<optimizer<<endl;
            break;
    }
}
extern void nodeError(int optimizer)
{
    switch (optimizer)
    {
        case 0: cout<<"NEN Ready!"<<endl;
            break;
        case 1: cout<<"I-nodes have been run out"<<endl;
            break;
        case 2: cout<<""<<endl;
            break;
    }
}
extern void blockError(int optimizer)
{
    switch (optimizer)
    {
        case 0: cout<<"BEN Ready!"<<endl;
            break;
        case 1: cout<<"block has been run out"<<endl;
            break;
        case 2: cout<<""<<endl;
            break;
    }
}
extern void directoryError(int optimizer)
{
    switch (optimizer)
    {
        case 0: cout<<"DEN Ready!"<<endl;
            break;
        case 1: cout<<"the directory does not exist!"<<endl;
            break;
        case 2: cout<<"no such directory"<<endl;
            break;
    }
}
extern void cdError(int optimizer)
{
    switch (optimizer)
    {
        case 0: cout<<"CEN Ready!"<<endl;
            break;
        case 1: cout<<"you can not cd a file"<<endl;
            break;
        case 2: cout<<""<<endl;
            break;
    }
}
extern void clipboardError(int optimizer)
{
    switch (optimizer)
    {
        case 0: cout<<"CBEN Ready!"<<endl;
            break;
        case 1: cout<<"a file still remains in the clipboard!"<<endl;
            break;
        case 2: cout<<"you can not copy a directory!"<<endl;
            break;
        case 3: cout<<"you can not cut a directory!"<<endl;
            break;
        case 4: cout<<"no file in the clipboard"<<endl;
            break;
    }
}
extern void pointerError(int optimizer)
{
    switch (optimizer)
    {
        case 0: cout<<"PEN Ready!"<<endl;
            break;
        case 1: cout<<"you can not change the pointer of a directory"<<endl;
            break;
        case 2: cout<<""<<endl;
            break;
    }
}
extern void fileError(int optimizer)
{
    switch (optimizer)
    {
        case 0: cout<<"FEN Ready!"<<endl;
            break;
        case 1: cout<<"the file does not exist!"<<endl;
            break;
        case 2: cout<<"the file have not been opened!"<<endl;
            break;
        case 3: cout<<"the file is read only!"<<endl;
            break;
        case 4: cout<<"the file has not been opened!"<<endl;
            break;
    }
}