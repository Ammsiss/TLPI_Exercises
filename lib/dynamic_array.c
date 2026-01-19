#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "dynamic_array.h"

int da_intMax(DynArray *arr)
{
    int max = INT_MIN;
    for (size_t i = 0; i < arr->size; ++i) {
        int *element = (int *)arr->data;
        if (element[i] > max)
            max = element[i];
    }

    return max;
}

void* da_back(DynArray *arr)
{
    return (char *)arr->data + (arr->size - 1) * arr->dataSize;
}

int da_init(DynArray *arr, size_t size, size_t dataSize)
{
    if (dataSize == 0)
        return -1;

    arr->size = size;
    arr->dataSize = dataSize;
    arr->capacity = size;
    arr->data = malloc(arr->size * arr->dataSize);

    return arr->data == NULL;
}

int da_reserve(DynArray *arr, size_t capacity)
{
    if (capacity <= arr->capacity)
        return 0;

    void *temp = arr->data;
    temp = realloc(arr->data, capacity * arr->dataSize);
    if (temp == NULL)
        return -1;

    arr->data = temp;
    arr->capacity = capacity;

    return 1;
}

void da_free(DynArray *arr)
{
    free(arr->data);
}

void* da_detach(DynArray *arr)
{
    void *data = arr->data;

    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    arr->dataSize = 0;

    return data;
}

void* da_get(const DynArray *arr, size_t index)
{
    char *out = (char *)arr->data;
    out += index * arr->dataSize;

    return out;
}

void* da_push(DynArray *arr)
{
    if (arr->size >= arr->capacity) {
        size_t capacity;

        if (arr->capacity == 0) capacity = 1;
        else                    capacity = arr->capacity * 2;

        void *temp;
        temp = realloc(arr->data, capacity * arr->dataSize);
        if (temp == NULL)
            return NULL;

        arr->capacity = capacity;
        arr->data = temp;
    }

    ++arr->size;

    void *out = da_get(arr, arr->size - 1);
    if (out == NULL)
        return NULL;

    return out;
}

int da_pop(DynArray *arr)
{
    if (arr->size < 1)
        return -1;

    --arr->size;
    return 1;
}

int da_clear(DynArray *arr)
{
    da_free(arr);

    arr->size = 0;
    arr->capacity = 0;
    arr->data = malloc(arr->size * arr->dataSize);
    if (arr->data == NULL)
        return -1;

    return 0;
}

// Algorithms

int da_reverse(DynArray *arr)
{
    if (arr->size <= 0)
        return -1;

    char *newData = malloc(arr->capacity * arr->dataSize);
    if (!newData)
        return -1;

    for (size_t i = arr->size - 1, y = 0; (int)i >= 0; --i, ++y) {
        void *element = da_get(arr, i);
        memcpy(newData + y * arr->dataSize, element, arr->dataSize);
    }

    free(arr->data);
    arr->data = newData;

    return 1;
}

int int_asc(const void *elem0, const void *elem1)
{
    const int *x = elem0;
    const int *y = elem1;

    return x - y;
}

void da_sort(DynArray *arr, int (* pred)(const void *, const void *))
{
    qsort(arr->data, arr->size, arr->dataSize, pred);
}
