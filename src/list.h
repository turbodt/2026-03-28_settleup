#ifndef MATRIX_CSR_LIST_H
#define MATRIX_CSR_LIST_H


#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "./error.h"


#ifndef SPM_LIST__MIN_CAPACITY
#define SPM_LIST__MIN_CAPACITY 8
#endif


#define SPM_LIST_HEADER(S_T, T_SFX, F_SFX) \
\
typedef struct { \
    S_T *items; \
    size_t count; \
    size_t capacity; \
} SpmList##T_SFX; \
\
\
\
SpmErr spm_list_##F_SFX##_init(SpmList##T_SFX *l, size_t initial_capacity); \
SpmErr spm_list_##F_SFX##_cpy(SpmList##T_SFX *dst, SpmList##T_SFX const *src); \
void spm_list_##F_SFX##_clear(SpmList##T_SFX *l); \
\
size_t spm_list_##F_SFX##_get_count(SpmList##T_SFX const *l); \
S_T const * spm_list_##F_SFX##_atc(SpmList##T_SFX const *l, size_t index); \
S_T * spm_list_##F_SFX##_at(SpmList##T_SFX *l, size_t index); \
S_T spm_list_##F_SFX##_get(SpmList##T_SFX const *l, size_t index); \
void spm_list_##F_SFX##_set(SpmList##T_SFX *l, size_t index, S_T); \
SpmErr spm_list_##F_SFX##_insert_at(SpmList##T_SFX *l, size_t index, size_t insert_count); \
SpmErr spm_list_##F_SFX##_remove_at(SpmList##T_SFX *l, size_t index, size_t delete_count); \
SpmErr spm_list_##F_SFX##_append(SpmList##T_SFX *l, size_t insert_count); \



#define SPM_LIST_IMPLEMENTATION(S_T, T_SFX, F_SFX) \
\
\
 \
 \
static SpmErr spm_list_##F_SFX##_ensure_capacity(SpmList##T_SFX *, size_t capacity); \
 \
 \
SpmErr spm_list_##F_SFX##_init(SpmList##T_SFX *list, size_t initial_capacity) { \
    list->count = 0; \
    list->capacity = initial_capacity; \
    list->items = NULL; \
 \
    if (!initial_capacity) { \
        return SPM_ERR__OK; \
    } \
 \
    list->items = malloc(sizeof(S_T) * list->capacity); \
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
SpmErr spm_list_##F_SFX##_cpy(SpmList##T_SFX *dst, SpmList##T_SFX const *src) { \
 \
    if (!src->items || !src->capacity) { \
        spm_list_##F_SFX##_clear(dst); \
        return SPM_ERR__OK; \
    } \
 \
    void *new_items; \
    if (dst->items) { \
        new_items = realloc(dst->items, sizeof(S_T)*src->capacity); \
    } else { \
        new_items = malloc(sizeof(S_T)*src->capacity); \
    } \
 \
    if (!new_items) { \
        return SPM_ERR__ALLOC; \
    } \
    dst->items = new_items; \
    dst->count = src->count; \
    dst->capacity = src->capacity; \
 \
    memcpy(dst->items, src->items, sizeof(S_T)*src->capacity); \
 \
    return SPM_ERR__OK; \
}; \
 \
 \
void spm_list_##F_SFX##_clear(SpmList##T_SFX *list) { \
    if (list->items) { \
        free(list->items); \
        list->items = NULL; \
    } \
    list->capacity = 0; \
    list->count = 0; \
}; \
 \
 \
inline size_t spm_list_##F_SFX##_get_count(SpmList##T_SFX const *list) { \
    return list->count; \
}; \
 \
 \
inline S_T * spm_list_##F_SFX##_at(SpmList##T_SFX *list, size_t index) { \
    return list->items + index; \
}; \
 \
 \
inline S_T const * spm_list_##F_SFX##_atc(SpmList##T_SFX const *list, size_t index) { \
    return spm_list_##F_SFX##_at((SpmList##T_SFX *)(list), index); \
}; \
 \
 \
inline S_T spm_list_##F_SFX##_get(SpmList##T_SFX const *list, size_t index) { \
    return list->items[index]; \
}; \
 \
 \
inline void spm_list_##F_SFX##_set(SpmList##T_SFX *list, size_t index, S_T value) { \
    memcpy(list->items + index, &value, sizeof(S_T)); \
}; \
 \
 \
SpmErr spm_list_##F_SFX##_insert_at(SpmList##T_SFX *list, size_t index, size_t insert_count) { \
    SpmErr err; \
 \
    if (index > spm_list_##F_SFX##_get_count(list)) { \
        return SPM_ERR__OUT_INDEX; \
    } \
 \
    err = spm_list_##F_SFX##_ensure_capacity(list, spm_list_##F_SFX##_get_count(list) + insert_count); \
 \
    if (err != SPM_ERR__OK) { \
        return err; \
    } \
 \
    for (size_t i = list->count; i > index; i--) { \
        memcpy( \
            list->items + (i + insert_count -1), \
            list->items + (i - 1), \
            sizeof(S_T) \
        ); \
    } \
    list->count += insert_count; \
 \
    return SPM_ERR__OK; \
}; \
 \
 \
SpmErr spm_list_##F_SFX##_remove_at(SpmList##T_SFX *list, size_t index, size_t delete_count) { \
    SpmErr err; \
 \
    if (delete_count + index > spm_list_##F_SFX##_get_count(list)) { \
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
            sizeof(S_T) \
        ); \
    } \
 \
    spm_list_##F_SFX##_ensure_capacity(list, list->count - delete_count); \
    list->count -= delete_count; \
 \
    return SPM_ERR__OK; \
}; \
 \
 \
inline SpmErr spm_list_##F_SFX##_append(SpmList##T_SFX *list, size_t insert_count) { \
    return spm_list_##F_SFX##_insert_at(list, list->count, insert_count); \
}; \
 \
 \
SpmErr spm_list_##F_SFX##_ensure_capacity(SpmList##T_SFX *list, size_t capacity) { \
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
        new_items = malloc(new_capacity*sizeof(S_T)); \
    } else { \
        new_items = realloc(list->items, new_capacity*sizeof(S_T)); \
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
