/*
 * main_sims_general.c
 *
 * Genera los datos para la Figura 3 del artículo usando las funciones genéricas
 * de rk4_sims_general.c.
 *   - Lee las tres redes sintéticas desde results/redes_txt/
 *   - Simula el modelo SIMS con inmunidad cruzada (delta = 3) para
 *     gamma = 0 y gamma = 0.03
 *   - Guarda archivos de dos columnas: tiempo  I(t)
 *
 * Compilación:
 *   gcc -o sims_general main_sims_general.c rk4_sims_general.c -lm -std=c99
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "definiciones.h"   // Contiene dT, ParametrosSIMS, etc.


void paso_rk4_sims_general(double rho[], double mu[], double **X, double **A,
                           ParametrosSIMS p);

/* ------------------------------------------------------------
 * Lee la red desde un archivo con el formato:
 *   N
 *   i1 j1
 *   i2 j2
 *   ...
 * Construye la matriz de adyacencia A (double **) y calcula
 * la matriz de distancias X (camino más corto) con Floyd‑Warshall.
 * Devuelve el número de nodos N.
 * ------------------------------------------------------------ */
int leer_red(const char *nombre, double ***A_out, double ***X_out) {
    FILE *f = fopen(nombre, "r");
    if (!f) {
        printf("Error: no se pudo abrir %s\n", nombre);
        exit(1);
    }
    int N;
    fscanf(f, "%d", &N);

    // Asignar memoria para A y X (matrices N x N)
    double **A = (double **)malloc(N * sizeof(double *));
    double **X = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        A[i] = (double *)calloc(N, sizeof(double));  // todo ceros
        X[i] = (double *)malloc(N * sizeof(double));
    }

    // Leer aristas
    int u, v;
    while (fscanf(f, "%d %d", &u, &v) == 2) {
        A[u][v] = 1.0;
        A[v][u] = 1.0;
    }
    fclose(f);

    // Floyd‑Warshall para distancias más cortas
    double INF = 1e9;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j)      X[i][j] = 0.0;
            else if (A[i][j] > 0.5) X[i][j] = 1.0;
            else             X[i][j] = INF;
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
void liberar_matriz(double **M, int N) {
    for (int i = 0; i < N; i++) free(M[i]);
    free(M);
}

/* ------------------------------------------------------------
 * Simula el modelo SIMS sobre una red dada, para un valor de gamma
 * y guarda la prevalencia total I(t) en el archivo indicado.
 * ------------------------------------------------------------ */
void simular_red(int N, double **A, double **X, double gamma,
                 const char *archivo_salida) {
    // Parámetros fijos del modelo (Figura 3)
    ParametrosSIMS pa;
    pa.beta  = 0.3;
    pa.mu_0  = 0.1;
    pa.Dx    = 1.0e-5;
    pa.alfa  = 0.03;
    pa.delta = 3.0;        // inmunidad cruzada con alcance 3
    pa.gamma = gamma;
    pa.N     = N;

    // Condiciones iniciales: solo el nodo 0 infectado
    double rho[N], mu[N];
    for (int i = 0; i < N; i++) {
        rho[i] = 0.0;
        mu[i]  = pa.mu_0;
    }
    rho[0] = 0.01;

    double t = 0.0;
    int max_iter = (int)(2000.0 / dT);   // 5000 días, paso 0.01

    FILE *salida = fopen(archivo_salida, "w");
    if (!salida) {
        printf("Error creando %s\n", archivo_salida);
        exit(1);
    }

    for (int iter = 0; iter < max_iter; iter++) {
        // Prevalencia total
        double I = 0.0;
        for (int i = 0; i < N; i++) I += rho[i];

        fprintf(salida, "%.6f\t%.6f\n", t, I);

        // Un paso del integrador RK4
        paso_rk4_sims_general(rho, mu, X, A, pa);
        t += dT;
    }

    fclose(salida);
}

/* ------------------------------------------------------------
 * MAIN
 * ------------------------------------------------------------ */
int main() {
    // Lista de archivos de red y etiquetas
    const char *nombres_red[] = {
        "results/redes_txt/reticula_10x10.txt",
        "results/redes_txt/red_homogenea.txt",
        "results/redes_txt/red_heterogenea.txt"
    };
    const char *etiquetas[] = {"reticula", "hom", "het"};
    double gammas[]         = {0.0, 0.03};
    const char *gamma_str[] = {"0", "003"};

    // Para cada red…
    for (int r = 0; r < 3; r++) {
        double **A, **X;
        int N = leer_red(nombres_red[r], &A, &X);
        printf("Red %s cargada (N=%d)\n", etiquetas[r], N);

        // …y cada valor de gamma
        for (int g = 0; g < 2; g++) {
            char archivo_salida[256];
            sprintf(archivo_salida, "results/fig3/datos_sims_%s_gamma_%s.txt",
                    etiquetas[r], gamma_str[g]);

            printf("   Simulando gamma=%.2f -> %s\n", gammas[g], archivo_salida);
            simular_red(N, A, X, gammas[g], archivo_salida);
        }

        liberar_matriz(A, N);
        liberar_matriz(X, N);
    }

    printf("Simulaciones terminadas.\n");
    return 0;
}