#include "./matrix_csr.h"
#include "./list.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#ifndef MAX
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif


#define __MATRIX_CSR_BOOL__TRANSPOSED_OFFSET 00


#define __MATRIX_CSR_IS_TRANSPOSED(m) \
    ((1 << __MATRIX_CSR_BOOL__TRANSPOSED_OFFSET) & (m)->bools)


typedef struct {
    MATRIX_CSR_SCALAR_T value;
    size_t col_index;
    size_t row_index;
} Entry;


struct MatrixCSR {
    size_t col_count;
    size_t row_count;

    List entries;
    List row_indexes;
    List col_indexes;

    uint32_t bools;
};


static SpmErr matrix_prod_tn(MatrixCSR const *, MatrixCSR const *, MatrixCSR *);
static SpmErr matrix_prod_nn(MatrixCSR const *, MatrixCSR const *, MatrixCSR *);
static SpmErr matrix_prod_nt(MatrixCSR const *, MatrixCSR const *, MatrixCSR *);
static SpmErr matrix_value_remove(MatrixCSR *, size_t row, size_t col);
static SpmErr matrix_value_insert(MatrixCSR *, size_t row, size_t col, MATRIX_CSR_SCALAR_T);
static SpmErr matrix_entry_insert_at(MatrixCSR *, size_t entry_index, Entry);
static void matrix_toggle_transpose_bool(MatrixCSR *);
static void matrix_transpose_values(MatrixCSR *);
static void matrix_rebuild_col_indexes(MatrixCSR *);
static SpmErr entry_insert(List *, size_t entry_index, Entry);
static void entry_remove(List *, size_t entry_index);
static void entry_sort_by_row(List *, size_t start, size_t end);
static Entry * entry_get_at(MatrixCSR *, size_t row, size_t col);
static Entry const * entry_getc_at(MatrixCSR const *, size_t row, size_t col);
static size_t entry_index_get_at(MatrixCSR const *, size_t row, size_t col);
static size_t entry_index_get_with_col(Entry *,size_t col, size_t start, size_t end);
static void entry_swap(Entry *, Entry *);
static void list_swap(List *, List *);
static void size_swap(size_t *, size_t *);
static void ptr_swap(void **, void **);


MatrixCSR * matrix_csr_make(size_t row_count, size_t col_count) {
    int err;

    MatrixCSR *m = malloc(sizeof(MatrixCSR));
    if (!m) {
        goto MatrixMakeAllocFailed;
    }

    m->col_count = col_count;
    m->row_count = row_count;
    size_t const max_dim_count = MAX(col_count, row_count);
    m->bools = 0;
    err = list_init(&m->entries, sizeof(Entry), max_dim_count);
    if (err) {
        goto MatrixMakeEntriesAllocFailed;
    }

    err = list_init(&m->row_indexes, sizeof(size_t), max_dim_count + 1);
    if (err) {
        goto MatrixMakeRowIndexesAllocFailed;
    }
    err = list_insert_at(&m->row_indexes, 0, max_dim_count + 1);
    if (err) {
        goto MatrixMakeRowIndexesInsertFailed;
    }
    for (size_t i = 0; i < row_count + 1; i++) {
        *(size_t *) list_at(&m->row_indexes, i) = 0;
    }

    err = list_init(&m->col_indexes, sizeof(size_t), max_dim_count + 1);
    if (err) {
        goto MatrixMakeColIndexesAllocFailed;
    }
    err = list_insert_at(&m->col_indexes, 0, max_dim_count + 1);
    if (err) {
        goto MatrixMakeColIndexesInsertFailed;
    }
    for (size_t j = 0; j < col_count + 1; j++) {
        *(size_t *) list_at(&m->col_indexes, j) = 0;
    }

    return m;
MatrixMakeColIndexesInsertFailed:
    list_clear(&m->row_indexes);
MatrixMakeColIndexesAllocFailed:
MatrixMakeRowIndexesInsertFailed:
    list_clear(&m->row_indexes);
MatrixMakeRowIndexesAllocFailed:
    list_clear(&m->entries);
MatrixMakeEntriesAllocFailed:
    free(m);
MatrixMakeAllocFailed:
    return NULL;
};



