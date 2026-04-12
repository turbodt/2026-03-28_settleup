#include <spm.h>
#include <test-shared.h>


SPM_MATRIX_HEADER(double, F64, f64);


static int test_001(void) {
    int err;
    SpmMatrixF64 *m = spm_matrix_f64_make(4, 4);
    ASSERT_NOT_NULL(m);
    ASSERT_EQ(4, spm_matrix_f64_get_row_count(m));
    ASSERT_EQ(4, spm_matrix_f64_get_col_count(m));

    err = spm_matrix_f64_set(m, 2, 3, 5.1);
    ASSERT_FALSE(err);
    err = spm_matrix_f64_set(m, 2, 1, 3.6);
    ASSERT_FALSE(err);

    for (size_t i = 0; i < spm_matrix_f64_get_row_count(m); i++) {
        for (size_t j = 0; j < spm_matrix_f64_get_col_count(m); j++) {
            double value = spm_matrix_f64_get(m, i, j);
            if (i == 2 && j == 3) {
                ASSERT_EQ(5.1, value);
            } else if (i == 2 && j == 1) {
                ASSERT_EQ(3.6, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    spm_matrix_f64_transpose(m);
    for (size_t i = 0; i < spm_matrix_f64_get_row_count(m); i++) {
        for (size_t j = 0; j < spm_matrix_f64_get_col_count(m); j++) {
            double value = spm_matrix_f64_get(m, i, j);
            if (i == 3 && j == 2) {
                ASSERT_EQ(5.1, value);
            } else if (i == 1 && j == 2) {
                ASSERT_EQ(3.6, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    spm_matrix_f64_transpose(m);
    for (size_t i = 0; i < spm_matrix_f64_get_row_count(m); i++) {
        for (size_t j = 0; j < spm_matrix_f64_get_col_count(m); j++) {
            double value = spm_matrix_f64_get(m, i, j);
            if (i == 2 && j == 3) {
                ASSERT_EQ(5.1, value);
            } else if (i == 2 && j == 1) {
                ASSERT_EQ(3.6, value);
            } else {
                ASSERT_EQ(0.0, value);
            }
        }
    }

    spm_matrix_f64_destroy(m);

    return 0;
}


static int test_002(void) {
    SpmMatrixF64 *A = spm_matrix_f64_make(2, 4);
    ASSERT_NOT_NULL(A);
    ASSERT_EQ(2, spm_matrix_f64_get_row_count(A));
    ASSERT_EQ(4, spm_matrix_f64_get_col_count(A));

    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 1, 1, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 2, 3.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 1, 3, 4.0));

    SpmMatrixF64 *B = spm_matrix_f64_make(4, 4);
    ASSERT_NOT_NULL(B);
    ASSERT_EQ(4, spm_matrix_f64_get_row_count(B));
    ASSERT_EQ(4, spm_matrix_f64_get_col_count(B));

    ASSERT_FALSE(spm_matrix_f64_set(B, 2, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 1, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 0, 3, 3.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 3, 4.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 2, 3, 5.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 3, 3, 6.0));

    SpmMatrixF64 *C = spm_matrix_f64_prod(A, B);
    ASSERT_NOT_NULL(C);

    for (size_t i = 0; i < spm_matrix_f64_get_row_count(C); i++) {
        for (size_t j = 0; j < spm_matrix_f64_get_col_count(C); j++) {
            double value = spm_matrix_f64_get(C, i, j);
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

    spm_matrix_f64_destroy(C);
    spm_matrix_f64_destroy(B);
    spm_matrix_f64_destroy(A);

    return 0;
}


static int test_003(void) {
    SpmMatrixF64 *m = spm_matrix_f64_make(3, 3);
    ASSERT_NOT_NULL(m);

    ASSERT_FALSE(spm_matrix_f64_set(m, 1, 1, 10.5));
    ASSERT_EQ(10.5, spm_matrix_f64_get(m, 1, 1));

    ASSERT_FALSE(spm_matrix_f64_set(m, 1, 1, 42.0));
    ASSERT_EQ(42.0, spm_matrix_f64_get(m, 1, 1));

    ASSERT_FALSE(spm_matrix_f64_set(m, 0, 0, 0.0));
    ASSERT_EQ(0.0, spm_matrix_f64_get(m, 0, 0));

    ASSERT_FALSE(spm_matrix_f64_set(m, 1, 1, 0.0));
    ASSERT_EQ(0.0, spm_matrix_f64_get(m, 1, 1));

    for (size_t i = 0; i < spm_matrix_f64_get_row_count(m); i++) {
        for (size_t j = 0; j < spm_matrix_f64_get_col_count(m); j++) {
            ASSERT_EQ(0.0, spm_matrix_f64_get(m, i, j));
        }
    }

    spm_matrix_f64_destroy(m);
    return 0;
}


static int test_004(void) {
    SpmMatrixF64 *m = spm_matrix_f64_make(2, 5);
    ASSERT_NOT_NULL(m);

    ASSERT_FALSE(spm_matrix_f64_set(m, 0, 4, 9.9));
    ASSERT_FALSE(spm_matrix_f64_set(m, 1, 2, 8.8));

    spm_matrix_f64_transpose(m);
    ASSERT_EQ(5, spm_matrix_f64_get_row_count(m));
    ASSERT_EQ(2, spm_matrix_f64_get_col_count(m));

    ASSERT_EQ(9.9, spm_matrix_f64_get(m, 4, 0));
    ASSERT_EQ(8.8, spm_matrix_f64_get(m, 2, 1));
    ASSERT_EQ(0.0, spm_matrix_f64_get(m, 0, 4));

    ASSERT_FALSE(spm_matrix_f64_set(m, 1, 0, 7.7));

    spm_matrix_f64_transpose(m);
    ASSERT_EQ(2, spm_matrix_f64_get_row_count(m));
    ASSERT_EQ(5, spm_matrix_f64_get_col_count(m));

    ASSERT_EQ(7.7, spm_matrix_f64_get(m, 0, 1));

    spm_matrix_f64_destroy(m);
    return 0;
}


static int test_005(void) {
    SpmMatrixF64 *A = spm_matrix_f64_make(2, 3);
    ASSERT_NOT_NULL(A);
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 1, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 1, 2, 3.0));

    SpmMatrixF64 *BadDim = spm_matrix_f64_make(4, 2);
    SpmMatrixF64 *C_err = spm_matrix_f64_prod(A, BadDim);
    ASSERT_NULL(C_err);
    spm_matrix_f64_destroy(BadDim);

    SpmMatrixF64 *Zero = spm_matrix_f64_make(3, 2);
    ASSERT_NOT_NULL(Zero);
    SpmMatrixF64 *C_zero = spm_matrix_f64_prod(A, Zero);
    ASSERT_NOT_NULL(C_zero);
    for (size_t i = 0; i < spm_matrix_f64_get_row_count(C_zero); i++) {
        for (size_t j = 0; j < spm_matrix_f64_get_col_count(C_zero); j++) {
            ASSERT_EQ(0.0, spm_matrix_f64_get(C_zero, i, j));
        }
    }

    SpmMatrixF64 *Id = spm_matrix_f64_make(3, 3);
    ASSERT_NOT_NULL(Id);
    ASSERT_FALSE(spm_matrix_f64_set(Id, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(Id, 1, 1, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(Id, 2, 2, 1.0));

    SpmMatrixF64 *C_id = spm_matrix_f64_prod(A, Id);
    ASSERT_NOT_NULL(C_id);
    ASSERT_EQ(1.0, spm_matrix_f64_get(C_id, 0, 0));
    ASSERT_EQ(2.0, spm_matrix_f64_get(C_id, 0, 1));
    ASSERT_EQ(3.0, spm_matrix_f64_get(C_id, 1, 2));
    ASSERT_EQ(0.0, spm_matrix_f64_get(C_id, 1, 0));

    spm_matrix_f64_destroy(C_id);
    spm_matrix_f64_destroy(Id);
    spm_matrix_f64_destroy(C_zero);
    spm_matrix_f64_destroy(Zero);
    spm_matrix_f64_destroy(A);

    return 0;
}


static int test_006(void) {
    SpmMatrixF64 *m = spm_matrix_f64_make(3, 3);
    ASSERT_NOT_NULL(m);

    ASSERT_EQ(SPM_ERR__OUT_INDEX, spm_matrix_f64_set(m, 3, 0, 1.0));
    ASSERT_EQ(SPM_ERR__OUT_INDEX, spm_matrix_f64_set(m, 100, 0, 1.0));

    ASSERT_EQ(SPM_ERR__OUT_INDEX, spm_matrix_f64_set(m, 0, 3, 1.0));

    ASSERT_EQ(0.0, spm_matrix_f64_get(m, 3, 3));
    ASSERT_EQ(0.0, spm_matrix_f64_get(m, 0, 10));

    spm_matrix_f64_transpose(m);

    SpmMatrixF64 *m2 = spm_matrix_f64_make(2, 4);
    spm_matrix_f64_transpose(m2);

    ASSERT_FALSE(spm_matrix_f64_set(m2, 3, 1, 5.0));
    ASSERT_EQ(SPM_ERR__OUT_INDEX, spm_matrix_f64_set(m2, 1, 3, 5.0));

    spm_matrix_f64_destroy(m);
    spm_matrix_f64_destroy(m2);
    return 0;
}


static int test_007(void) {
    SpmMatrixF64 *m = spm_matrix_f64_make(3, 3);

    ASSERT_FALSE(spm_matrix_f64_set(m, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(m, 1, 1, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(m, 2, 2, 3.0));

    ASSERT_FALSE(spm_matrix_f64_set(m, 1, 1, 0.0));
    ASSERT_EQ(0.0, spm_matrix_f64_get(m, 1, 1));

    ASSERT_FALSE(spm_matrix_f64_set(m, 0, 1, 0.0));

    spm_matrix_f64_destroy(m);
    return 0;
}

static int test_008(void) {
    SpmMatrixF64 *A = spm_matrix_f64_make(2, 3);
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 1, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 1, 2, 3.0));

    SpmMatrixF64 *B = spm_matrix_f64_make(2, 3);
    ASSERT_FALSE(spm_matrix_f64_set(B, 0, 0, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 0, 1, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 1, 3.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 2, 1.0));
    spm_matrix_f64_transpose(B);

    SpmMatrixF64 *C = spm_matrix_f64_prod(A, B);
    ASSERT_NOT_NULL(C);

    ASSERT_EQ(4.0, spm_matrix_f64_get(C, 0, 0));

    spm_matrix_f64_destroy(C);
    spm_matrix_f64_destroy(B);
    spm_matrix_f64_destroy(A);
    return 0;
}


static int test_009(void) {
    // A_orig = [1.0, 2.0]
    //          [0.0, 0.0]
    //          [0.0, 0.0]
    SpmMatrixF64 *A = spm_matrix_f64_make(3, 2);
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 1, 2.0));
    spm_matrix_f64_transpose(A);

    // B_orig = [1.0, 0.0]
    //          [3.0, 0.0]
    //          [0.0, 0.0]
    SpmMatrixF64 *B = spm_matrix_f64_make(3, 2);
    ASSERT_FALSE(spm_matrix_f64_set(B, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 0, 3.0));

    SpmMatrixF64 *C = spm_matrix_f64_prod(A, B);
    ASSERT_NOT_NULL(C);
    ASSERT_EQ(2, spm_matrix_f64_get_row_count(C));
    ASSERT_EQ(2, spm_matrix_f64_get_col_count(C));

    for (size_t row = 0; row < 2; row++) {
        for (size_t col = 0; col < 2; col++) {
            if (row == 0 && col == 0) {
                ASSERT_EQ(1.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 1 && col == 0) {
                ASSERT_EQ(2.0, spm_matrix_f64_get(C, row, col));
            } else {
                ASSERT_EQ(0.0, spm_matrix_f64_get(C, row, col));
            }
        }
    }

    spm_matrix_f64_destroy(C);
    spm_matrix_f64_destroy(B);
    spm_matrix_f64_destroy(A);
    return 0;
}


static int test_010(void) {
    // A_orig = [1.0, 2.0, 0.0]
    //          [0.0, 3.0, 4.0]
    SpmMatrixF64 *A = spm_matrix_f64_make(2, 3);
    ASSERT_NOT_NULL(A);
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 1, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 1, 1, 3.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 1, 2, 4.0));
    spm_matrix_f64_transpose(A);

    // B_orig = [1.0, 0.0]
    //          [2.0, 0.0]
    //          [0.0, 1.0]
    //          [0.0, 3.0]
    SpmMatrixF64 *B = spm_matrix_f64_make(4, 2);
    ASSERT_NOT_NULL(B);
    ASSERT_FALSE(spm_matrix_f64_set(B, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 0, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 2, 1, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 3, 1, 3.0));
    spm_matrix_f64_transpose(B);

    SpmMatrixF64 *C = spm_matrix_f64_prod(A, B);
    ASSERT_NOT_NULL(C);
    ASSERT_EQ(3, spm_matrix_f64_get_row_count(C));
    ASSERT_EQ(4, spm_matrix_f64_get_col_count(C));

    for (size_t row = 0; row < 3; row++) {
        for (size_t col = 0; col < 4; col++) {
            if (row == 0 && col == 0) {
                ASSERT_EQ(1.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 0 && col == 1) {
                ASSERT_EQ(2.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 1 && col == 0) {
                ASSERT_EQ(2.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 1 && col == 1) {
                ASSERT_EQ(4.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 1 && col == 2) {
                ASSERT_EQ(3.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 1 && col == 3) {
                ASSERT_EQ(9.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 2 && col == 2) {
                ASSERT_EQ(4.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 2 && col == 3) {
                ASSERT_EQ(12.0, spm_matrix_f64_get(C, row, col));
            } else {
                ASSERT_EQ(0.0, spm_matrix_f64_get(C, row, col));
            }
        }
    }

    spm_matrix_f64_destroy(C);
    spm_matrix_f64_destroy(B);
    spm_matrix_f64_destroy(A);
    return 0;
}

static int test_011(void) {
    // A_orig = [1.0, 2.0]
    //          [0.0, 3.0]
    //          [4.0, 0.0]
    SpmMatrixF64 *A = spm_matrix_f64_make(3, 2);
    ASSERT_NOT_NULL(A);
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 1, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 1, 1, 3.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 2, 0, 4.0));
    spm_matrix_f64_transpose(A); // Ara A és lògicament 2x3

    // B_orig = [1.0, 0.0]
    //          [2.0, 1.0]
    //          [0.0, 3.0]
    SpmMatrixF64 *B = spm_matrix_f64_make(3, 2);
    ASSERT_NOT_NULL(B);
    ASSERT_FALSE(spm_matrix_f64_set(B, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 0, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 1, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 2, 1, 3.0));

    SpmMatrixF64 *C = spm_matrix_f64_prod(A, B);
    ASSERT_NOT_NULL(C);
    ASSERT_EQ(2, spm_matrix_f64_get_row_count(C));
    ASSERT_EQ(2, spm_matrix_f64_get_col_count(C));

    for (size_t row = 0; row < 2; row++) {
        for (size_t col = 0; col < 2; col++) {
            if (row == 0 && col == 0) {
                ASSERT_EQ(1.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 0 && col == 1) {
                ASSERT_EQ(12.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 1 && col == 0) {
                ASSERT_EQ(8.0, spm_matrix_f64_get(C, row, col));
            } else if (row == 1 && col == 1) {
                ASSERT_EQ(3.0, spm_matrix_f64_get(C, row, col));
            } else {
                ASSERT_EQ(0.0, spm_matrix_f64_get(C, row, col));
            }
        }
    }

    spm_matrix_f64_destroy(C);
    spm_matrix_f64_destroy(B);
    spm_matrix_f64_destroy(A);
    return 0;
}


static int test_012(void) {
    SpmMatrixF64 *A = spm_matrix_f64_make(2, 3);
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 0, 1, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 1, 1, 3.0));
    ASSERT_FALSE(spm_matrix_f64_set(A, 1, 2, 4.0));

    SpmMatrixF64 *B = spm_matrix_f64_make(3, 2);
    ASSERT_FALSE(spm_matrix_f64_set(B, 0, 0, 2.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 0, 1.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 1, 1, 3.0));
    ASSERT_FALSE(spm_matrix_f64_set(B, 2, 1, 1.0));

    SpmMatrixF64 *C = spm_matrix_f64_prod(A, B);
    ASSERT_NOT_NULL(C);
    ASSERT_EQ(2, spm_matrix_f64_get_row_count(C));
    ASSERT_EQ(2, spm_matrix_f64_get_col_count(C));

    ASSERT_EQ(4.0, spm_matrix_f64_get(C, 0, 0));
    ASSERT_EQ(6.0, spm_matrix_f64_get(C, 0, 1));
    ASSERT_EQ(3.0, spm_matrix_f64_get(C, 1, 0));
    ASSERT_EQ(13.0, spm_matrix_f64_get(C, 1, 1));

    spm_matrix_f64_destroy(C);
    spm_matrix_f64_destroy(B);
    spm_matrix_f64_destroy(A);
    return 0;
}



static Test tests[] = {
    {.name="Matrix F64 001", .call=&test_001},
    {.name="Matrix F64 002", .call=&test_002},
    {.name="Matrix F64 003", .call=&test_003},
    {.name="Matrix F64 004", .call=&test_004},
    {.name="Matrix F64 005", .call=&test_005},
    {.name="Matrix F64 006", .call=&test_006},
    {.name="Matrix F64 007", .call=&test_007},
    {.name="Matrix F64 008", .call=&test_008},
    {.name="Matrix F64 009", .call=&test_009},
    {.name="Matrix F64 010", .call=&test_010},
    {.name="Matrix F64 011", .call=&test_011},
    {.name="Matrix F64 012", .call=&test_012},
    {.name="", .call=NULL},
};


static TestSuite test_suite = (TestSuite) {
    .name = "Matrix: Basic operations with double (f64)",
    .tests = tests,
};


TestSuite const * test_suite_matrix_get_001(void) {
    return &test_suite;
};
