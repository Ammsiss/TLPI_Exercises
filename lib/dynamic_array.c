/*
 * list_pop()
 * list_clear()
 * list_foreach() macro
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
    void *data;
    size_t size;
    size_t capacity;
    size_t dataSize;
} DynArray;

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
        else                     capacity = arr->capacity * 2;

        void *temp = arr->data;
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