MatrixCSR * matrix_csr_clone(MatrixCSR const *src) {
    SpmErr err;
    MatrixCSR *dst = malloc(sizeof(MatrixCSR));
    if (!dst) {
        return NULL;
    }

    dst->col_count = src->col_count;
    dst->row_count = src->row_count;
    dst->bools = src->bools;

    list_init(&dst->entries, sizeof(Entry), 0);
    list_init(&dst->row_indexes, sizeof(size_t), 0);
    list_init(&dst->col_indexes, sizeof(size_t), 0);

    err = list_cpy(&dst->entries, &src->entries);
    if (err != SPM_ERR__OK) {
        goto MatrixCloneListCopyFailed;
    }

    err = list_cpy(&dst->row_indexes, &src->row_indexes);
    if (err != SPM_ERR__OK) {
        goto MatrixCloneListCopyFailed;
    }

    err = list_cpy(&dst->col_indexes, &src->col_indexes);
    if (err != SPM_ERR__OK) {
        goto MatrixCloneListCopyFailed;
    }

    return dst;
MatrixCloneListCopyFailed:
    matrix_csr_destroy(dst);
    return NULL;
};


void matrix_csr_destroy(MatrixCSR *m) {
    list_clear(&m->col_indexes);
    list_clear(&m->row_indexes);
    list_clear(&m->entries);
    free(m);
};


inline size_t matrix_csr_get_col_count(MatrixCSR const *m) {
    return __MATRIX_CSR_IS_TRANSPOSED(m) ? m->row_count : m->col_count;
};


inline size_t matrix_csr_get_row_count(MatrixCSR const *m) {
    return __MATRIX_CSR_IS_TRANSPOSED(m) ? m->col_count : m->row_count;
};


MATRIX_CSR_SCALAR_T matrix_csr_get(MatrixCSR const *m, size_t row, size_t col) {
    if (__MATRIX_CSR_IS_TRANSPOSED(m)) size_swap(&row, &col);

    if (row >= m->row_count || col >= m->col_count) {
        return MATRIX_CSR_SCALAR_ZERO;
    }

    Entry const *entry = entry_get_at((MatrixCSR *)m, row, col);
    if (!entry) {
        return MATRIX_CSR_SCALAR_ZERO;
    }
    return entry->value;
};


SpmErr matrix_csr_set(
    MatrixCSR *m,
    size_t row,
    size_t col,
    MATRIX_CSR_SCALAR_T value
) {
    if (__MATRIX_CSR_IS_TRANSPOSED(m)) size_swap(&row, &col);

    if (row >= m->row_count || col >= m->col_count) {
        return SPM_ERR__OUT_INDEX;
    }

    Entry * entry = entry_get_at(m, row, col);
    int new_value_is_zero = MATRIX_CSR_SCALAR_IS_ZERO(value);

    if (!entry && new_value_is_zero) {
        return SPM_ERR__OK;
    } else if (!entry && !new_value_is_zero) {
        return matrix_value_insert(m, row, col, value);
    } else if (entry && new_value_is_zero) {
        return matrix_value_remove(m, row, col);
    } else if (entry && !new_value_is_zero) {
        entry->value = value;
    }

    return SPM_ERR__OK;
};


inline void matrix_csr_transpose(MatrixCSR *m) {
    matrix_toggle_transpose_bool(m);
    //matrix_transpose_values(m);
};


