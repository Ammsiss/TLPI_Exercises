/*
 * list_pop()
 * list_clear()
 * list_foreach() macro
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "stdlib.h"

typedef struct {
    void *data;
    size_t size;
    size_t capacity;
    size_t dataSize;
} DynArray;

int da_max(DynArray *arr);
void* da_back(DynArray *arr);
int da_init(DynArray *arr, size_t size, size_t dataSize);
int da_reserve(DynArray *arr, size_t capacity);
void da_free(DynArray *arr);
void* da_detach(DynArray *arr);
void* da_get(const DynArray *arr, size_t index);
void* da_push(DynArray *arr);
void* da_pop(DynArray *arr);

int da_reverse(DynArray *arr);

// Sorting
int int_asc(const void *elem0, const void *elem1);
void da_sort(DynArray *arr, int (* pred)(const void *, const void *));

#endif
