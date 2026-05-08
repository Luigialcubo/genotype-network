
#include "definiciones.h"   // Contiene dT, ParametrosSIMS, etc.
// Para compilar: gcc -o main_sims_fig4 main_sims_fig4.c rk4_sims.c -lm -std=c99 -Wall
// Para ejecutar: ./main_sims_fig4.exe

int main() {
    const char *archivos_red[3], *archivos_salida[3], *archivo_red, *archivo_salida;
    int nodo_inicial, c, N, max_iter, guardar_cada, i, iter;
    double t_max, *A_uni, *X_uni, *rho, *mu, t;
    FILE *f_tmp, *salida;
    
    ParametrosSIMS pa;     // Parámetros fig4
    pa.beta  = 0.3;
    pa.mu_0  = 0.1;
    pa.Dx    = 1.0e-5;
    pa.alfa  = 0.03;
    pa.delta = 3.0;
    pa.gamma = 0.0;     // inmunidad duradera

    archivos_red[0] = "componentes_conexas/componente_1.txt";
    archivos_red[1] = "componentes_conexas/componente_2.txt";
    archivos_red[2] = "componentes_conexas/componente_7.txt";
    archivos_salida[0] = "results/fig4/sims_comp1.txt";
    archivos_salida[1] = "results/fig4/sims_comp2.txt";
    archivos_salida[2] = "results/fig4/sims_comp7.txt";
    
    nodo_inicial = 0;     // nodo más viejo infectado
    t_max = 1000.0;
    guardar_cada = 10;

    // Procesar cada componente
    for (c = 0; c < 3; c++) {
        archivo_red    = archivos_red[c];
        archivo_salida = archivos_salida[c];

        // Leer N
        f_tmp = fopen(archivo_red, "r");
        if (!f_tmp) {
            printf("Error al abrir %s\n", archivo_red);
            continue;
        }
        fscanf(f_tmp, "%d", &N);
        fclose(f_tmp);
        pa.N = N;

        printf("Red: %s, N=%d, nodo inicial=%d, gamma=%.2f, t_max=%.0f\n",
               archivo_red, N, nodo_inicial, pa.gamma, t_max);

        // Reservar memoria
        A_uni = (double *)malloc(N * N * sizeof(double));
        X_uni = (double *)malloc(N * N * sizeof(double));
        rho   = (double *)malloc(N * sizeof(double));
        mu    = (double *)malloc(N * sizeof(double));
        if (!A_uni || !X_uni || !rho || !mu) {
            printf("Error de memoria\n");
            free(A_uni); free(X_uni); free(rho); free(mu);
            continue;
        }

        // Cargar red (A y X)
        cargar_red(archivo_red, N, A_uni, X_uni);

        // Condiciones iniciales :nodo mas viejo infectado
        for (i = 0; i < N; i++) {
            rho[i] = 0.0;
            mu[i]  = pa.mu_0;
        }
        rho[nodo_inicial] = 0.01;

        // Abrir archivo de salida
        salida = fopen(archivo_salida, "w");
        if (!salida) {
            printf("Error creando %s\n", archivo_salida);
            free(A_uni); free(X_uni); free(rho); free(mu);
            continue;
        }

        // Simulación
        t = 0.0;
        max_iter = (int)(t_max / dT);
        guardar_cada = 10;

        printf("Simulando...\n");
        for (iter = 0; iter < max_iter; iter++) {
            if (iter % guardar_cada == 0) {
                fprintf(salida, "%.6f", t);
                for (i = 0; i < N; i++) {
                    fprintf(salida, "\t%.6f", rho[i]);
                }
                fprintf(salida, "\n");
            }
            paso_rk4_sims(rho, mu, X_uni, A_uni, pa);
            t += dT;
        }

        fclose(salida);
        free(A_uni);
        free(X_uni);
        free(rho);
        free(mu);
        printf("Terminado %s -> %s\n", archivo_red, archivo_salida);
    }

    printf("Simulaciones finalizadas.\n");
    return 0;
}