#ifndef MATRIX_GEN_H
#define MATRIX_GEN_H

#include "matrix.h"

matrix_t generate_matrix(double (*f)(double),double, double, int);
/* parametric */
matrix_t generate_matrix_parametric(double (*f)(double), double (*g)(double),double, double, int);


#endif
