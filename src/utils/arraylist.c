//
// Created by Sam on 2018/1/28.
//

#include "arraylist.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

array_list array_list_create(size_t element_size) {
    array_list al;
    al.capacity = 10;
    al.size = 0;
    al.element_size = element_size;
    al.data = malloc(al.element_size * al.capacity);

    return al;
}

void array_list_enlarge(array_list *al) {
    al->capacity *= 2;
    al->data = realloc(al->data,
                       al->capacity * al->element_size);
}

void array_list_put(array_list *al, void *p) {
    if (al->size + 1 > al->capacity) {
        array_list_enlarge(al);
    }

    memcpy(al->data + al->element_size * al->size,
           p, al->element_size);

    al->size++;
}

void *array_list_get(const array_list *al, int idx) {
    return al->data + idx * al->element_size;
}


void *array_list_add_all(const array_list *al) {
    void *arr;

    arr = malloc(al->element_size * al->size);
    memcpy(arr, al->data, al->element_size * al->size);

    return arr;
}

void array_list_remove(array_list *al) {
    if (al != NULL && al->data != NULL) {
        free(al->data);
    }
}