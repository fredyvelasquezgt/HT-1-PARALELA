//Fredy Velasquez 201011
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

// Definición de las funciones
double fun_cuadrada(double x) {
    return x * x;
}

double fun_cubo(double x) {
    return 2 * pow(x, 3);
}

double fun_seno(double x) {
    return sin(x);
}

// Función para calcular la integral numérica usando el método del trapecio
void trapezoide(int tipo, double a, double b, int n, int threads) {
    double (*f)(double);
    switch (tipo) {
        case 1:
            f = fun_cuadrada;
            break;
        case 2:
            f = fun_cubo;
            break;
        case 3:
            f = fun_seno;
            break;
        default:
            printf("Tipo de función inválido.\n");
            return;
    }
    
    double h = (b - a) / n;
    double resultado = 0.0;

    #pragma omp parallel num_threads(threads)
    {
        int thread_num = omp_get_thread_num();
        double A_threaded = a + thread_num * (n / threads) * h;
        double B_threaded = A_threaded + (n / threads) * h;
        double pool_threads = (f(A_threaded) + f(B_threaded)) / 2.0;

        #pragma omp single
        {
            for (int i = 1; i < (n / threads); i++) {
                double x = A_threaded + i * h;
                pool_threads += f(x);
            }

            pool_threads *= h;

            #pragma omp atomic
            resultado += pool_threads;
        }
    }

    printf("Resultado de la integral:\n");
    printf("  Para n = %d trapezoides, aproximación de la integral en el intervalo (%f, %f) es %f\n", n, a, b, resultado);
}

int main(int argc, char* argv[]) {
    if (argc <= 3) {
        printf(" Falta de parámetros correctos.\n");
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int n = 1000000; 
    int threads = atoi(argv[3]);
    
    if (n % threads) {
        printf("  La cantidad de trapezoides  debe ser múltiplo de los threads ingresados.\n");
        return 1;
    }

    trapezoide(3, a, b, n, threads);

    return 0;
}

