#ifndef SPM_MATRIX_H
#define SPM_MATRIX_H


#include <stddef.h>
#include "./error.h"


#ifndef SPM_SCALAR_T
#define SPM_SCALAR_T double
#endif


#ifndef SPM_SCALAR_ZERO
#define SPM_SCALAR_ZERO 0.0
#endif


#ifndef SPM_SCALAR_EQ
#define SPM_SCALAR_EQ(a, b) (a == b)
#endif


#ifndef SPM_SCALAR_IS_ZERO
#define SPM_SCALAR_IS_ZERO(a) SPM_SCALAR_EQ(a, SPM_SCALAR_ZERO)
#endif


#ifndef SPM_SCALAR_SUM
#define SPM_SCALAR_SUM(a, b) (a+b)
#endif


#ifndef SPM_SCALAR_PROD
#define SPM_SCALAR_PROD(a, b) (a*b)
#endif


typedef struct SpmMatrix SpmMatrix;


SpmMatrix * spm_matrix_make(size_t row_count, size_t col_count);
SpmMatrix * spm_matrix_clone(SpmMatrix const *src);
void spm_matrix_destroy(SpmMatrix *m);


size_t spm_matrix_get_col_count(SpmMatrix const *m);
size_t spm_matrix_get_row_count(SpmMatrix const *m);
SPM_SCALAR_T spm_matrix_get(SpmMatrix const *m, size_t row, size_t col);
SpmErr spm_matrix_set(
    SpmMatrix *m,
    size_t row,
    size_t col,
    SPM_SCALAR_T value
);
void spm_matrix_transpose(SpmMatrix *m);
SpmMatrix * spm_matrix_prod(SpmMatrix const *left, SpmMatrix const *right);


#endif
