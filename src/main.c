#include <stdlib.h>
#include <stdio.h>


#include "./list.h"
#include "./matrix.h"



#define RED(s) "\x1b[31m" s "\x1b[0m"
#define GREEN(s) "\x1b[32m" s "\x1b[0m"
#define DEBUG(s) fprintf(stderr, "[%s:%d]: %s", __FILE__, __LINE__, ""#s)
#define ASSERT(b) do { if (b) continue; DEBUG(); return 1; } while(0)
#define ASSERT_NOT(b) ASSERT(!(b))
#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_NEQ(a, b) ASSERT((a) != (b))
#define ASSERT_NULL(b) ASSERT_NOT(b)
#define ASSERT_NOT_NULL(b) ASSERT(b)

SPM_LIST_HEADER(size_t, Size, size);

typedef struct {
    char name[32];
    int (* const call)(void);
} Test;

static void spm_list_int_print(SpmListSize const *);
static void spm_matrix_print(SpmMatrix const *);
static int test_list_001(void);
static int test_list_002(void);
static int test_matrix_001(void);
static int test_matrix_002(void);
static int test_matrix_003(void);
static int test_matrix_004(void);
static int test_matrix_005(void);
static int test_matrix_006(void);
static int test_matrix_007(void);
static int test_matrix_008(void);
static int test_matrix_009(void);
static int test_matrix_010(void);
static int test_matrix_011(void);


int main(void) {
    static Test tests[] = {
        {"List 001", test_list_001},
        {"List 002", test_list_002},
        {"Matrix 001", test_matrix_001},
        {"Matrix 002", test_matrix_002},
        {"Matrix 003", test_matrix_003},
        {"Matrix 004", test_matrix_004},
        {"Matrix 005", test_matrix_005},
        {"Matrix 006", test_matrix_006},
        {"Matrix 007", test_matrix_007},
        {"Matrix 008", test_matrix_008},
        {"Matrix 009", test_matrix_009},
        {"Matrix 010", test_matrix_010},
        {"Matrix 011", test_matrix_011},
        {.call=NULL},
    };

    for (unsigned int i = 0; tests[i].call != NULL; i++) {
        Test test = tests[i];
        printf("%s. Running...", test.name);
        fflush(stdout);

        if (test.call()) {
            printf(" " RED("Failed") ". \n");
        } else {
            printf(" " GREEN("Success") ". \n");
        }

    }

    return EXIT_SUCCESS;
}


int test_list_001(void) {
    int err;
    SpmListSize numbers = {0};
    spm_list_size_init(&numbers, 10);
    ASSERT_EQ(0, spm_list_size_get_count(&numbers));

    err = spm_list_size_insert_at(&numbers, 0, 10);
    ASSERT_NOT(err);
    ASSERT_EQ(10, spm_list_size_get_count(&numbers));

    for (size_t i = 0; i < spm_list_size_get_count(&numbers); i++) {
        spm_list_size_set(&numbers, i, i);
    }

    for (size_t i = 0; i < spm_list_size_get_count(&numbers); i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i);
    }

    err = spm_list_size_insert_at(&numbers, 3, 18);
    ASSERT_NOT(err);
    ASSERT_EQ(28, spm_list_size_get_count(&numbers));
    for (size_t i = 0; i < 3; i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i);
    }
    for (size_t i = 21; i < 28; i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i-18);
    }


    err = spm_list_size_remove_at(&numbers, 5, 7);
    ASSERT_NOT(err);
    ASSERT_EQ(21, spm_list_size_get_count(&numbers));
    for (size_t i = 0; i < 3; i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i);
    }
    for (size_t i = 14; i < 21; i++) {
        ASSERT_EQ(spm_list_size_get(&numbers, i), (size_t) i-11);
    }

    spm_list_size_clear(&numbers);
    return 0;
}


