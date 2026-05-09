/*
 * main_sims_fig4_comp7.c
 *
 * Simulación del modelo SIMS para la componente 7.
 *
 * Lee:
 *   - Red:        componentes_conexas_temporales/componente_7_temporal.edge
 *   - Partición:  results/fig4/particion_comp7.txt
 *
 * Guarda:
 *   - results/fig4/sims_comp7.txt
 *
 * Formato de salida:
 *   t   I_total   I_comunidad_0   I_comunidad_1   ...
 *
 * Compilación:
 *   gcc -o main_sims_fig4_comp7 main_sims_fig4_comp7.c rk4_sims.c -lm -std=c99 -Wall
 *
 * Ejecución:
 *   ./main_sims_fig4_comp7
 */

#include "definiciones.h"
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------
   Calcula N a partir de un archivo .edge.
   El archivo .edge tiene líneas:
       u   v

   Si el máximo nodo es 128, entonces N = 129.
   ------------------------------------------------------------ */
// int calcular_N_desde_edge(const char *archivo_red) {
//     FILE *f = fopen(archivo_red, "r");

//     if (!f) {
//         printf("Error al abrir %s\n", archivo_red);
//         exit(1);
//     }

//     int u, v;
//     int max_node = -1;

//     while (fscanf(f, "%d %d", &u, &v) == 2) {
//         if (u > max_node) max_node = u;
//         if (v > max_node) max_node = v;
//     }

//     fclose(f);

//     if (max_node < 0) {
//         printf("Error: no se han leído enlaces en %s\n", archivo_red);
//         exit(1);
//     }

//     return max_node + 1;
// }

/* ------------------------------------------------------------
   Calcula el grado de un nodo usando A_uni.
   Sirve para comprobar si la red se ha cargado bien.
   ------------------------------------------------------------ */
double calcular_grado_nodo(double *A_uni, int N, int nodo) {
    double grado = 0.0;

    for (int j = 0; j < N; j++) {
        grado += A_uni[nodo * N + j];
    }

    return grado;
}

/* ------------------------------------------------------------
   Programa principal
   ------------------------------------------------------------ */
