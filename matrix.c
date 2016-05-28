#include <stdlib.h>
#include <math.h>
#include "matrix.h"

matrix_t create_matrix(int size)
{
    matrix_t m;
    m.x = (double*)calloc(size, sizeof(double*));
    m.y = (double*)calloc(size, sizeof(double*));
    m.z = (double*)calloc(size, sizeof(double*));
    m.size = size;
    return m;
}

pmatrix_t create_pmatrix(int size)
{
    pmatrix_t m;
    m.th = (double*)calloc(size, sizeof(double*));
    m.ph = (double*)calloc(size, sizeof(double*));
    m.r = (double*)calloc(size, sizeof(double*));
    m.size = size;
    return m;
}

void free_matrix(matrix_t* m)
{
    free(m->x);
    free(m->y);
    free(m->z);
    m->size = 0; 
}

void free_pmatrix(pmatrix_t* pm)
{
    free(pm->ph);
    free(pm->th);
    free(pm->r);
    pm->size = 0; 
}

pmatrix_t convert_to_pmatrix(matrix_t m, double ox, double oy, double oz)
{
    pmatrix_t pm = create_pmatrix(m.size);
    for (int i = 0; i < pm.size; i++)
    {
        pm.r[i] = pow(pow((m.x[i]),2)+pow((m.y[i]),2)+pow((m.z[i]),2),0.5);
        /* solves domain error for acos */
        if (pm.r[i] == 0)
        {
            pm.th[i] = 0;
            pm.ph[i] = 0;
        }
        else
        {
            pm.th[i] = acos((m.z[i])/pm.r[i]);
            pm.ph[i] = atan2((m.y[i]),(m.x[i]));
        }
    }
    return pm;
}

matrix_t convert_to_matrix(pmatrix_t pm)
{
    matrix_t m = create_matrix(pm.size);
    for (int i = 0; i < m.size; i++)
    {
        m.x[i] = pm.r[i]*sin(pm.th[i])*cos(pm.ph[i]);
        m.y[i] = pm.r[i]*sin(pm.th[i])*sin(pm.ph[i]);
        m.z[i] = pm.r[i]*cos(pm.th[i]);
    }
    return m;
}

void shift_pmatrix(pmatrix_t* pm,double th, double ph)
{
    for (int i = 0; i < pm->size; i++)
    {
        pm->th[i] += th;
        pm->ph[i] += ph;
    }
}
