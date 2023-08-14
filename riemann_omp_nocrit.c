#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double calculate_square(double x) {
    return x * x;
}

double calculate_cubic(double x) {
    return 2 * pow(x, 3);
}

double calculate_sine(double x) {
    return sin(x);
}

double *partial_results;

void integrate_trapezoid(int function_type, double a, double b, int n, int threads) {
    double (*f)(double);
    switch (function_type) {
        case 1: f = calculate_square; break;
        case 2: f = calculate_cubic; break;
        case 3: f = calculate_sine; break;
        default: printf("Tipo de función inválido.\n"); return;
    }
    
    double h = (b - a) / n;
    partial_results = (double *)malloc(threads * sizeof(double));

    #pragma omp parallel num_threads(threads)
    {
        int thread_num = omp_get_thread_num();
        double A_threaded = a + thread_num * (n / threads) * h;
        double pool_threads = (f(A_threaded) + f(A_threaded + (n / threads) * h)) / 2.0;

        for (int i = 1; i < (n / threads); i++) {
            double x = A_threaded + i * h;
            pool_threads += f(x);
        }

        pool_threads *= h;

        partial_results[thread_num] = pool_threads;
    }

    double resultado = 0.0;
    for (int i = 0; i < threads; i++) {
        resultado += partial_results[i];
    }

    printf("Resultado de la integral con n = %d trapezoides, integral de (%f, %f) es %f\n", n, a, b, resultado);
    free(partial_results);
}

int main(int argc, char* argv[]) {
    if (argc <= 3) {
        printf("Faltan parámetros.\n");
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int n = 1000000; 
    int threads = atoi(argv[3]);

    if (n % threads) {
        printf(" La cantidad de trapezoides debe ser múltiplo de threads ingresados.\n");
        return 1;
    }

    integrate_trapezoid(1, a, b, n, threads);
    integrate_trapezoid(2, a, b, n, threads);
    integrate_trapezoid(3, a, b, n, threads);

    return 0;
}

