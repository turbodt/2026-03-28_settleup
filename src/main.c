#include <stdlib.h>
#include <stdio.h>


#include "./list.h"
#include "./matrix_csr.h"


#define DEBUG(s) fprintf(stderr, "[%s:%d]: %s", __FILE__, __LINE__, ""#s)
#define ASSERT(b) do { if (b) continue; DEBUG(); return 1; } while(0)
#define ASSERT_NOT(b) ASSERT(!(b))
#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_NEQ(a, b) ASSERT((a) != (b))
#define ASSERT_NULL(b) ASSERT_NOT(b)
#define ASSERT_NOT_NULL(b) ASSERT(b)


typedef struct {
    char name[32];
    int (* const call)(void);
} Test;

static void list_int_print(List const *);
static void matrix_csr_print(MatrixCSR const *);
static int test_list_001(void);
static int test_list_002(void);
static int test_matrix_001(void);
static int test_matrix_002(void);
static int test_matrix_003(void);
static int test_matrix_004(void);
static int test_matrix_005(void);


int main(void) {
    static Test tests[] = {
        {"List 001", test_list_001},
        {"List 002", test_list_002},
        {"Matrix 001", test_matrix_001},
        {"Matrix 002", test_matrix_002},
        {"Matrix 003", test_matrix_003},
        {"Matrix 004", test_matrix_004},
        {"Matrix 005", test_matrix_005},
        {.call=NULL},
    };

    for (unsigned int i = 0; tests[i].call != NULL; i++) {
        Test test = tests[i];
        printf("%s. Running...", test.name);
        fflush(stdout);

        if (test.call()) {
            printf(" Failed!\n");
            return EXIT_FAILURE;
        }

        printf(" Success.\n");
    }

    return EXIT_SUCCESS;
}


int test_list_001(void) {
    int err;
    List numbers = {0};
    list_init(&numbers, sizeof(int), 10);
    ASSERT_EQ(0, list_get_count(&numbers));

    err = list_insert_at(&numbers, 0, 10);
    ASSERT_NOT(err);
    ASSERT_EQ(10, list_get_count(&numbers));

    for (size_t i = 0; i < list_get_count(&numbers); i++) {
        *(int*)list_at(&numbers, i) = (int) i;
    }

    for (size_t i = 0; i < list_get_count(&numbers); i++) {
        ASSERT_EQ(*(int*)list_at(&numbers, i), (double) i);
    }

    err = list_insert_at(&numbers, 3, 18);
    ASSERT_NOT(err);
    ASSERT_EQ(28, list_get_count(&numbers));
    for (size_t i = 0; i < 3; i++) {
        ASSERT_EQ(*(int*)list_at(&numbers, i), (double) i);
    }
    for (size_t i = 21; i < 28; i++) {
        ASSERT_EQ(*(int*)list_at(&numbers, i), (double) i-18);
    }


    err = list_remove_at(&numbers, 5, 7);
    ASSERT_NOT(err);
    ASSERT_EQ(21, list_get_count(&numbers));
    for (size_t i = 0; i < 3; i++) {
        ASSERT_EQ(*(int*)list_at(&numbers, i), (double) i);
    }
    for (size_t i = 14; i < 21; i++) {
        ASSERT_EQ(*(int*)list_at(&numbers, i), (double) i-11);
    }

    list_clear(&numbers);
    return 0;
}


int test_list_002(void) {
    int err;
    List numbers = {0};
    list_init(&numbers, sizeof(int), 2);

    for (int i = 0; i < 100; i++) {
        err = list_insert_at(&numbers, list_get_count(&numbers), 1);
        ASSERT_NOT(err);
        *(int*)list_at(&numbers, i) = i;
    }
    ASSERT_EQ(100, list_get_count(&numbers));

    list_remove_at(&numbers, 0, 10);
    ASSERT_EQ(90, list_get_count(&numbers));
    ASSERT_EQ(10, *(int*)list_at(&numbers, 0));

    list_remove_at(&numbers, list_get_count(&numbers) - 10, 10);
    ASSERT_EQ(80, list_get_count(&numbers));

    list_clear(&numbers);
    return 0;
}


