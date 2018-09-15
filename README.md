# DongmenDB
DongmenDB is a didactical relational database system.


## 常见错误
googletest报错:`OpenThread is not declared `
[参考链接](https://github.com/google/googletest/issues/893#issuecomment-252029768_)
修改`MinGW/include/windef.h`中的   
```
#ifndef WINVER
//#define WINVER 0x0400 #注释这一行
#define WINVER 0x0500  #替换成这一行
/*
 * If you need Win32 API features newer the Win95 and WinNT then you must
 * define WINVER before including windows.h or any other method of including
 * the windef.h header.
 */
#endif
```