#ifndef MATRIX_CSR_LIST_H
#define MATRIX_CSR_LIST_H


#include <stddef.h>


typedef struct List List;
struct List {
    void *items;
    size_t count;
    size_t capacity;
    size_t item_size;
};

enum {
    LIST_ERR__OK = 0,
    LIST_ERR__ALLOC = 1,
    LIST_ERR__OUT_INDEX = 2,
};
typedef int ListErr;

ListErr list_init(List *, size_t item_size, size_t initial_capacity);
ListErr list_cpy(List *, List const *);
void list_clear(List *);

size_t list_get_count(List const *);
void const * list_atc(List const *, size_t index);
void * list_at(List *, size_t index);
ListErr list_insert_at(List *, size_t index, size_t insert_count);
ListErr list_remove_at(List *, size_t index, size_t delete_count);
ListErr list_append(List *, size_t insert_count);



#endif
