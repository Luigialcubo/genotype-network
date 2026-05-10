/*
 * main_sims_fig4_comp1.c
 * Simula el modelo SIMS para la componente 1.
 *
 * Compilar:
 *   gcc -o main_sims_fig4_comp1 main_sims_fig4_comp1.c rk4_sims.c -lm -std=c99 -Wall
 * Ejecutar:
 *   ./main_sims_fig4_comp1
 */

#include "definiciones.h"

int main() {
    // Parámetros del modelo (idénticos a los de la componente 7)
    ParametrosSIMS pa;
    pa.beta  = 0.3;
    pa.mu_0  = 0.1;
    pa.Dx    = 1.0e-5;
    pa.alfa  = 0.03;
    pa.delta = 3.0;
    pa.gamma = 0.0;

    // Archivos específicos para la componente 1
    const char *archivo_red       = "componentes_conexas/componente_1.txt";
    const char *archivo_salida    = "results/fig4/sims_comp1.txt";
    const char *archivo_particion = "results/fig4/particion_comp1.txt";

    // El wild‑type es el nodo 1385 según se indica
    int nodo_inicial = 1385;
    double t_max     = 1000.0;        
    int guardar_cada = 10;

    // Leer número de nodos N
    FILE *f_tmp = fopen(archivo_red, "r");
    if (!f_tmp) {
        printf("Error al abrir %s\n", archivo_red);
        return 1;
    }
    int N;
    fscanf(f_tmp, "%d", &N);
    fclose(f_tmp);
    pa.N = N;

    printf("Componente 1: N = %d\n", N);

    // Reservar memoria para matrices y vectores
    double *A_uni = (double *)malloc(N * N * sizeof(double));
    double *X_uni = (double *)malloc(N * N * sizeof(double));
    double *rho   = (double *)malloc(N * sizeof(double));
    double *mu    = (double *)malloc(N * sizeof(double));
    if (!A_uni || !X_uni || !rho || !mu) {
        printf("Error de memoria\n");
        free(A_uni); free(X_uni); free(rho); free(mu);
        return 1;
    }

    // Cargar red (matrices de adyacencia y distancias)
    cargar_red(archivo_red, N, A_uni, X_uni);

    // Cargar partición de comunidades
    int *comm_id = (int *)malloc(N * sizeof(int));
    if (!comm_id) {
        printf("Error de memoria para comm_id\n");
        free(A_uni); free(X_uni); free(rho); free(mu);
        return 1;
    }
    for (int i = 0; i < N; i++) comm_id[i] = -1;

    FILE *f_part = fopen(archivo_particion, "r");
    if (!f_part) {
        printf("Error al abrir %s\n", archivo_particion);
        free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
        return 1;
    }
    int nodo, com;
    while (fscanf(f_part, "%d %d", &nodo, &com) == 2) {
        if (nodo >= 0 && nodo < N)
            comm_id[nodo] = com;
    }
    fclose(f_part);

    // Mapear IDs originales a índices compactos 0..K-1
    int K = 0;
    int *map_com = (int *)malloc(N * sizeof(int));
    if (!map_com) {
        printf("Error de memoria\n");
        free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
        return 1;
    }
    for (int i = 0; i < N; i++) map_com[i] = -1;
    for (int i = 0; i < N; i++) {
        int cid = comm_id[i];
        if (cid != -1 && map_com[cid] == -1) {
            map_com[cid] = K;
            K++;
        }
    }
    for (int i = 0; i < N; i++) {
        if (comm_id[i] != -1)
            comm_id[i] = map_com[comm_id[i]];
        else
            comm_id[i] = 0;   // Nodos no asignados van a comunidad 0
    }
    free(map_com);
    printf("Comunidades detectadas: %d\n", K);

    // Prevalencia por comunidad
    double *I_comm = (double *)malloc(K * sizeof(double));
    if (!I_comm) {
        printf("Error de memoria para I_comm\n");
        free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
        return 1;
    }

    // Condiciones iniciales
    for (int i = 0; i < N; i++) {
        rho[i] = 0.0;
        mu[i]  = pa.mu_0;
    }
    rho[nodo_inicial] = 0.01;

    // Archivo de salida
    FILE *salida = fopen(archivo_salida, "w");
    if (!salida) {
        printf("Error creando %s\n", archivo_salida);
        free(I_comm); free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
        return 1;
    }

    // Integración
    double t = 0.0;
    int max_iter = (int)(t_max / dT);
    printf("Simulando componente 1...\n");

    for (int iter = 0; iter < max_iter; iter++) {
        if (iter % guardar_cada == 0) {
            // Calcular I_comm
            for (int i = 0; i < K; i++) I_comm[i] = 0.0;
            for (int i = 0; i < N; i++) {
                int cid = comm_id[i];
                I_comm[cid] += rho[i];
            }
            // Escribir tiempo y comunidades (K columnas)
            fprintf(salida, "%.6f", t);
            for (int i = 0; i < K; i++) {
                fprintf(salida, "\t%.6f", I_comm[i]);
            }
            fprintf(salida, "\n");
        }
        paso_rk4_sims(rho, mu, X_uni, A_uni, pa);
        t += dT;
    }

    fclose(salida);
    free(I_comm);
    free(comm_id);
    free(A_uni);
    free(X_uni);
    free(rho);
    free(mu);

    printf("Terminado: %s -> %s\n", archivo_red, archivo_salida);
    return 0;
}