#ifndef MATRIX_CSR_H
#define MATRIX_CSR_H


#include <stddef.h>


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
typedef int CSRErr;
enum {
    MATRIX_CSR__OK = 0,
    MATRIX_CSR__ALLOC = 1,
    MATRIX_CSR__UNKNOWN = 7,
};

MatrixCSR * matrix_csr_make(size_t, size_t);
void matrix_csr_destroy(MatrixCSR *);


size_t matrix_csr_get_col_count(MatrixCSR const *);
size_t matrix_csr_get_row_count(MatrixCSR const *);
MATRIX_CSR_SCALAR_T matrix_csr_get(MatrixCSR const *, size_t, size_t);
CSRErr matrix_csr_set(MatrixCSR *, size_t, size_t, MATRIX_CSR_SCALAR_T);
void matrix_csr_transpose(MatrixCSR *);
MatrixCSR * matrix_csr_prod(MatrixCSR const *, MatrixCSR const *);


#endif
