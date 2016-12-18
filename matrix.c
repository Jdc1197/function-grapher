#include <stdlib.h>
#include <math.h>
#include "matrix.h"

matrix_t create_matrix(int size)
{
    matrix_t m;
    m.x = (double*)calloc(size, sizeof(double*));
    m.y = (double*)calloc(size, sizeof(double*));
    m.size = size;
    return m;
}

void free_matrix(matrix_t* m)
{
    free(m->x);
    free(m->y);
    m->size = 0; 
}
