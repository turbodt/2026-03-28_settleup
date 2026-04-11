#include "./list.h"
#include "./matrix.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define SPM_F64_IS_ZERO(a) ((a) == 0.0)
#define SPM_F64_SUM(a, b) ((a) + (b))
#define SPM_F64_PROD(a, b) ((a) * (b))


SPM_MATRIX_HEADER(double, F64, f64);
SPM_MATRIX_IMPLEMENTATION(double, 0.0, SPM_F64_IS_ZERO, SPM_F64_SUM, SPM_F64_PROD, F64, f64);