int main() {

    /* ------------------------------
       Parámetros del modelo SIMS
       ------------------------------ */
    ParametrosSIMS pa;

    pa.beta  = 0.3;
    pa.mu_0  = 0.1;
    pa.Dx    = 1.0e-5;
    pa.alfa  = 0.03;
    pa.delta = 3.0;
    pa.gamma = 0.0;
    
    /* ------------------------------
       Archivos
       ------------------------------ */
    const char *archivo_red =
        "componentes_conexas_temporales/componente_7_temporal_con_N.txt";

    const char *archivo_salida =
        "results/fig4/sims_comp7.txt";

    const char *archivo_particion =
        "results/fig4/particion_comp7.txt";

    /* ------------------------------
       Configuración de simulación
       ------------------------------ */
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
    /* ------------------------------
       Calcular N desde el .edge
       ------------------------------ */
    

    printf("========================================\n");
    printf("SIMULACIÓN COMPONENTE 7\n");
    printf("Red: %s\n", archivo_red);
    printf("Partición: %s\n", archivo_particion);
    printf("Salida: %s\n", archivo_salida);
    printf("N = %d\n", N);
    printf("Nodo inicial = %d\n", nodo_inicial);
    printf("beta = %.4f\n", pa.beta);
    printf("mu0 = %.4f\n", pa.mu_0);
    printf("Dx = %.4e\n", pa.Dx);
    printf("alfa = %.4f\n", pa.alfa);
    printf("delta = %.4f\n", pa.delta);
    printf("gamma = %.4f\n", pa.gamma);
    printf("t_max = %.2f\n", t_max);
    printf("========================================\n");

    if (nodo_inicial < 0 || nodo_inicial >= N) {
        printf("Error: nodo_inicial = %d fuera de rango [0, %d]\n",
               nodo_inicial, N - 1);
        return 1;
    }

    /* ------------------------------
       Reservar memoria
       ------------------------------ */
    double *A_uni = (double *)malloc(N * N * sizeof(double));
    double *X_uni = (double *)malloc(N * N * sizeof(double));
    double *rho   = (double *)malloc(N * sizeof(double));
    double *mu    = (double *)malloc(N * sizeof(double));

    if (!A_uni || !X_uni || !rho || !mu) {
        printf("Error de memoria\n");

        free(A_uni);
        free(X_uni);
        free(rho);
        free(mu);

        return 1;
    }

    /* ------------------------------
       Cargar red y matriz de distancias
       ------------------------------ */
    cargar_red(archivo_red, N, A_uni, X_uni);

    /* ------------------------------
       Diagnóstico: grado del nodo inicial
       ------------------------------ */
    double grado_inicial = calcular_grado_nodo(A_uni, N, nodo_inicial);

    printf("Grado del nodo inicial %d = %.0f\n",
           nodo_inicial, grado_inicial);

    if (grado_inicial <= 0.0) {
        printf("ADVERTENCIA: el nodo inicial tiene grado 0.\n");
        printf("Puede que la red no se haya cargado bien o que el nodo inicial no tenga vecinos.\n");
    }

    /* ------------------------------
       Cargar partición de comunidades
       ------------------------------ */
    int *comm_id = (int *)malloc(N * sizeof(int));

    if (!comm_id) {
        printf("Error de memoria para comm_id\n");

        free(A_uni);
        free(X_uni);
        free(rho);
        free(mu);

        return 1;
    }

    for (int i = 0; i < N; i++) {
        comm_id[i] = -1;
    }

    FILE *f_part = fopen(archivo_particion, "r");

    if (!f_part) {
        printf("Error al abrir %s\n", archivo_particion);

        free(comm_id);
        free(A_uni);
        free(X_uni);
        free(rho);
        free(mu);

        return 1;
    }

    int nodo, com;

    while (fscanf(f_part, "%d %d", &nodo, &com) == 2) {
        if (nodo >= 0 && nodo < N) {
            comm_id[nodo] = com;
        }
    }

    fclose(f_part);

    /* ------------------------------
       Remapear comunidades a 0, 1, 2, ...
       ------------------------------ */

    int max_com = -1;

    for (int i = 0; i < N; i++) {
        if (comm_id[i] > max_com) {
            max_com = comm_id[i];
        }
    }

    if (max_com < 0) {
        printf("ADVERTENCIA: no se han cargado comunidades válidas.\n");
        printf("Todos los nodos se asignarán a la comunidad 0.\n");

        for (int i = 0; i < N; i++) {
            comm_id[i] = 0;
        }

        max_com = 0;
    }

    int *map_com = (int *)malloc((max_com + 1) * sizeof(int));

    if (!map_com) {
        printf("Error de memoria para map_com\n");

        free(comm_id);
        free(A_uni);
        free(X_uni);
        free(rho);
        free(mu);

        return 1;
    }

    for (int i = 0; i <= max_com; i++) {
        map_com[i] = -1;
    }

    int K = 0;

    for (int i = 0; i < N; i++) {
        int cid = comm_id[i];

        if (cid >= 0 && cid <= max_com && map_com[cid] == -1) {
            map_com[cid] = K;
            K++;
        }
    }

    if (K == 0) {
        K = 1;
    }

    for (int i = 0; i < N; i++) {
        int cid = comm_id[i];

        if (cid >= 0 && cid <= max_com && map_com[cid] != -1) {
            comm_id[i] = map_com[cid];
        } else {
            comm_id[i] = 0;
        }
    }

    free(map_com);

    printf("Comunidades cargadas y remapeadas: K = %d\n", K);

    /* ------------------------------
       Diagnóstico: tamaño de cada comunidad
       ------------------------------ */
    int *count_comm = (int *)calloc(K, sizeof(int));

    if (!count_comm) {
        printf("Error de memoria para count_comm\n");

        free(comm_id);
        free(A_uni);
        free(X_uni);
        free(rho);
        free(mu);

        return 1;
    }

    for (int i = 0; i < N; i++) {
        int cid = comm_id[i];

        if (cid >= 0 && cid < K) {
            count_comm[cid]++;
        }
    }

    printf("Tamaño de comunidades:\n");

    for (int c = 0; c < K; c++) {
        printf("  Comunidad %d: %d nodos\n", c, count_comm[c]);
    }

    free(count_comm);

    /* ------------------------------
       Reservar prevalencia por comunidad
       ------------------------------ */
    double *I_comm = (double *)malloc(K * sizeof(double));

    if (!I_comm) {
        printf("Error de memoria para I_comm\n");

        free(comm_id);
        free(A_uni);
        free(X_uni);
        free(rho);
        free(mu);

        return 1;
    }

    /* ------------------------------
       Condiciones iniciales
       ------------------------------ */
    for (int i = 0; i < N; i++) {
        rho[i] = 0.0;
        mu[i]  = pa.mu_0;
    }

    rho[nodo_inicial] = 0.01;

    printf("Condición inicial: rho[%d] = %.6f\n",
           nodo_inicial, rho[nodo_inicial]);

    /* ------------------------------
       Abrir archivo de salida
       ------------------------------ */
    FILE *salida = fopen(archivo_salida, "w");

    if (!salida) {
        printf("Error creando %s\n", archivo_salida);

        free(I_comm);
        free(comm_id);
        free(A_uni);
        free(X_uni);
        free(rho);
        free(mu);

        return 1;
    }

    /* ------------------------------
       Simulación temporal
       ------------------------------ */
    double t = 0.0;
    int max_iter = (int)(t_max / dT);

    printf("Simulando...\n");

    for (int iter = 0; iter < max_iter; iter++) {

        if (iter % guardar_cada == 0) {

            for (int c = 0; c < K; c++) {
                I_comm[c] = 0.0;
            }

            double I_total = 0.0;

            for (int i = 0; i < N; i++) {
                int cid = comm_id[i];

                if (cid >= 0 && cid < K) {
                    I_comm[cid] += rho[i];
                }

                I_total += rho[i];
            }

            /*
             * Formato:
             * t   I_total   I_comm_0   I_comm_1   ...
             */
            fprintf(salida, "%.6f\t%.12e", t, I_total);

            for (int c = 0; c < K; c++) {
                fprintf(salida, "\t%.12e", I_comm[c]);
            }

            fprintf(salida, "\n");
        }

        paso_rk4_sims(rho, mu, X_uni, A_uni, pa);
        t += dT;
    }

    fclose(salida);

    /* ------------------------------
       Liberar memoria
       ------------------------------ */
    free(I_comm);
    free(comm_id);
    free(A_uni);
    free(X_uni);
    free(rho);
    free(mu);

    printf("Terminado: %s -> %s\n", archivo_red, archivo_salida);

    return 0;
}