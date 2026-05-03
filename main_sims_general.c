/*
 * main_sims_general.c  (versión Windows)
 *
 * Genera los datos de la Figura 3:
 *   - Simula el modelo SIMS sobre las tres redes sintéticas.
 *   - Para cada red se ejecutan dos valores de gamma (0 y 0.03).
 *   - Se guardan archivos de dos columnas (tiempo, I(t)).
 *
 * Utiliza las funciones genéricas de rk4_sims_general.c:
 *   derivada_general(rho, mu, X, A, p, drho, dmu)
 *   paso_rk4_sims_general(rho, mu, X, A, p)
 *
 * Compilación (Windows, MinGW o similar):
 *   gcc -o sims_general main_sims_general.c rk4_sims_general.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "definiciones.h"

/* Prototipos de las funciones genéricas */
void derivada_general(double rho[], double mu[], double **X, double **A,
              ParametrosSIMS p, double drho[], double dmu[]);
void paso_rk4_sims_general(double rho[], double mu[], double **X, double **A,
                   ParametrosSIMS p);

/* ------------------------------------------------------------
 * Lee una red desde archivo (formato: N, luego aristas i j)
 * y construye las matrices de adyacencia A y distancias X.
 * ------------------------------------------------------------ */
int leer_red_dinamica(const char *nombre, double ***A_out, double ***X_out) {
    FILE *f = fopen(nombre, "r");
    if (!f) {
        printf("Error: no se pudo abrir %s\n", nombre);
        exit(1);
    }
    int N;
    fscanf(f, "%d", &N);

    double **A = (double **)malloc(N * sizeof(double *));
    double **X = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        A[i] = (double *)calloc(N, sizeof(double));
        X[i] = (double *)malloc(N * sizeof(double));
    }

    int u, v;
    while (fscanf(f, "%d %d", &u, &v) == 2) {
        A[u][v] = 1.0;
        A[v][u] = 1.0;
    }
    fclose(f);

    /* Floyd-Warshall para distancias más cortas */
    double INF = 1e9;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) X[i][j] = 0.0;
            else if (A[i][j] > 0.5) X[i][j] = 1.0;
            else X[i][j] = INF;
        }
    }
    for (int k = 0; k < N; k++)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (X[i][k] + X[k][j] < X[i][j])
                    X[i][j] = X[i][k] + X[k][j];

    *A_out = A;
    *X_out = X;
    return N;
}

/* ------------------------------------------------------------
 * Libera una matriz de N filas.
 * ------------------------------------------------------------ */
void liberar_matriz_dinamica(double **M, int N) {
    for (int i = 0; i < N; i++) free(M[i]);
    free(M);
}

/* ------------------------------------------------------------
 * Ejecuta la simulación y guarda I(t) en archivo.
 * ------------------------------------------------------------ */
void simular_fig3(int N, double **A, double **X, double gamma,
                  const char *archivo_salida, double t_max) {
    ParametrosSIMS pa;
    pa.beta  = 0.3;
    pa.mu_0  = 0.1;
    pa.Dx    = 1.0e-5;
    pa.alfa  = 0.03;
    pa.delta = 3.0;
    pa.gamma = gamma;
    pa.N     = N;

    double rho[N], mu[N];
    for (int i = 0; i < N; i++) {
        rho[i] = 0.0;
        mu[i]  = pa.mu_0;
    }
    rho[0] = 0.01;   // wild type

    double t = 0.0;
    int max_iter = (int)(t_max / dT);

    FILE *salida = fopen(archivo_salida, "w");
    if (!salida) {
        printf("Error creando %s\n", archivo_salida);
        exit(1);
    }

    for (int iter = 0; iter < max_iter; iter++) {
        double I = 0.0;
        for (int i = 0; i < N; i++) I += rho[i];
        fprintf(salida, "%.6f\t%.6f\n", t, I);
        paso_rk4_sims_general(rho, mu, X, A, pa);
        t += dT;
    }
    fclose(salida);
}

/* ------------------------------------------------------------
 * MAIN
 * ------------------------------------------------------------ */
int main() {
    /* Archivos de las redes (ruta relativa a donde se ejecute el .exe) */
    const char *nombres_red[] = {
        "results/redes_txt/reticula_10x10.txt",
        "results/redes_txt/red_homogenea.txt",
        "results/redes_txt/red_heterogenea.txt"
    };
    const char *etiquetas[] = {"reticula", "hom", "het"};
    double gammas[]         = {0.0, 0.03};
    const char *gamma_str[] = {"0", "003"};

    /* La carpeta de salida results/fig3 ya existe, no se intenta crear */

    for (int r = 0; r < 3; r++) {
        double **A, **X;
        int N = leer_red_dinamica(nombres_red[r], &A, &X);
        printf("Simulando red %s (N=%d)...\n", etiquetas[r], N);

        for (int g = 0; g < 2; g++) {
            char archivo_salida[256];
            sprintf(archivo_salida, "results/fig3/datos_sims_%s_gamma_%s.txt",
                    etiquetas[r], gamma_str[g]);

            printf("   gamma=%.2f -> %s\n", gammas[g], archivo_salida);
            simular_fig3(N, A, X, gammas[g], archivo_salida, 2000.0);
        }
        liberar_matriz_dinamica(A, N);
        liberar_matriz_dinamica(X, N);
    }

    printf("Simulaciones terminadas.\n");
    return 0;
}