MatrixCSR * matrix_csr_prod(MatrixCSR const *A, MatrixCSR const *B) {
    SpmErr err;
    size_t const common_count = matrix_csr_get_col_count(A);
    if (common_count != matrix_csr_get_row_count(B)) {
        goto MatrixProdDimErr;
    }

    size_t const row_count = matrix_csr_get_row_count(A);
    size_t const col_count = matrix_csr_get_col_count(B);
    MatrixCSR *C = matrix_csr_make(row_count, col_count);

    if (!C) {
        goto MatrixProdAllocErr;
    }

    if (__MATRIX_CSR_IS_TRANSPOSED(A)) {
        if (__MATRIX_CSR_IS_TRANSPOSED(B)) {
            matrix_transpose_values(C);
            err = matrix_prod_nn(B, A, C);
        } else {
            err = matrix_prod_tn(A, B, C);
        }
    } else if (__MATRIX_CSR_IS_TRANSPOSED(B)) {
        err = matrix_prod_nt(A, B, C);
    } else {
        err = matrix_prod_nn(A, B, C);
    }

    if (err) {
        goto MatrixProdEntryAllocErr;
    }


    if (__MATRIX_CSR_IS_TRANSPOSED(A) && __MATRIX_CSR_IS_TRANSPOSED(B)) {
        matrix_transpose_values(C);
    }

    return C;

MatrixProdEntryAllocErr:
    matrix_csr_destroy(C);
MatrixProdAllocErr:
MatrixProdDimErr:
    return NULL;
};


SpmErr matrix_prod_tn(
    MatrixCSR const *A,
    MatrixCSR const *B,
    MatrixCSR *C
) {
    MatrixCSR *At = matrix_csr_clone(A);
    if (!At) {
        return SPM_ERR__ALLOC;
    }

    matrix_transpose_values(At);
    matrix_toggle_transpose_bool(At);

    SpmErr err = matrix_prod_nn(At, B, C);

    matrix_csr_destroy(At);
    return err;
}


SpmErr matrix_prod_nn(
    MatrixCSR const *A,
    MatrixCSR const *B,
    MatrixCSR *C
) {
    SpmErr err = SPM_ERR__OK;
    size_t const row_count = matrix_csr_get_row_count(C);
    size_t const col_count = matrix_csr_get_col_count(C);

    for (size_t row = 0; row < row_count; row++) {
        size_t const A_start = *(size_t *)list_atc(&A->row_indexes, row);
        size_t const A_end = *(size_t *)list_atc(&A->row_indexes, row+1);

        for (size_t col = 0; col < col_count; col++) {
            MATRIX_CSR_SCALAR_T value = MATRIX_CSR_SCALAR_ZERO;

            for (size_t A_curr = A_start; A_curr < A_end; A_curr++) {
                Entry const *A_entry = list_atc(&A->entries, A_curr);
                Entry const *B_entry = entry_getc_at(B, A_entry->col_index,col);
                if (!B_entry) {
                    continue;
                }
                value = MATRIX_CSR_SCALAR_SUM(
                    value,
                    MATRIX_CSR_SCALAR_PROD(A_entry->value, B_entry->value)
                );
            }

            if (MATRIX_CSR_SCALAR_IS_ZERO(value)) {
                continue;
            }

            Entry entry = {.value=value, .col_index=col, .row_index=row};
            err = matrix_entry_insert_at(C, list_get_count(&C->entries), entry);
            if (err != SPM_ERR__OK) {
                return err;
            }
        }
    }
    return SPM_ERR__OK;
}


SpmErr matrix_prod_nt(
    MatrixCSR const *A,
    MatrixCSR const *B,
    MatrixCSR *C
) {
    SpmErr err = SPM_ERR__OK;
    size_t const row_count = matrix_csr_get_row_count(C);
    size_t const col_count = matrix_csr_get_col_count(C);

    for (size_t row = 0; row < row_count; row++) {
        size_t const A_start = *(size_t *)list_atc(&A->row_indexes, row);
        size_t const A_end = *(size_t *)list_atc(&A->row_indexes, row+1);

        for (size_t col = 0; col < col_count; col++) {
            size_t const B_start = *(size_t *)list_atc(&B->row_indexes, col);
            size_t const B_end = *(size_t *)list_atc(&B->row_indexes, col+1);

            MATRIX_CSR_SCALAR_T value = MATRIX_CSR_SCALAR_ZERO;
            size_t A_curr = A_start;
            size_t B_curr = B_start;

            while (A_curr < A_end && B_curr < B_end) {
                Entry const *A_entry = list_atc(&A->entries, A_curr);
                Entry const *B_entry = list_atc(&B->entries, B_curr);

                if (A_entry->col_index < B_entry->col_index) {
                    A_curr++;
                } else if (B_entry->col_index < A_entry->col_index) {
                    B_curr++;
                } else {
                    value = MATRIX_CSR_SCALAR_SUM(
                        value,
                        MATRIX_CSR_SCALAR_PROD(A_entry->value, B_entry->value)
                    );
                    A_curr++;
                    B_curr++;
                }
            }

            if (MATRIX_CSR_SCALAR_IS_ZERO(value)) {
                continue;
            }

            Entry entry = {.value=value, .col_index=col, .row_index=row};
            err = matrix_entry_insert_at(C, list_get_count(&C->entries), entry);
            if (err != SPM_ERR__OK) {
                return err;
            }
        }
    }
    return SPM_ERR__OK;
}


