//
// Created by Sam on 2018/1/25.
//

#ifndef DONGMENDB_UTILS_H
#define DONGMENDB_UTILS_H

#include "arraylist.h"
#ifdef __cplusplus
extern "C" {
#endif

int dongmendb_tokenize(char *str, char ***tokens);

int bytes2int(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4);

/**
 * 产生一个长度为MAX_ID_NAME_LENGTH的字符串，作为id的名称
 * @return 字符串
 */
char *new_id_name();

/* 删除指定文件夹以及其中文档*/
int  removeDir(const char*  dirPath);
int  removeDirW(const wchar_t*  dirPath);

#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_UTILS_H