int test_list_002(void) {
    int err;
    SpmListSize numbers = {0};
    spm_list_size_init(&numbers, 2);

    for (int i = 0; i < 100; i++) {
        err = spm_list_size_insert_at(&numbers, spm_list_size_get_count(&numbers), 1);
        ASSERT_NOT(err);
        spm_list_size_set(&numbers, i, i);
    }
    ASSERT_EQ(100, spm_list_size_get_count(&numbers));

    spm_list_size_remove_at(&numbers, 0, 10);
    ASSERT_EQ(90, spm_list_size_get_count(&numbers));
    ASSERT_EQ(10, spm_list_size_get(&numbers, 0));

    spm_list_size_remove_at(&numbers, spm_list_size_get_count(&numbers) - 10, 10);
    ASSERT_EQ(80, spm_list_size_get_count(&numbers));

    spm_list_size_clear(&numbers);
    return 0;
}


int test_matrix_001(void) {
    int err;
    SpmMatrix *m = spm_matrix_make(4, 4);
    ASSERT_NOT_NULL(m);
    ASSERT_EQ(4, spm_matrix_get_row_count(m));
    ASSERT_EQ(4, spm_matrix_get_col_count(m));

    err = spm_matrix_set(m, 2, 3, 5.1);
    ASSERT_NOT(err);
    err = spm_matrix_set(m, 2, 1, 3.6);
    ASSERT_NOT(err);

    for (size_t i = 0; i < spm_matrix_get_row_count(m); i++) {
        for (size_t j = 0; j < spm_matrix_get_col_count(m); j++) {
            double value = spm_matrix_get(m, i, j);
            if (i == 2 && j == 3) {
                ASSERT_EQ(5.1, value);
            } else if (i == 2 && j == 1) {
                ASSERT_EQ(3.6, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    spm_matrix_transpose(m);
    for (size_t i = 0; i < spm_matrix_get_row_count(m); i++) {
        for (size_t j = 0; j < spm_matrix_get_col_count(m); j++) {
            double value = spm_matrix_get(m, i, j);
            if (i == 3 && j == 2) {
                ASSERT_EQ(5.1, value);
            } else if (i == 1 && j == 2) {
                ASSERT_EQ(3.6, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    spm_matrix_transpose(m);
    for (size_t i = 0; i < spm_matrix_get_row_count(m); i++) {
        for (size_t j = 0; j < spm_matrix_get_col_count(m); j++) {
            double value = spm_matrix_get(m, i, j);
            if (i == 2 && j == 3) {
                ASSERT_EQ(5.1, value);
            } else if (i == 2 && j == 1) {
                ASSERT_EQ(3.6, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    spm_matrix_destroy(m);

    return 0;
}


int test_matrix_002(void) {
    int err;
    SpmMatrix *A = spm_matrix_make(2, 4);
    ASSERT_NOT_NULL(A);
    ASSERT_EQ(2, spm_matrix_get_row_count(A));
    ASSERT_EQ(4, spm_matrix_get_col_count(A));

    ASSERT_NOT(spm_matrix_set(A, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(A, 1, 1, 2.0));
    ASSERT_NOT(spm_matrix_set(A, 0, 2, 3.0));
    ASSERT_NOT(spm_matrix_set(A, 1, 3, 4.0));

    SpmMatrix *B = spm_matrix_make(4, 4);
    ASSERT_NOT_NULL(B);
    ASSERT_EQ(4, spm_matrix_get_row_count(B));
    ASSERT_EQ(4, spm_matrix_get_col_count(B));

    ASSERT_NOT(spm_matrix_set(B, 2, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 1, 2.0));
    ASSERT_NOT(spm_matrix_set(B, 0, 3, 3.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 3, 4.0));
    ASSERT_NOT(spm_matrix_set(B, 2, 3, 5.0));
    ASSERT_NOT(spm_matrix_set(B, 3, 3, 6.0));

    SpmMatrix *C = spm_matrix_prod(A, B);
    ASSERT_NOT_NULL(C);

    for (size_t i = 0; i < spm_matrix_get_row_count(C); i++) {
        for (size_t j = 0; j < spm_matrix_get_col_count(C); j++) {
            double value = spm_matrix_get(C, i, j);
            if (i == 0 && j == 0) {
                ASSERT_EQ(3.0, value);
            } else if (i == 1 && j == 1) {
                ASSERT_EQ(4.0, value);
            } else if (i == 0 && j == 3) {
                ASSERT_EQ(18.0, value);
            } else if (i == 1 && j == 3) {
                ASSERT_EQ(32.0, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    spm_matrix_destroy(C);
    spm_matrix_destroy(B);
    spm_matrix_destroy(A);

    return 0;
}


int test_matrix_003(void) {
    SpmMatrix *m = spm_matrix_make(3, 3);
    ASSERT_NOT_NULL(m);

    ASSERT_NOT(spm_matrix_set(m, 1, 1, 10.5));
    ASSERT_EQ(10.5, spm_matrix_get(m, 1, 1));

    ASSERT_NOT(spm_matrix_set(m, 1, 1, 42.0));
    ASSERT_EQ(42.0, spm_matrix_get(m, 1, 1));

    ASSERT_NOT(spm_matrix_set(m, 0, 0, 0.0));
    ASSERT_EQ(0.0, spm_matrix_get(m, 0, 0));

    ASSERT_NOT(spm_matrix_set(m, 1, 1, 0.0));
    ASSERT_EQ(0.0, spm_matrix_get(m, 1, 1));

    for (size_t i = 0; i < spm_matrix_get_row_count(m); i++) {
        for (size_t j = 0; j < spm_matrix_get_col_count(m); j++) {
            ASSERT_EQ(0.0, spm_matrix_get(m, i, j));
        }
    }

    spm_matrix_destroy(m);
    return 0;
}


int test_matrix_004(void) {
    SpmMatrix *m = spm_matrix_make(2, 5);
    ASSERT_NOT_NULL(m);

    ASSERT_NOT(spm_matrix_set(m, 0, 4, 9.9));
    ASSERT_NOT(spm_matrix_set(m, 1, 2, 8.8));

    spm_matrix_transpose(m);
    ASSERT_EQ(5, spm_matrix_get_row_count(m));
    ASSERT_EQ(2, spm_matrix_get_col_count(m));

    ASSERT_EQ(9.9, spm_matrix_get(m, 4, 0));
    ASSERT_EQ(8.8, spm_matrix_get(m, 2, 1));
    ASSERT_EQ(0.0, spm_matrix_get(m, 0, 4));

    ASSERT_NOT(spm_matrix_set(m, 1, 0, 7.7));

    spm_matrix_transpose(m);
    ASSERT_EQ(2, spm_matrix_get_row_count(m));
    ASSERT_EQ(5, spm_matrix_get_col_count(m));

    ASSERT_EQ(7.7, spm_matrix_get(m, 0, 1));

    spm_matrix_destroy(m);
    return 0;
}


int test_matrix_005(void) {
    SpmMatrix *A = spm_matrix_make(2, 3);
    ASSERT_NOT_NULL(A);
    ASSERT_NOT(spm_matrix_set(A, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(A, 0, 1, 2.0));
    ASSERT_NOT(spm_matrix_set(A, 1, 2, 3.0));

    SpmMatrix *BadDim = spm_matrix_make(4, 2);
    SpmMatrix *C_err = spm_matrix_prod(A, BadDim);
    ASSERT_NULL(C_err);
    spm_matrix_destroy(BadDim);

    SpmMatrix *Zero = spm_matrix_make(3, 2);
    ASSERT_NOT_NULL(Zero);
    SpmMatrix *C_zero = spm_matrix_prod(A, Zero);
    ASSERT_NOT_NULL(C_zero);
    for (size_t i = 0; i < spm_matrix_get_row_count(C_zero); i++) {
        for (size_t j = 0; j < spm_matrix_get_col_count(C_zero); j++) {
            ASSERT_EQ(0.0, spm_matrix_get(C_zero, i, j));
        }
    }

    SpmMatrix *Id = spm_matrix_make(3, 3);
    ASSERT_NOT_NULL(Id);
    ASSERT_NOT(spm_matrix_set(Id, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(Id, 1, 1, 1.0));
    ASSERT_NOT(spm_matrix_set(Id, 2, 2, 1.0));

    SpmMatrix *C_id = spm_matrix_prod(A, Id);
    ASSERT_NOT_NULL(C_id);
    ASSERT_EQ(1.0, spm_matrix_get(C_id, 0, 0));
    ASSERT_EQ(2.0, spm_matrix_get(C_id, 0, 1));
    ASSERT_EQ(3.0, spm_matrix_get(C_id, 1, 2));
    ASSERT_EQ(0.0, spm_matrix_get(C_id, 1, 0));

    spm_matrix_destroy(C_id);
    spm_matrix_destroy(Id);
    spm_matrix_destroy(C_zero);
    spm_matrix_destroy(Zero);
    spm_matrix_destroy(A);

    return 0;
}


int test_matrix_006(void) {
    SpmMatrix *m = spm_matrix_make(3, 3);
    ASSERT_NOT_NULL(m);

    ASSERT_EQ(SPM_ERR__OUT_INDEX, spm_matrix_set(m, 3, 0, 1.0));
    ASSERT_EQ(SPM_ERR__OUT_INDEX, spm_matrix_set(m, 100, 0, 1.0));

    ASSERT_EQ(SPM_ERR__OUT_INDEX, spm_matrix_set(m, 0, 3, 1.0));

    ASSERT_EQ(0.0, spm_matrix_get(m, 3, 3));
    ASSERT_EQ(0.0, spm_matrix_get(m, 0, 10));

    spm_matrix_transpose(m);

    SpmMatrix *m2 = spm_matrix_make(2, 4);
    spm_matrix_transpose(m2);

    ASSERT_NOT(spm_matrix_set(m2, 3, 1, 5.0));
    ASSERT_EQ(SPM_ERR__OUT_INDEX, spm_matrix_set(m2, 1, 3, 5.0));

    spm_matrix_destroy(m);
    spm_matrix_destroy(m2);
    return 0;
}


int test_matrix_007(void) {
    SpmMatrix *m = spm_matrix_make(3, 3);

    ASSERT_NOT(spm_matrix_set(m, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(m, 1, 1, 2.0));
    ASSERT_NOT(spm_matrix_set(m, 2, 2, 3.0));

    ASSERT_NOT(spm_matrix_set(m, 1, 1, 0.0));
    ASSERT_EQ(0.0, spm_matrix_get(m, 1, 1));

    ASSERT_NOT(spm_matrix_set(m, 0, 1, 0.0));

    spm_matrix_destroy(m);
    return 0;
}


int test_matrix_prod_007(void) {
    SpmMatrix *A = spm_matrix_make(2, 3);
    ASSERT_NOT(spm_matrix_set(A, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(A, 0, 1, 2.0));
    ASSERT_NOT(spm_matrix_set(A, 1, 1, 3.0));
    ASSERT_NOT(spm_matrix_set(A, 1, 2, 4.0));

    SpmMatrix *B = spm_matrix_make(3, 2);
    ASSERT_NOT(spm_matrix_set(B, 0, 0, 2.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 1, 3.0));
    ASSERT_NOT(spm_matrix_set(B, 2, 1, 1.0));

    SpmMatrix *C = spm_matrix_prod(A, B);
    ASSERT_NOT_NULL(C);
    ASSERT_EQ(2, spm_matrix_get_row_count(C));
    ASSERT_EQ(2, spm_matrix_get_col_count(C));

    ASSERT_EQ(4.0, spm_matrix_get(C, 0, 0));
    ASSERT_EQ(6.0, spm_matrix_get(C, 0, 1));
    ASSERT_EQ(3.0, spm_matrix_get(C, 1, 0));
    ASSERT_EQ(13.0, spm_matrix_get(C, 1, 1));

    spm_matrix_destroy(C);
    spm_matrix_destroy(B);
    spm_matrix_destroy(A);
    return 0;
}


int test_matrix_008(void) {
    SpmMatrix *A = spm_matrix_make(2, 3);
    ASSERT_NOT(spm_matrix_set(A, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(A, 0, 1, 2.0));
    ASSERT_NOT(spm_matrix_set(A, 1, 2, 3.0));

    SpmMatrix *B = spm_matrix_make(2, 3);
    ASSERT_NOT(spm_matrix_set(B, 0, 0, 2.0));
    ASSERT_NOT(spm_matrix_set(B, 0, 1, 1.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 1, 3.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 2, 1.0));
    spm_matrix_transpose(B);

    SpmMatrix *C = spm_matrix_prod(A, B);
    ASSERT_NOT_NULL(C);

    ASSERT_EQ(4.0, spm_matrix_get(C, 0, 0));

    spm_matrix_destroy(C);
    spm_matrix_destroy(B);
    spm_matrix_destroy(A);
    return 0;
}


int test_matrix_009(void) {
    // A_orig = [1.0, 2.0]
    //          [0.0, 0.0]
    //          [0.0, 0.0]
    SpmMatrix *A = spm_matrix_make(3, 2);
    ASSERT_NOT(spm_matrix_set(A, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(A, 0, 1, 2.0));
    spm_matrix_transpose(A);

    // B_orig = [1.0, 0.0]
    //          [3.0, 0.0]
    //          [0.0, 0.0]
    SpmMatrix *B = spm_matrix_make(3, 2);
    ASSERT_NOT(spm_matrix_set(B, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 0, 3.0));

    SpmMatrix *C = spm_matrix_prod(A, B);
    ASSERT_NOT_NULL(C);
    ASSERT_EQ(2, spm_matrix_get_row_count(C));
    ASSERT_EQ(2, spm_matrix_get_col_count(C));

    for (size_t row = 0; row < 2; row++) {
        for (size_t col = 0; col < 2; col++) {
            if (row == 0 && col == 0) {
                ASSERT_EQ(1.0, spm_matrix_get(C, row, col));
            } else if (row == 1 && col == 0) {
                ASSERT_EQ(2.0, spm_matrix_get(C, row, col));
            } else {
                ASSERT_EQ(0.0, spm_matrix_get(C, row, col));
            }
        }
    }

    spm_matrix_destroy(C);
    spm_matrix_destroy(B);
    spm_matrix_destroy(A);
    return 0;
}


int test_matrix_010(void) {
    // A_orig = [1.0, 2.0, 0.0]
    //          [0.0, 3.0, 4.0]
    SpmMatrix *A = spm_matrix_make(2, 3);
    ASSERT_NOT_NULL(A);
    ASSERT_NOT(spm_matrix_set(A, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(A, 0, 1, 2.0));
    ASSERT_NOT(spm_matrix_set(A, 1, 1, 3.0));
    ASSERT_NOT(spm_matrix_set(A, 1, 2, 4.0));
    spm_matrix_transpose(A);

    // B_orig = [1.0, 0.0]
    //          [2.0, 0.0]
    //          [0.0, 1.0]
    //          [0.0, 3.0]
    SpmMatrix *B = spm_matrix_make(4, 2);
    ASSERT_NOT_NULL(B);
    ASSERT_NOT(spm_matrix_set(B, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 0, 2.0));
    ASSERT_NOT(spm_matrix_set(B, 2, 1, 1.0));
    ASSERT_NOT(spm_matrix_set(B, 3, 1, 3.0));
    spm_matrix_transpose(B);

    SpmMatrix *C = spm_matrix_prod(A, B);
    ASSERT_NOT_NULL(C);
    ASSERT_EQ(3, spm_matrix_get_row_count(C));
    ASSERT_EQ(4, spm_matrix_get_col_count(C));

    for (size_t row = 0; row < 3; row++) {
        for (size_t col = 0; col < 4; col++) {
            if (row == 0 && col == 0) {
                ASSERT_EQ(1.0, spm_matrix_get(C, row, col));
            } else if (row == 0 && col == 1) {
                ASSERT_EQ(2.0, spm_matrix_get(C, row, col));
            } else if (row == 1 && col == 0) {
                ASSERT_EQ(2.0, spm_matrix_get(C, row, col));
            } else if (row == 1 && col == 1) {
                ASSERT_EQ(4.0, spm_matrix_get(C, row, col));
            } else if (row == 1 && col == 2) {
                ASSERT_EQ(3.0, spm_matrix_get(C, row, col));
            } else if (row == 1 && col == 3) {
                ASSERT_EQ(9.0, spm_matrix_get(C, row, col));
            } else if (row == 2 && col == 2) {
                ASSERT_EQ(4.0, spm_matrix_get(C, row, col));
            } else if (row == 2 && col == 3) {
                ASSERT_EQ(12.0, spm_matrix_get(C, row, col));
            } else {
                ASSERT_EQ(0.0, spm_matrix_get(C, row, col));
            }
        }
    }

    spm_matrix_destroy(C);
    spm_matrix_destroy(B);
    spm_matrix_destroy(A);
    return 0;
}

int test_matrix_011(void) {
    // A_orig = [1.0, 2.0]
    //          [0.0, 3.0]
    //          [4.0, 0.0]
    SpmMatrix *A = spm_matrix_make(3, 2);
    ASSERT_NOT_NULL(A);
    ASSERT_NOT(spm_matrix_set(A, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(A, 0, 1, 2.0));
    ASSERT_NOT(spm_matrix_set(A, 1, 1, 3.0));
    ASSERT_NOT(spm_matrix_set(A, 2, 0, 4.0));
    spm_matrix_transpose(A); // Ara A és lògicament 2x3

    // B_orig = [1.0, 0.0]
    //          [2.0, 1.0]
    //          [0.0, 3.0]
    SpmMatrix *B = spm_matrix_make(3, 2);
    ASSERT_NOT_NULL(B);
    ASSERT_NOT(spm_matrix_set(B, 0, 0, 1.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 0, 2.0));
    ASSERT_NOT(spm_matrix_set(B, 1, 1, 1.0));
    ASSERT_NOT(spm_matrix_set(B, 2, 1, 3.0));

    SpmMatrix *C = spm_matrix_prod(A, B);
    ASSERT_NOT_NULL(C);
    ASSERT_EQ(2, spm_matrix_get_row_count(C));
    ASSERT_EQ(2, spm_matrix_get_col_count(C));

    for (size_t row = 0; row < 2; row++) {
        for (size_t col = 0; col < 2; col++) {
            if (row == 0 && col == 0) {
                ASSERT_EQ(1.0, spm_matrix_get(C, row, col));
            } else if (row == 0 && col == 1) {
                ASSERT_EQ(12.0, spm_matrix_get(C, row, col));
            } else if (row == 1 && col == 0) {
                ASSERT_EQ(8.0, spm_matrix_get(C, row, col));
            } else if (row == 1 && col == 1) {
                ASSERT_EQ(3.0, spm_matrix_get(C, row, col));
            } else {
                ASSERT_EQ(0.0, spm_matrix_get(C, row, col));
            }
        }
    }

    spm_matrix_destroy(C);
    spm_matrix_destroy(B);
    spm_matrix_destroy(A);
    return 0;
}


void spm_list_size_print(SpmListSize const *list) {
    printf("%zu: [", spm_list_size_get_count(list));
    for (size_t i = 0; i< spm_list_size_get_count(list); i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%zu", spm_list_size_get(list, i));
    }
    printf("]\n");
};


void spm_matrix_print(SpmMatrix const *m) {
    for (size_t i = 0; i < spm_matrix_get_row_count(m); i++) {
        for (size_t j = 0; j < spm_matrix_get_col_count(m); j++) {
            if (j > 0) {
                printf(" ");
            }
            printf("%.2f", spm_matrix_get(m, i, j));
        }
        printf("\n");
    }
};