inline SpmErr matrix_value_insert(
    MatrixCSR *m,
    size_t row,
    size_t col,
    MATRIX_CSR_SCALAR_T value
) {
    size_t index = entry_index_get_at(m, row, col);
    Entry entry = {.value=value, .col_index=col, .row_index=row};
    return matrix_entry_insert_at(m, index, entry);
}


SpmErr matrix_entry_insert_at(MatrixCSR *m, size_t index, Entry entry) {
    SpmErr err;

    err = entry_insert(&m->entries, index, entry);

    if (err != SPM_ERR__OK) {
        return err;
    }

    size_t row_count = m->row_count;
    for (size_t i = entry.row_index; i < row_count; i++) {
        *(size_t*)list_at(&m->row_indexes, i + 1) += 1;
    }

    size_t col_count = m->col_count;
    for (size_t j = entry.col_index; j < col_count; j++) {
        *(size_t*)list_at(&m->col_indexes, j + 1) += 1;
    }

    return SPM_ERR__OK;
};


SpmErr matrix_value_remove(
    MatrixCSR *m,
    size_t row,
    size_t col
) {
    size_t entry_index = entry_index_get_at(m, row, col);
    entry_remove(&m->entries, entry_index);

    size_t row_count = m->row_count;
    for (size_t i = row; i < row_count; i++) {
        *(size_t*)list_at(&m->row_indexes, i + 1) -= 1;
    }

    size_t col_count = m->col_count;
    for (size_t j = col; j < col_count; j++) {
        *(size_t*)list_at(&m->col_indexes, j + 1) -= 1;
    }

    return SPM_ERR__OK;
}


void matrix_toggle_transpose_bool(MatrixCSR *m) {
    uint32_t mask = 1 << __MATRIX_CSR_BOOL__TRANSPOSED_OFFSET;
    uint32_t is_transposed = m->bools & mask;
    is_transposed = mask - is_transposed;

    m->bools = (m->bools & (~mask)) | is_transposed;
};


void matrix_transpose_values(MatrixCSR *m) {
    size_swap(&m->row_count, &m->col_count);
    list_swap(&m->row_indexes, &m->col_indexes);
    size_t const entry_count = list_get_count(&m->entries);
    for (size_t i = 0; i < entry_count; i++) {
        Entry *entry = list_at(&m->entries, i);
        size_swap(&entry->row_index, &entry->col_index);
    }

    if (entry_count == 0) {
        return;
    }

    Entry *entries = (Entry *)m->entries.items;
    size_t *new_row_starts = (size_t *)m->row_indexes.items;
    size_t *write_pointers = (size_t *)m->col_indexes.items;
    for (size_t i = 0; i < m->row_count; i++) {
        write_pointers[i] = new_row_starts[i];
    }

    for (size_t orig_index = 0; orig_index < m->row_count; orig_index++) {
        while (write_pointers[orig_index] < new_row_starts[orig_index + 1]) {
            size_t curr_idx = write_pointers[orig_index];
            Entry *e = &entries[curr_idx];

            size_t target_row = e->row_index;
            size_t dest_idx = write_pointers[target_row];

            if (curr_idx == dest_idx) {
                write_pointers[orig_index]++;
            } else {
                entry_swap(e, &entries[dest_idx]);
                write_pointers[target_row]++;
            }
        }
    }

    matrix_rebuild_col_indexes(m);
}


