#ifndef SPM_MATRIX_H
#define SPM_MATRIX_H


#include <stddef.h>
#include "./error.h"


#ifndef MATRIX_CSR_SCALAR_T
#define MATRIX_CSR_SCALAR_T double
#endif


#ifndef MATRIX_CSR_SCALAR_ZERO
#define MATRIX_CSR_SCALAR_ZERO 0.0
#endif


#ifndef MATRIX_CSR_SCALAR_EQ
#define MATRIX_CSR_SCALAR_EQ(a, b) (a == b)
#endif


#ifndef MATRIX_CSR_SCALAR_IS_ZERO
#define MATRIX_CSR_SCALAR_IS_ZERO(a) MATRIX_CSR_SCALAR_EQ(a, MATRIX_CSR_SCALAR_ZERO)
#endif


#ifndef MATRIX_CSR_SCALAR_SUM
#define MATRIX_CSR_SCALAR_SUM(a, b) (a+b)
#endif


#ifndef MATRIX_CSR_SCALAR_PROD
#define MATRIX_CSR_SCALAR_PROD(a, b) (a*b)
#endif


typedef struct MatrixCSR MatrixCSR;


MatrixCSR * matrix_csr_make(size_t row_count, size_t col_count);
MatrixCSR * matrix_csr_clone(MatrixCSR const *src);
void matrix_csr_destroy(MatrixCSR *m);


size_t matrix_csr_get_col_count(MatrixCSR const *m);
size_t matrix_csr_get_row_count(MatrixCSR const *m);
MATRIX_CSR_SCALAR_T matrix_csr_get(MatrixCSR const *m, size_t row, size_t col);
SpmErr matrix_csr_set(
    MatrixCSR *m,
    size_t row,
    size_t col,
    MATRIX_CSR_SCALAR_T value
);
void matrix_csr_transpose(MatrixCSR *m);
MatrixCSR * matrix_csr_prod(MatrixCSR const *left, MatrixCSR const *right);


#endif
