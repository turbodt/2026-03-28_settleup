#ifndef MATRIX_CSR_LIST_H
#define MATRIX_CSR_LIST_H


#include <stddef.h>
#include "./error.h"

typedef struct List List;
struct List {
    void *items;
    size_t count;
    size_t capacity;
    size_t item_size;
};



SpmErr list_init(List *l, size_t item_size, size_t initial_capacity);
SpmErr list_cpy(List *dst, List const *src);
void list_clear(List *l);

size_t list_get_count(List const *l);
void const * list_atc(List const *l, size_t index);
void * list_at(List *l, size_t index);
SpmErr list_insert_at(List *l, size_t index, size_t insert_count);
SpmErr list_remove_at(List *l, size_t index, size_t delete_count);
SpmErr list_append(List *l, size_t insert_count);



#endif