void matrix_rebuild_col_indexes(MatrixCSR *m) {
    size_t entry_count = m->entries.count;
    if (entry_count == 0) return;

    Entry *entries = (Entry *)m->entries.items;
    size_t *col_indexes = (size_t *)m->col_indexes.items;
    size_t col_count = m->col_count;

    memset(col_indexes, 0, (col_count + 1) * sizeof(size_t));

    for (size_t i = 0; i < entry_count; i++) {
        size_t col = entries[i].col_index;
        col_indexes[col + 1]++;
    }

    for (size_t i = 0; i < col_count; i++) {
        col_indexes[i + 1] += col_indexes[i];
    }

    m->col_indexes.count = col_count + 1;
}


SpmErr entry_insert(List *entries, size_t entry_index, Entry entry) {
    SpmErr err = list_insert_at(entries, entry_index, 1);
    switch (err) {
        case SPM_ERR__OK: break;
        case SPM_ERR__ALLOC: return SPM_ERR__ALLOC;
        default: return SPM_ERR__UNKNOWN;
    }

    *(Entry *)list_at(entries, entry_index) = entry;

    return SPM_ERR__OK;
};


void entry_remove(List *entries, size_t entry_index) {
    list_remove_at(entries, entry_index, 1);
};


void entry_sort_by_row(List *entries, size_t start, size_t end) {
    if (end <= start + 1) {
        return;
    }

    size_t middle = start + (end - start)/2;
    entry_sort_by_row(entries, start, middle);
    entry_sort_by_row(entries, middle, end);

    while (middle < end) {
        for (size_t i = middle; i > 0; i--) {
            Entry * a = list_at(entries, i-1);
            Entry * b = list_at(entries, i);
            if (a->row_index > b->row_index) {
                entry_swap(a,b);
            } else {
                break;
            }
        }
        middle++;
    }
}


Entry * entry_get_at(MatrixCSR *m, size_t row, size_t col) {
    size_t entry_index = entry_index_get_at(m, row, col);
    size_t entry_index_end = *(size_t *)list_atc(&m->row_indexes, row+1);

    if (entry_index == entry_index_end) {
        return NULL;
    }

    Entry * entry = list_at(&m->entries, entry_index);

    if (entry->col_index != col) {
        return NULL;
    }
    return entry;
}


inline Entry const * entry_getc_at(MatrixCSR const *m, size_t row, size_t col) {
    return entry_get_at((MatrixCSR *)m, row, col);
}


size_t entry_index_get_at(MatrixCSR const *m, size_t row, size_t col) {
    if (row >= m->row_count || col >= m->col_count) {
        return list_get_count(&m->entries);
    }
    size_t entry_index = *(size_t *)list_atc(&m->row_indexes, row);
    size_t entry_index_end = *(size_t *)list_atc(&m->row_indexes, row+1);

    return entry_index_get_with_col(
        m->entries.items,
        col,
        entry_index,
        entry_index_end
    );
};


size_t entry_index_get_with_col(
    Entry *entries,
    size_t col,
    size_t start,
    size_t end
) {
    if (start >= end) {
        return start;
    }

    size_t middle = (start + end)/2;
    if (entries[middle].col_index < col) {
        return entry_index_get_with_col(entries, col, middle + 1, end);
    } else if (entries[middle].col_index > col) {
        return entry_index_get_with_col(entries, col, start, middle);
    }

    return middle;
};


inline void entry_swap(Entry *a, Entry *b) {
    Entry c = *a;
    *a = *b;
    *b = c;
};


inline void list_swap(List *a, List *b) {
    List c = *a;
    *a = *b;
    *b = c;
};


inline void size_swap(size_t *a, size_t *b) {
    size_t c = *a;
    *a = *b;
    *b = c;
};


inline void ptr_swap(void **a, void **b) {
    void *c = *a;
    *a = *b;
    *b = c;
};
