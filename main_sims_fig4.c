
#include "definiciones.h"
// Para compilar: gcc -o main_sims_fig4 main_sims_fig4.c rk4_sims.c -lm -std=c99 -Wall
// Para ejecutar: ./main_sims_fig4.exe

int main() {
    const char *archivos_red[3], *archivos_salida[3];
    const char *archivo_red, *archivo_salida, *archivo_particion;
    int nodo_inicial, c, N, max_iter, guardar_cada, i, iter;
    double t_max, *A_uni, *X_uni, *rho, *mu, t;
    FILE *f_tmp, *salida, *f_part;
    
    ParametrosSIMS pa;
    pa.beta  = 0.3;
    pa.mu_0  = 0.1;
    pa.Dx    = 1.0e-5;
    pa.alfa  = 0.03;
    pa.delta = 3.0;
    pa.gamma = 0.0;

    archivos_red[0] = "componentes_conexas/componente_1.txt";
    archivos_red[1] = "componentes_conexas/componente_2.txt";
    archivos_red[2] = "componentes_conexas/componente_7.txt";
    archivos_salida[0] = "results/fig4/sims_comp1.txt";
    archivos_salida[1] = "results/fig4/sims_comp2.txt";
    archivos_salida[2] = "results/fig4/sims_comp7.txt";
    
    nodo_inicial = 0;
    t_max = 1000.0;
    guardar_cada = 10;

    for (c = 0; c < 3; c++) {
        archivo_red    = archivos_red[c];
        archivo_salida = archivos_salida[c];

        // Leer N (número de nodos)
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

        // Reservar memoria para A, X, rho, mu
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

        // --- Cargar partición de comunidades ---
        // Construir ruta del archivo de partición
        char ruta_part[256];
        sprintf(ruta_part, "results/fig4/particion_comp%d.txt", c==0?1:(c==1?2:7));
        archivo_particion = ruta_part;

        // Array para almacenar la comunidad de cada nodo
        int *comm_id = (int *)malloc(N * sizeof(int));
        if (!comm_id) {
            printf("Error de memoria para comm_id\n");
            free(A_uni); free(X_uni); free(rho); free(mu);
            continue;
        }
        // Inicializar con -1
        for (i = 0; i < N; i++) comm_id[i] = -1;

        f_part = fopen(archivo_particion, "r");
        if (!f_part) {
            printf("Error al abrir %s\n", archivo_particion);
            free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
            continue;
        }
        int nodo, com;
        while (fscanf(f_part, "%d %d", &nodo, &com) == 2) {
            if (nodo >= 0 && nodo < N)
                comm_id[nodo] = com;
        }
        fclose(f_part);

        // Determinar número de comunidades y mapear a índices 0..K-1
        int K = 0;
        int *map_com = (int *)malloc(N * sizeof(int));  // temporal para reindexar
        if (!map_com) {
            printf("Error de memoria\n");
            free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
            continue;
        }
        for (i = 0; i < N; i++) map_com[i] = -1;
        for (i = 0; i < N; i++) {
            int cid = comm_id[i];
            if (cid != -1 && map_com[cid] == -1) {
                map_com[cid] = K;
                K++;
            }
        }
        // Reescribir comm_id con los índices compactos (0..K-1)
        for (i = 0; i < N; i++) {
            if (comm_id[i] != -1)
                comm_id[i] = map_com[comm_id[i]];
            else
                comm_id[i] = 0; // por seguridad, aunque no debería haber nodos sin comunidad
        }
        free(map_com);
        printf("  Comunidades cargadas: %d\n", K);

        // Array para prevalencia por comunidad
        double *I_comm = (double *)malloc(K * sizeof(double));
        if (!I_comm) {
            printf("Error de memoria para I_comm\n");
            free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
            continue;
        }

        // Condiciones iniciales
        for (i = 0; i < N; i++) {
            rho[i] = 0.0;
            mu[i]  = pa.mu_0;
        }
        rho[nodo_inicial] = 0.01;

        // Abrir archivo de salida
        salida = fopen(archivo_salida, "w");
        if (!salida) {
            printf("Error creando %s\n", archivo_salida);
            free(I_comm); free(comm_id); free(A_uni); free(X_uni); free(rho); free(mu);
            continue;
        }

        // Simulación
        t = 0.0;
        max_iter = (int)(t_max / dT);
        printf("Simulando...\n");

        for (iter = 0; iter < max_iter; iter++) {
            if (iter % guardar_cada == 0) {
                // Calcular prevalencia por comunidad
                for (i = 0; i < K; i++) I_comm[i] = 0.0;
                for (i = 0; i < N; i++) {
                    int cid = comm_id[i];
                    I_comm[cid] += rho[i];
                }
                // Escribir tiempo y las K columnas
                fprintf(salida, "%.6f", t);
                for (i = 0; i < K; i++) {
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
    }

    printf("Simulaciones finalizadas.\n");
    return 0;
}