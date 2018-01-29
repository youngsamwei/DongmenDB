//
// Created by Sam on 2018/1/28.
//

#ifndef DONGMENDB_ARRAYLIST_H
#define DONGMENDB_ARRAYLIST_H
#pragma once

#include <stdlib.h>

typedef struct array_list_ {
    int size;
    int capacity;
    size_t element_size;
    void *data;
}array_list;

array_list array_list_create(size_t element_size);
void array_list_add(array_list *pa, void *p);
void *array_list_get(const array_list *pa, int idx);
void array_list_remove(array_list *pa, void *p);
void *array_list_add_all(const array_list *pa);

#endif //DONGMENDB_ARRAYLIST_H
