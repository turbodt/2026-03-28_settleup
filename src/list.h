#ifndef MATRIX_CSR_LIST_H
#define MATRIX_CSR_LIST_H


#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "./error.h"


#define SPM_LIST_HEADER(SPM_SCALAR_T, TYPE_SUFFIX, FUNC_SUFFIX) \
\
typedef struct { \
    SPM_SCALAR_T *items; \
    size_t count; \
    size_t capacity; \
} SpmList##TYPE_SUFFIX; \
\
\
\
SpmErr spm_list_##FUNC_SUFFIX##_init(SpmList##TYPE_SUFFIX *l, size_t initial_capacity); \
SpmErr spm_list_##FUNC_SUFFIX##_cpy(SpmList##TYPE_SUFFIX *dst, SpmList##TYPE_SUFFIX const *src); \
void spm_list_##FUNC_SUFFIX##_clear(SpmList##TYPE_SUFFIX *l); \
\
size_t spm_list_##FUNC_SUFFIX##_get_count(SpmList##TYPE_SUFFIX const *l); \
SPM_SCALAR_T const * spm_list_##FUNC_SUFFIX##_atc(SpmList##TYPE_SUFFIX const *l, size_t index); \
SPM_SCALAR_T * spm_list_##FUNC_SUFFIX##_at(SpmList##TYPE_SUFFIX *l, size_t index); \
SPM_SCALAR_T spm_list_##FUNC_SUFFIX##_get(SpmList##TYPE_SUFFIX const *l, size_t index); \
void spm_list_##FUNC_SUFFIX##_set(SpmList##TYPE_SUFFIX *l, size_t index, SPM_SCALAR_T); \
SpmErr spm_list_##FUNC_SUFFIX##_insert_at(SpmList##TYPE_SUFFIX *l, size_t index, size_t insert_count); \
SpmErr spm_list_##FUNC_SUFFIX##_remove_at(SpmList##TYPE_SUFFIX *l, size_t index, size_t delete_count); \
SpmErr spm_list_##FUNC_SUFFIX##_append(SpmList##TYPE_SUFFIX *l, size_t insert_count); \



#define SPM_LIST__MIN_CAPACITY 8
#define SPM_LIST_IMPLEMENTATION(SPM_SCALAR_T, TYPE_SUFFIX, FUNC_SUFFIX) \
\
\
 \
 \
static SpmErr spm_list_##FUNC_SUFFIX##_ensure_capacity(SpmList##TYPE_SUFFIX *, size_t capacity); \
 \
 \
SpmErr spm_list_##FUNC_SUFFIX##_init(SpmList##TYPE_SUFFIX *list, size_t initial_capacity) { \
    list->count = 0; \
    list->capacity = initial_capacity; \
    list->items = NULL; \
 \
    if (!initial_capacity) { \
        return SPM_ERR__OK; \
    } \
 \
    list->items = malloc(sizeof(SPM_SCALAR_T) * list->capacity); \
 \
    if (!list->items) { \
        list->capacity = 0; \
        return SPM_ERR__ALLOC; \
    } \
 \
    return SPM_ERR__OK; \
}; \
 \
 \
SpmErr spm_list_##FUNC_SUFFIX##_cpy(SpmList##TYPE_SUFFIX *dst, SpmList##TYPE_SUFFIX const *src) { \
 \
    if (!src->items || !src->capacity) { \
        spm_list_##FUNC_SUFFIX##_clear(dst); \
        return SPM_ERR__OK; \
    } \
 \
    void *new_items; \
    if (dst->items) { \
        new_items = realloc(dst->items, sizeof(SPM_SCALAR_T)*src->capacity); \
    } else { \
        new_items = malloc(sizeof(SPM_SCALAR_T)*src->capacity); \
    } \
 \
    if (!new_items) { \
        return SPM_ERR__ALLOC; \
    } \
    dst->items = new_items; \
    dst->count = src->count; \
    dst->capacity = src->capacity; \
 \
    memcpy(dst->items, src->items, sizeof(SPM_SCALAR_T)*src->capacity); \
 \
    return SPM_ERR__OK; \
}; \
 \
 \
void spm_list_##FUNC_SUFFIX##_clear(SpmList##TYPE_SUFFIX *list) { \
    if (list->items) { \
        free(list->items); \
        list->items = NULL; \
    } \
    list->capacity = 0; \
    list->count = 0; \
}; \
 \
 \
