
#include "definiciones.h"   // Contiene dT, ParametrosSIMS, etc.
// Para compilar: gcc -o main_sims_fig4 main_sims_fig4.c rk4_sims.c -lm -std=c99 -Wall
// Para ejecutar: ./main_sims_fig4.exe

int main() {
    char *archivo_red, *archivo_salida;
    int nodo_inicial, N, max_iter, guardar_cada;
    double t_max, t;
    double *A_uni, *X_uni, *rho, *mu;
    ParametrosSIMS pa;
    FILE *f_tmp, *salida;
    
    archivo_red    = "BWetal2022_LCC.edge";
    nodo_inicial     = 0;
    archivo_salida = "resultados_fig4.txt";
    t_max = 1000.0;   // 1000 como en la fig4

    // Parámetros fijos de fig4
    pa.beta  = 0.3;
    pa.mu_0  = 0.1;
    pa.Dx    = 1.0e-5;
    pa.alfa  = 0.03;
    pa.delta = 3.0;
    pa.gamma = 0.0;      // inmunidad duradera
    // pa.N se fijará al leer la red

    // --- Leer N del archivo de red para asignar memoria ---
    f_tmp = fopen(archivo_red, "r");
    if (!f_tmp) {
        printf("Error al abrir %s\n", archivo_red);
        return 1;
    }
    fscanf(f_tmp, "%d", &N);
    fclose(f_tmp);
    pa.N = N;

    printf("Red: %s, N=%d, nodo inicial=%d, gamma=%.2f, t_max=%.0f\n",
           archivo_red, N, nodo_inicial, pa.gamma, t_max);

    // --- Reservar memoria para matrices y vectores ---
    A_uni = (double *)malloc(N * N * sizeof(double));
    X_uni = (double *)malloc(N * N * sizeof(double));
    rho   = (double *)malloc(N * sizeof(double));
    mu    = (double *)malloc(N * sizeof(double));
    if (!A_uni || !X_uni || !rho || !mu) {
        printf("Error de memoria\n");
        return 1;
    }

    cargar_red(archivo_red, N, A_uni, X_uni);

    // Condiciones iniciales: nodo mas viejo infectado
    for (int i = 0; i < N; i++) {
        rho[i] = 0.0;
        mu[i]  = pa.mu_0;
    }
    rho[nodo_inicial] = 0.01;

    salida = fopen(archivo_salida, "w");
    if (!salida) {
        printf("Error creando %s\n", archivo_salida);
        return 1;
    }

    // Simulación
    t = 0.0;
    max_iter = (int)(t_max / dT);
    guardar_cada = 10;   // guardar cada 10 pasos (cada 0.1 días) para no generar archivos enormes

    printf("Simulando...\n");
    for (int iter = 0; iter < max_iter; iter++) {
        // Escribir cada 'guardar_cada' pasos
        if (iter % guardar_cada == 0) {
            fprintf(salida, "%.6f", t);
            for (int i = 0; i < N; i++) {
                fprintf(salida, "\t%.6f", rho[i]);
            }
            fprintf(salida, "\n");
        }

        // Avanzar un paso RK4
        paso_rk4_sims(rho, mu, X_uni, A_uni, pa);

        t += dT;
    }

    fclose(salida);
    free(A_uni);
    free(X_uni);
    free(rho);
    free(mu);

    printf("Simulación terminada. Resultados en %s\n", archivo_salida);
    return 0;
}