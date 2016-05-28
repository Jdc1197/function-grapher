#ifndef MATRIX_H
#define MATRIX_H

/* dynamically allocated matrix */
typedef struct {
    double* x;
    double* y;
    double* z; /* coordinates */
    int size;      
} matrix_t;

/* dynamically allocated polar matrix */
typedef struct {
    double* th;
    double* ph;
    double* r; /* polar coordinates (i.e. theta, phi, and radius) */
    int size;
} pmatrix_t;

matrix_t create_matrix(int);
pmatrix_t create_pmatrix(int size);

void free_matrix(matrix_t*);
void free_pmatrix(pmatrix_t*);

pmatrix_t convert_to_pmatrix(matrix_t, double, double, double);
matrix_t convert_to_matrix(pmatrix_t);

void shift_pmatrix(pmatrix_t*, double, double);
#endif
