#ifndef MATRIX_H
#define MATRIX_H

/* dynamically allocated matrix */
typedef struct {
    double* x;
    double* y;
    int size;      
} matrix_t;

matrix_t create_matrix(int);

void free_matrix(matrix_t*);
#endif
