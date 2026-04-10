#ifndef MATRIX_CSR_LIST_H
#define MATRIX_CSR_LIST_H


#include <stddef.h>
#include "./error.h"

typedef struct SpmList SpmList;
struct SpmList {
    void *items;
    size_t count;
    size_t capacity;
    size_t item_size;
};



SpmErr spm_list_init(SpmList *l, size_t item_size, size_t initial_capacity);
SpmErr spm_list_cpy(SpmList *dst, SpmList const *src);
void spm_list_clear(SpmList *l);

size_t spm_list_get_count(SpmList const *l);
void const * spm_list_atc(SpmList const *l, size_t index);
void * spm_list_at(SpmList *l, size_t index);
SpmErr spm_list_insert_at(SpmList *l, size_t index, size_t insert_count);
SpmErr spm_list_remove_at(SpmList *l, size_t index, size_t delete_count);
SpmErr spm_list_append(SpmList *l, size_t insert_count);



#endif
