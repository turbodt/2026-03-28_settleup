#include "./list.h"
#include <stdlib.h>
#include <string.h>


#define LIST_MIN_CAPACITY 8
static ListErr ensure_capacity(List *, size_t capacity);


ListErr list_init(List *list, size_t item_size, size_t initial_capacity) {
    list->item_size = item_size;
    list->count = 0;
    list->capacity = initial_capacity;
    list->items = NULL;

    if (!initial_capacity) {
        return LIST_ERR__OK;
    }

    list->items = malloc(list->item_size * list->capacity);

    if (!list->items) {
        list->item_size = 0;
        list->capacity = 0;
        return LIST_ERR__ALLOC;
    }

    return LIST_ERR__OK;
};


ListErr list_cpy(List *dst, List const *src) {

    if (!src->items || !src->capacity) {
        list_clear(dst);
        return LIST_ERR__OK;
    }

    void *new_items;
    if (dst->items) {
        new_items = realloc(dst->items, src->item_size*src->capacity);
    } else {
        new_items = malloc(src->item_size*src->capacity);
    }

    if (!new_items) {
        return LIST_ERR__ALLOC;
    }
    dst->items = new_items;
    dst->count = src->count;
    dst->capacity = src->capacity;
    dst->item_size = src->item_size;

    memcpy(dst->items, src->items, src->item_size*src->capacity);

    return LIST_ERR__OK;
};


void list_clear(List *list) {
    if (list->items) {
        free(list->items);
        list->items = NULL;
    }
    list->capacity = 0;
    list->count = 0;
};


inline size_t list_get_count(List const *list) {
    return list->count;
};


inline void * list_at(List *list, size_t index) {
    return list->items + index*list->item_size;
};


inline void const * list_atc(List const *list, size_t index) {
    return list_at((List *)(list), index);
};


ListErr list_insert_at(List *list, size_t index, size_t insert_count) {
    ListErr err;

    if (index > list_get_count(list)) {
        return LIST_ERR__OUT_INDEX;
    }

    err = ensure_capacity(list, list_get_count(list) + insert_count);

    if (err != LIST_ERR__OK) {
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

    return LIST_ERR__OK;
};


ListErr list_remove_at(List *list, size_t index, size_t delete_count) {
    ListErr err;

    if (delete_count + index > list_get_count(list)) {
        return LIST_ERR__OUT_INDEX;
    }

    if (delete_count == 0) {
        return LIST_ERR__OK;
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

    return LIST_ERR__OK;
};


inline ListErr list_append(List *list, size_t insert_count) {
    return list_insert_at(list, list->count, insert_count);
};


ListErr ensure_capacity(List *list, size_t capacity) {
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
        return LIST_ERR__OK;
    }

    void *new_items;
    if (!list->items) {
        new_items = malloc(new_capacity*list->item_size);
    } else {
        new_items = realloc(list->items, new_capacity*list->item_size);
    }

    if (!new_items) {
        return LIST_ERR__ALLOC;
    }

    list->items = new_items;
    list->capacity = new_capacity;
    return LIST_ERR__OK;
};