inline size_t spm_list_##FUNC_SUFFIX##_get_count(SpmList##TYPE_SUFFIX const *list) { \
    return list->count; \
}; \
 \
 \
inline SPM_SCALAR_T * spm_list_##FUNC_SUFFIX##_at(SpmList##TYPE_SUFFIX *list, size_t index) { \
    return list->items + index; \
}; \
 \
 \
inline SPM_SCALAR_T const * spm_list_##FUNC_SUFFIX##_atc(SpmList##TYPE_SUFFIX const *list, size_t index) { \
    return spm_list_##FUNC_SUFFIX##_at((SpmList##TYPE_SUFFIX *)(list), index); \
}; \
 \
 \
inline SPM_SCALAR_T spm_list_##FUNC_SUFFIX##_get(SpmList##TYPE_SUFFIX const *list, size_t index) { \
    return list->items[index]; \
}; \
 \
 \
inline void spm_list_##FUNC_SUFFIX##_set(SpmList##TYPE_SUFFIX *list, size_t index, SPM_SCALAR_T value) { \
    memcpy(list->items + index, &value, sizeof(SPM_SCALAR_T)); \
}; \
 \
 \
SpmErr spm_list_##FUNC_SUFFIX##_insert_at(SpmList##TYPE_SUFFIX *list, size_t index, size_t insert_count) { \
    SpmErr err; \
 \
    if (index > spm_list_##FUNC_SUFFIX##_get_count(list)) { \
        return SPM_ERR__OUT_INDEX; \
    } \
 \
    err = spm_list_##FUNC_SUFFIX##_ensure_capacity(list, spm_list_##FUNC_SUFFIX##_get_count(list) + insert_count); \
 \
    if (err != SPM_ERR__OK) { \
        return err; \
    } \
 \
    for (size_t i = list->count; i > index; i--) { \
        memcpy( \
            list->items + (i + insert_count -1), \
            list->items + (i - 1), \
            sizeof(SPM_SCALAR_T) \
        ); \
    } \
    list->count += insert_count; \
 \
    return SPM_ERR__OK; \
}; \
 \
 \
SpmErr spm_list_##FUNC_SUFFIX##_remove_at(SpmList##TYPE_SUFFIX *list, size_t index, size_t delete_count) { \
    SpmErr err; \
 \
    if (delete_count + index > spm_list_##FUNC_SUFFIX##_get_count(list)) { \
        return SPM_ERR__OUT_INDEX; \
    } \
 \
    if (delete_count == 0) { \
        return SPM_ERR__OK; \
    } \
 \
    for (size_t i = index + delete_count; i < list->count; i++) { \
        memcpy( \
            list->items + (i - delete_count), \
            list->items + i, \
            sizeof(SPM_SCALAR_T) \
        ); \
    } \
 \
    spm_list_##FUNC_SUFFIX##_ensure_capacity(list, list->count - delete_count); \
    list->count -= delete_count; \
 \
    return SPM_ERR__OK; \
}; \
 \
 \
inline SpmErr spm_list_##FUNC_SUFFIX##_append(SpmList##TYPE_SUFFIX *list, size_t insert_count) { \
    return spm_list_##FUNC_SUFFIX##_insert_at(list, list->count, insert_count); \
}; \
 \
 \
SpmErr spm_list_##FUNC_SUFFIX##_ensure_capacity(SpmList##TYPE_SUFFIX *list, size_t capacity) { \
    size_t const curr_capacity = list->capacity; \
 \
    if (capacity < SPM_LIST__MIN_CAPACITY) { \
        capacity = SPM_LIST__MIN_CAPACITY; \
    } \
 \
    size_t new_capacity = curr_capacity; \
    while (capacity > new_capacity) { \
        new_capacity *= 2; \
    } \
    while (2*capacity < new_capacity) { \
        new_capacity /= 2; \
    } \
 \
    if (new_capacity == curr_capacity) { \
        return SPM_ERR__OK; \
    } \
 \
    void *new_items; \
    if (!list->items) { \
        new_items = malloc(new_capacity*sizeof(SPM_SCALAR_T)); \
    } else { \
        new_items = realloc(list->items, new_capacity*sizeof(SPM_SCALAR_T)); \
    } \
 \
    if (!new_items) { \
        return SPM_ERR__ALLOC; \
    } \
 \
    list->items = new_items; \
    list->capacity = new_capacity; \
    return SPM_ERR__OK; \
}; \



#endif
