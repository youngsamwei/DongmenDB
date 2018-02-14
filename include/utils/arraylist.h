//
// Created by Sam on 2018/1/31.
//

#ifndef DONGMENDB_ARRAYLISTB_H
#define DONGMENDB_ARRAYLISTB_H
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct arraylist {
    void **data;
    size_t capacity;
    size_t size;
} arraylist;

arraylist *arraylist_create();
void arraylist_destroy(arraylist **list);
int arraylist_add(arraylist *list, void *element);
int arraylist_add_all(arraylist *destlist, arraylist *srclist);
int arraylist_remove_by_element(arraylist *list, void *element);
int arraylist_remove(arraylist *list, size_t index);
void *arraylist_get(arraylist *list, size_t index);
int arraylist_set(arraylist *list, size_t index, void *value);
int arraylist_shrink(arraylist *list);
int arraylist_clear(arraylist *list);
#ifdef __cplusplus
}
#endif

#endif //DONGMENDB_ARRAYLISTB_H
