#include "./list.h"
#include <stdlib.h>
#include <string.h>


#define LIST_MIN_CAPACITY 8
static SpmErr ensure_capacity(SpmList *, size_t capacity);


SpmErr spm_list_init(SpmList *list, size_t item_size, size_t initial_capacity) {
    list->item_size = item_size;
    list->count = 0;
    list->capacity = initial_capacity;
    list->items = NULL;

    if (!initial_capacity) {
        return SPM_ERR__OK;
    }

    list->items = malloc(list->item_size * list->capacity);

    if (!list->items) {
        list->item_size = 0;
        list->capacity = 0;
        return SPM_ERR__ALLOC;
    }

    return SPM_ERR__OK;
};


SpmErr spm_list_cpy(SpmList *dst, SpmList const *src) {

    if (!src->items || !src->capacity) {
        spm_list_clear(dst);
        return SPM_ERR__OK;
    }

    void *new_items;
    if (dst->items) {
        new_items = realloc(dst->items, src->item_size*src->capacity);
    } else {
        new_items = malloc(src->item_size*src->capacity);
    }

    if (!new_items) {
        return SPM_ERR__ALLOC;
    }
    dst->items = new_items;
    dst->count = src->count;
    dst->capacity = src->capacity;
    dst->item_size = src->item_size;

    memcpy(dst->items, src->items, src->item_size*src->capacity);

    return SPM_ERR__OK;
};


void spm_list_clear(SpmList *list) {
    if (list->items) {
        free(list->items);
        list->items = NULL;
    }
    list->capacity = 0;
    list->count = 0;
};


inline size_t spm_list_get_count(SpmList const *list) {
    return list->count;
};


inline void * spm_list_at(SpmList *list, size_t index) {
    return list->items + index*list->item_size;
};


inline void const * spm_list_atc(SpmList const *list, size_t index) {
    return spm_list_at((SpmList *)(list), index);
};


SpmErr spm_list_insert_at(SpmList *list, size_t index, size_t insert_count) {
    SpmErr err;

    if (index > spm_list_get_count(list)) {
        return SPM_ERR__OUT_INDEX;
    }

    err = ensure_capacity(list, spm_list_get_count(list) + insert_count);

    if (err != SPM_ERR__OK) {
        return err;
    }

    for (size_t i = list->count; i > index; i--) {
        memcpy(
            list->items + (i + insert_count -1)*list->item_size,
            list->items + (i - 1)*list->item_size,
            list->item_size
        );
    }
    list->count += insert_count;

    return SPM_ERR__OK;
};


SpmErr spm_list_remove_at(SpmList *list, size_t index, size_t delete_count) {
    SpmErr err;

    if (delete_count + index > spm_list_get_count(list)) {
        return SPM_ERR__OUT_INDEX;
    }

    if (delete_count == 0) {
        return SPM_ERR__OK;
    }

    for (size_t i = index + delete_count; i < list->count; i++) {
        memcpy(
            list->items + (i - delete_count)*list->item_size,
            list->items + i*list->item_size,
            list->item_size
        );
    }

    ensure_capacity(list, list->count - delete_count);
    list->count -= delete_count;

    return SPM_ERR__OK;
};


inline SpmErr spm_list_append(SpmList *list, size_t insert_count) {
    return spm_list_insert_at(list, list->count, insert_count);
};


SpmErr ensure_capacity(SpmList *list, size_t capacity) {
    size_t const curr_capacity = list->capacity;

    if (capacity < LIST_MIN_CAPACITY) {
        capacity = LIST_MIN_CAPACITY;
    }

    size_t new_capacity = curr_capacity;
    while (capacity > new_capacity) {
        new_capacity *= 2;
    }
    while (2*capacity < new_capacity) {
        new_capacity /= 2;
    }

    if (new_capacity == curr_capacity) {
        return SPM_ERR__OK;
    }

    void *new_items;
    if (!list->items) {
        new_items = malloc(new_capacity*list->item_size);
    } else {
        new_items = realloc(list->items, new_capacity*list->item_size);
    }

    if (!new_items) {
        return SPM_ERR__ALLOC;
    }

    list->items = new_items;
    list->capacity = new_capacity;
    return SPM_ERR__OK;
};