int test_matrix_001(void) {
    int err;
    MatrixCSR *m = matrix_csr_make(4, 4);
    ASSERT_NOT_NULL(m);
    ASSERT_EQ(4, matrix_csr_get_row_count(m));
    ASSERT_EQ(4, matrix_csr_get_col_count(m));

    err = matrix_csr_set(m, 2, 3, 5.1);
    ASSERT_NOT(err);
    err = matrix_csr_set(m, 2, 1, 3.6);
    ASSERT_NOT(err);

    for (size_t i = 0; i < matrix_csr_get_row_count(m); i++) {
        for (size_t j = 0; j < matrix_csr_get_col_count(m); j++) {
            double value = matrix_csr_get(m, i, j);
            if (i == 2 && j == 3) {
                ASSERT_EQ(5.1, value);
            } else if (i == 2 && j == 1) {
                ASSERT_EQ(3.6, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    matrix_csr_transpose(m);
    for (size_t i = 0; i < matrix_csr_get_row_count(m); i++) {
        for (size_t j = 0; j < matrix_csr_get_col_count(m); j++) {
            double value = matrix_csr_get(m, i, j);
            if (i == 3 && j == 2) {
                ASSERT_EQ(5.1, value);
            } else if (i == 1 && j == 2) {
                ASSERT_EQ(3.6, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    matrix_csr_transpose(m);
    for (size_t i = 0; i < matrix_csr_get_row_count(m); i++) {
        for (size_t j = 0; j < matrix_csr_get_col_count(m); j++) {
            double value = matrix_csr_get(m, i, j);
            if (i == 2 && j == 3) {
                ASSERT_EQ(5.1, value);
            } else if (i == 2 && j == 1) {
                ASSERT_EQ(3.6, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    matrix_csr_destroy(m);

    return 0;
}


int test_matrix_002(void) {
    int err;
    MatrixCSR *A = matrix_csr_make(2, 4);
    ASSERT_NOT_NULL(A);
    ASSERT_EQ(2, matrix_csr_get_row_count(A));
    ASSERT_EQ(4, matrix_csr_get_col_count(A));

    ASSERT_NOT(matrix_csr_set(A, 0, 0, 1.0));
    ASSERT_NOT(matrix_csr_set(A, 1, 1, 2.0));
    ASSERT_NOT(matrix_csr_set(A, 0, 2, 3.0));
    ASSERT_NOT(matrix_csr_set(A, 1, 3, 4.0));

    MatrixCSR *B = matrix_csr_make(4, 4);
    ASSERT_NOT_NULL(B);
    ASSERT_EQ(4, matrix_csr_get_row_count(B));
    ASSERT_EQ(4, matrix_csr_get_col_count(B));

    ASSERT_NOT(matrix_csr_set(B, 2, 0, 1.0));
    ASSERT_NOT(matrix_csr_set(B, 1, 1, 2.0));
    ASSERT_NOT(matrix_csr_set(B, 0, 3, 3.0));
    ASSERT_NOT(matrix_csr_set(B, 1, 3, 4.0));
    ASSERT_NOT(matrix_csr_set(B, 2, 3, 5.0));
    ASSERT_NOT(matrix_csr_set(B, 3, 3, 6.0));

    MatrixCSR *C = matrix_csr_prod(A, B);
    ASSERT_NOT_NULL(C);

    for (size_t i = 0; i < matrix_csr_get_row_count(C); i++) {
        for (size_t j = 0; j < matrix_csr_get_col_count(C); j++) {
            double value = matrix_csr_get(C, i, j);
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

    matrix_csr_destroy(C);
    matrix_csr_destroy(B);
    matrix_csr_destroy(A);

    return 0;
}


int test_matrix_003(void) {
    MatrixCSR *m = matrix_csr_make(3, 3);
    ASSERT_NOT_NULL(m);

    ASSERT_NOT(matrix_csr_set(m, 1, 1, 10.5));
    ASSERT_EQ(10.5, matrix_csr_get(m, 1, 1));

    ASSERT_NOT(matrix_csr_set(m, 1, 1, 42.0));
    ASSERT_EQ(42.0, matrix_csr_get(m, 1, 1));

    ASSERT_NOT(matrix_csr_set(m, 0, 0, 0.0));
    ASSERT_EQ(0.0, matrix_csr_get(m, 0, 0));

    ASSERT_NOT(matrix_csr_set(m, 1, 1, 0.0));
    ASSERT_EQ(0.0, matrix_csr_get(m, 1, 1));

    for (size_t i = 0; i < matrix_csr_get_row_count(m); i++) {
        for (size_t j = 0; j < matrix_csr_get_col_count(m); j++) {
            ASSERT_EQ(0.0, matrix_csr_get(m, i, j));
        }
    }

    matrix_csr_destroy(m);
    return 0;
}


int test_matrix_004(void) {
    MatrixCSR *m = matrix_csr_make(2, 5);
    ASSERT_NOT_NULL(m);

    ASSERT_NOT(matrix_csr_set(m, 0, 4, 9.9));
    ASSERT_NOT(matrix_csr_set(m, 1, 2, 8.8));

    matrix_csr_transpose(m);
    ASSERT_EQ(5, matrix_csr_get_row_count(m));
    ASSERT_EQ(2, matrix_csr_get_col_count(m));

    ASSERT_EQ(9.9, matrix_csr_get(m, 4, 0));
    ASSERT_EQ(8.8, matrix_csr_get(m, 2, 1));
    ASSERT_EQ(0.0, matrix_csr_get(m, 0, 4));

    ASSERT_NOT(matrix_csr_set(m, 1, 0, 7.7));

    matrix_csr_transpose(m);
    ASSERT_EQ(2, matrix_csr_get_row_count(m));
    ASSERT_EQ(5, matrix_csr_get_col_count(m));

    ASSERT_EQ(7.7, matrix_csr_get(m, 0, 1));

    matrix_csr_destroy(m);
    return 0;
}


int test_matrix_005(void) {
    MatrixCSR *A = matrix_csr_make(2, 3);
    ASSERT_NOT_NULL(A);
    ASSERT_NOT(matrix_csr_set(A, 0, 0, 1.0));
    ASSERT_NOT(matrix_csr_set(A, 0, 1, 2.0));
    ASSERT_NOT(matrix_csr_set(A, 1, 2, 3.0));

    MatrixCSR *BadDim = matrix_csr_make(4, 2);
    MatrixCSR *C_err = matrix_csr_prod(A, BadDim);
    ASSERT_NULL(C_err);
    matrix_csr_destroy(BadDim);

    MatrixCSR *Zero = matrix_csr_make(3, 2);
    ASSERT_NOT_NULL(Zero);
    MatrixCSR *C_zero = matrix_csr_prod(A, Zero);
    ASSERT_NOT_NULL(C_zero);
    for (size_t i = 0; i < matrix_csr_get_row_count(C_zero); i++) {
        for (size_t j = 0; j < matrix_csr_get_col_count(C_zero); j++) {
            ASSERT_EQ(0.0, matrix_csr_get(C_zero, i, j));
        }
    }

    MatrixCSR *Id = matrix_csr_make(3, 3);
    ASSERT_NOT_NULL(Id);
    ASSERT_NOT(matrix_csr_set(Id, 0, 0, 1.0));
    ASSERT_NOT(matrix_csr_set(Id, 1, 1, 1.0));
    ASSERT_NOT(matrix_csr_set(Id, 2, 2, 1.0));

    MatrixCSR *C_id = matrix_csr_prod(A, Id);
    ASSERT_NOT_NULL(C_id);
    ASSERT_EQ(1.0, matrix_csr_get(C_id, 0, 0));
    ASSERT_EQ(2.0, matrix_csr_get(C_id, 0, 1));
    ASSERT_EQ(3.0, matrix_csr_get(C_id, 1, 2));
    ASSERT_EQ(0.0, matrix_csr_get(C_id, 1, 0));

    matrix_csr_destroy(C_id);
    matrix_csr_destroy(Id);
    matrix_csr_destroy(C_zero);
    matrix_csr_destroy(Zero);
    matrix_csr_destroy(A);

    return 0;
}


void list_int_print(List const *list) {
    printf("%zu: [", list_get_count(list));
    for (size_t i = 0; i< list_get_count(list); i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%d", *(int *)list_atc(list, i));
    }
    printf("]\n");
};


void matrix_csr_print(MatrixCSR const *m) {
    for (size_t i = 0; i < matrix_csr_get_row_count(m); i++) {
        for (size_t j = 0; j < matrix_csr_get_col_count(m); j++) {
            if (j > 0) {
                printf(" ");
            }
            printf("%.2f", matrix_csr_get(m, i, j));
        }
        printf("\n");
    }
};
