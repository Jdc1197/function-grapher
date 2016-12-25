#include "matrix_gen.h"

/* Generates a matrix from a function*/
matrix_t generate_matrix(   double (*f)(double),    /* function to translate x->y*/
                            double min_x,           /* min and max x */ 
                            double max_x,
                            int steps)              /* matrix size*/
{
    matrix_t m = create_matrix(steps);
    for (int i = 0; i < steps; i++)
    {
        m.x[i] = min_x+((max_x-min_x)*((double)i/(steps-1)));
        m.y[i] = f((m.x[i]));
    } 
    return m;
}

matrix_t generate_matrix_parametric(    double (*x)(double),    /* function for x(t) */
                                        double (*y)(double),    /* function for y(t) */
                                        double min_t,           /* min and max t */ 
                                        double max_t,
                                        int steps)              /* matrix size*/
{
    matrix_t m = create_matrix(steps);
    for (int i = 0; i < steps; i++)
    {
        m.x[i] = x(min_t+((max_t-min_t)*((double)i/(steps-1))));
        m.y[i] = y(min_t+((max_t-min_t)*((double)i/(steps-1))));
    } 
    return m;
}
