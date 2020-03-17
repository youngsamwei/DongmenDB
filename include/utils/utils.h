//
// Created by Sam on 2018/1/25.
//

#include <dongmensql/sqlstatement.h>
#include <dongmensql/expression.h>

#ifndef DONGMENDB_UTILS_H
#define DONGMENDB_UTILS_H


int dongmendb_tokenize(char *str, char ***tokens);

unsigned int bkdr_hash(const char *str);

int bytes2int(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4);

/**
 * 产生一个长度为MAX_ID_NAME_LENGTH的字符串，作为id的名称
 * @return 字符串
 */
char *new_id_name();

/* 删除指定文件夹以及其中文档*/
int  removeDir(const char*  dirPath);

#ifdef __MINGW32__
int  removeDirW(const wchar_t*  dirPath);
#endif

/* case insensitive char strcmp */
int strcmp_ic(char const *a, char const *b);

#endif //DONGMENDB_UTILS_H
