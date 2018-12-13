#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <omp.h>

#define LENGTH 1000000000LL

typedef struct {
    int64_t len;
    double *data;
} my_vector;

int64_t my_vector_len(my_vector *v) {
    return v->len;
}

void multiply(my_vector *v, double *res)
{
    int64_t i;
    double tmp_res = 1;
    #pragma omp parallel for reduction(* : tmp_res)
    for (i = 0; i < my_vector_len(v); i++) {
        tmp_res *= v->data[i];
    }
    *res = tmp_res;
}

double a[LENGTH];
my_vector v = {LENGTH, a};

int main() 
{
    omp_set_num_threads(2);
    double res;
    srand(time(NULL));
    for (int64_t i = 0; i < v.len; ++i) 
    {
        v.data[i] = 1.0 + ((double)rand() - 0.5) / RAND_MAX / 1e9;
    }

    double start, end;
    start = omp_get_wtime();
    multiply(&v, &res);
    end = omp_get_wtime();

    printf("Result - %f\n", res);
    printf("Time - %f sec\n", (double)(end - start));
}
