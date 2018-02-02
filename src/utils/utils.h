//
// Created by Sam on 2018/1/25.
//

#ifndef DONGMENDB_UTILS_H
#define DONGMENDB_UTILS_H

int dongmengdb_tokenize(char *str, char ***tokens);

int bytes2int(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4);

char *new_id_name();
#endif //DONGMENDB_UTILS_H
