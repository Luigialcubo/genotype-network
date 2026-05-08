/*
 * main_sims_fig4_comp2.c
 * 
 * Simulación del modelo SIMS para la COMPONENTE 2 de la red de Influenza A.
 * 
 *   - Lee la red desde "componentes_conexas/componente_2.txt".
 *   - Carga la partición de comunidades desde "results/fig4/particion_comp2.txt".
 *   - Integra con RK4 y guarda la prevalencia por comunidad en "results/fig4/sims_comp2.txt".
 *
 * Compilación:
 *   gcc -o main_sims_fig4_comp2 main_sims_fig4_comp2.c rk4_sims.c -lm -std=c99 -Wall
 * Ejecución:
 *   ./main_sims_fig4_comp2
 */

#include "definiciones.h"

int main() {
    ParametrosSIMS pa;
    pa.beta  = 0.3;
    pa.mu_0  = 0.1;
    pa.Dx    = 1.0e-5;
    pa.alfa  = 0.03;
    pa.delta = 3.0;
    pa.gamma = 0.0;

    const char *archivo_red      = "componentes_conexas/componente_2.txt";
    const char *archivo_salida   = "results/fig4/sims_comp2.txt";
    const char *archivo_particion = "results/fig4/particion_comp2.txt";

    int nodo_inicial = 0;
    double t_max = 1000.0;
    int guardar_cada = 10;

    FILE *f_tmp = fopen(archivo_red, "r");
    if (!f_tmp) {
        printf("Error al abrir %s\n", archivo_red);
        return 1;
    }
    int N;
    fscanf(f_tmp, "%d", &N);
    fclose(f_tmp);
    pa.N = N;

    printf("Componente 2: N=%d, nodo inicial=%d, gamma=%.2f, t_max=%.0f\n",
           N, nodo_inicial, pa.gamma, t_max);

    double *A_uni = (double *)malloc(N * N * sizeof(double));
    double *X_uni = (double *)malloc(N * N * sizeof(double));
    double *rho   = (double *)malloc(N * sizeof(double));
    double *mu    = (double *)malloc(N * sizeof(double));
    if (!A_uni || !X_uni || !rho || !mu) {
        printf("Error de memoria\n");
        free(A_uni); free(X_uni); free(rho); free(mu);
        return 1;
    }

    cargar_red(archivo_red, N, A_uni, X_uni);

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
        comm_id[i] = (comm_id[i] != -1) ? map_com[comm_id[i]] : 0;
    }
    free(map_com);
    printf("  Comunidades cargadas: %d\n", K);

    double *I_comm = (double *)malloc(K * sizeof(double));
    if (!I_comm) {
        printf("Error de memoria para I_comm\n");
        free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        rho[i] = 0.0;
        mu[i]  = pa.mu_0;
    }
    rho[nodo_inicial] = 0.01;

    FILE *salida = fopen(archivo_salida, "w");
    if (!salida) {
        printf("Error creando %s\n", archivo_salida);
        free(I_comm); free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
        return 1;
    }

    double t = 0.0;
    int max_iter = (int)(t_max / dT);
    printf("Simulando componente 2...\n");

    for (int iter = 0; iter < max_iter; iter++) {
        if (iter % guardar_cada == 0) {
            for (int i = 0; i < K; i++) I_comm[i] = 0.0;
            for (int i = 0; i < N; i++) {
                int cid = comm_id[i];
                I_comm[cid] += rho[i];
            }
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

    printf("Terminado %s -> %s\n", archivo_red, archivo_salida);
    return 0;
}