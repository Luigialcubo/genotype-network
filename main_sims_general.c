
#include "definiciones.h"   // Contiene dT, ParametrosSIMS, etc.
// Para compilar: gcc -o main_sims_general main_sims_general.c rk4_sims.c -lm -std=c99 -Wall
// Para ejecutar: ./main_sims_general.exe

int main(){
    // Parámetros fijos del modelo (Figura 3)
    ParametrosSIMS pa_het_gamma_0;
    pa_het_gamma_0.beta  = 0.3;
    pa_het_gamma_0.mu_0  = 0.1;
    pa_het_gamma_0.Dx    = 1.0e-5;
    pa_het_gamma_0.alfa  = 0.03;
    pa_het_gamma_0.delta = 3.0;        // inmunidad cruzada con alcance 3
    pa_het_gamma_0.gamma = 0;
    pa_het_gamma_0.N     = 23;

    ParametrosSIMS pa_het_gamma_003;
    pa_het_gamma_003.beta  = 0.3;
    pa_het_gamma_003.mu_0  = 0.1;
    pa_het_gamma_003.Dx    = 1.0e-5;
    pa_het_gamma_003.alfa  = 0.03;
    pa_het_gamma_003.delta = 3.0;        // inmunidad cruzada con alcance 3
    pa_het_gamma_003.gamma = 0.03;
    pa_het_gamma_003.N     = 23;

    ParametrosSIMS pa_hom_gamma_0;
    pa_hom_gamma_0.beta  = 0.3;
    pa_hom_gamma_0.mu_0  = 0.1;
    pa_hom_gamma_0.Dx    = 1.0e-5;
    pa_hom_gamma_0.alfa  = 0.03;
    pa_hom_gamma_0.delta = 3.0;        // inmunidad cruzada con alcance 3
    pa_hom_gamma_0.gamma = 0;
    pa_hom_gamma_0.N     = 20;

    ParametrosSIMS pa_hom_gamma_003;
    pa_hom_gamma_003.beta  = 0.3;
    pa_hom_gamma_003.mu_0  = 0.1;
    pa_hom_gamma_003.Dx    = 1.0e-5;
    pa_hom_gamma_003.alfa  = 0.03;
    pa_hom_gamma_003.delta = 3.0;        // inmunidad cruzada con alcance 3
    pa_hom_gamma_003.gamma = 0.03;
    pa_hom_gamma_003.N     = 20;

    ParametrosSIMS pa_reticula_gamma_0;
    pa_reticula_gamma_0.beta  = 0.3;
    pa_reticula_gamma_0.mu_0  = 0.1;
    pa_reticula_gamma_0.Dx    = 1.0e-5;
    pa_reticula_gamma_0.alfa  = 0.03;
    pa_reticula_gamma_0.delta = 3.0;        // inmunidad cruzada con alcance 3
    pa_reticula_gamma_0.gamma = 0;
    pa_reticula_gamma_0.N     = 100;
         
    ParametrosSIMS pa_reticula_gamma_003;
    pa_reticula_gamma_003.beta  = 0.3;
    pa_reticula_gamma_003.mu_0  = 0.1;
    pa_reticula_gamma_003.Dx    = 1.0e-5;
    pa_reticula_gamma_003.alfa  = 0.03;
    pa_reticula_gamma_003.delta = 3.0;        // inmunidad cruzada con alcance 3
    pa_reticula_gamma_003.gamma = 0.03;
    pa_reticula_gamma_003.N     = 100;

    //Retícula 10x10
    double A_uni_reticula[pa_reticula_gamma_0.N*pa_reticula_gamma_0.N], X_uni_reticula[pa_reticula_gamma_0.N*pa_reticula_gamma_0.N];
    //Homogénea
    double A_uni_hom[pa_hom_gamma_0.N*pa_hom_gamma_0.N], X_uni_hom[pa_hom_gamma_0.N*pa_hom_gamma_0.N];
    //Heterogénea
    double A_uni_het[pa_het_gamma_0.N*pa_het_gamma_0.N], X_uni_het[pa_het_gamma_0.N*pa_het_gamma_0.N];

    double rho_het_gamma_0[pa_het_gamma_0.N], mu_het_gamma_0[pa_het_gamma_0.N];
    double rho_het_gamma_003[pa_het_gamma_003.N], mu_het_gamma_003[pa_het_gamma_003.N];
    double rho_hom_gamma_0[pa_hom_gamma_0.N], mu_hom_gamma_0[pa_hom_gamma_0.N];
    double rho_hom_gamma_003[pa_hom_gamma_003.N], mu_hom_gamma_003[pa_hom_gamma_003.N];
    double rho_reticula_gamma_0[pa_reticula_gamma_0.N], mu_reticula_gamma_0[pa_reticula_gamma_0.N];
    double rho_reticula_gamma_003[pa_reticula_gamma_003.N], mu_reticula_gamma_003[pa_reticula_gamma_003.N];
    double I_het_gamma_0, I_het_gamma_003, I_hom_gamma_0, I_hom_gamma_003, I_reticula_gamma_0, I_reticula_gamma_003;
    double t_het_gamma_0, t_het_gamma_003, t_hom_gamma_0, t_hom_gamma_003, t_reticula_gamma_0, t_reticula_gamma_003;
    double iter_het_gamma_0, iter_het_gamma_003, iter_hom_gamma_0, iter_hom_gamma_003, iter_reticula_gamma_0, iter_reticula_gamma_003;
    int i;

    FILE *sims_het_gamma_0;
    FILE *sims_het_gamma_003;
    FILE *sims_hom_gamma_0;
    FILE *sims_hom_gamma_003;
    FILE *sims_reticula_gamma_0;
    FILE *sims_reticula_gamma_003;

    sims_het_gamma_0 = fopen("results/fig3/sims_het_gamma_0.txt", "w");
    sims_het_gamma_003 = fopen("results/fig3/sims_het_gamma_003.txt", "w");
    sims_hom_gamma_0 = fopen("results/fig3/sims_hom_gamma_0.txt", "w");
    sims_hom_gamma_003 = fopen("results/fig3/sims_hom_gamma_003.txt", "w");
    sims_reticula_gamma_0 = fopen("results/fig3/sims_reticula_gamma_0.txt", "w");
    sims_reticula_gamma_003 = fopen("results/fig3/sims_reticula_gamma_003.txt", "w");

    if (sims_het_gamma_0 == NULL || sims_het_gamma_003 == NULL || sims_hom_gamma_0 == NULL || sims_hom_gamma_003 == NULL || sims_reticula_gamma_0 == NULL || sims_reticula_gamma_003 == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        return 1; 
    }

     // Condiciones iniciales: solo el nodo 0 infectado
    
    for (int i = 0; i < pa_het_gamma_0.N; i++) {
        rho_het_gamma_0[i] = 0.0;
        mu_het_gamma_0[i]  = pa_het_gamma_0.mu_0;
        rho_het_gamma_003[i] = 0.0;
        mu_het_gamma_003[i]  = pa_het_gamma_003.mu_0;
    }
    rho_het_gamma_0[0] = rho_het_gamma_003[0] = I_het_gamma_003 = I_het_gamma_0 = 0.01;

    for (int i = 0; i < pa_hom_gamma_0.N; i++) {
        rho_hom_gamma_0[i] = 0.0;
        mu_hom_gamma_0[i]  = pa_hom_gamma_0.mu_0;
        rho_hom_gamma_003[i] = 0.0;
        mu_hom_gamma_003[i]  = pa_hom_gamma_003.mu_0;
    }
    rho_hom_gamma_0[0] = rho_hom_gamma_003[0] = I_hom_gamma_0 = I_hom_gamma_003 = 0.01;

    for (int i = 0; i < pa_reticula_gamma_0.N; i++) {
        rho_reticula_gamma_0[i] = 0.0;
        mu_reticula_gamma_0[i]  = pa_reticula_gamma_0.mu_0;
        rho_reticula_gamma_003[i] = 0.0;
        mu_reticula_gamma_003[i]  = pa_reticula_gamma_003.mu_0;
    }
    rho_reticula_gamma_0[0] = rho_reticula_gamma_003[0] = I_reticula_gamma_0 = I_reticula_gamma_003 = 0.01;

    // Carga las tres redes y calcula sus matrices de adyacencia y distancias
    cargar_red("results/redes_txt/red_heterogenea.txt", pa_het_gamma_0.N, A_uni_het, X_uni_het);
    cargar_red("results/redes_txt/red_homogenea.txt", pa_hom_gamma_0.N, A_uni_hom, X_uni_hom);
    cargar_red("results/redes_txt/reticula_10x10.txt", pa_reticula_gamma_0.N, A_uni_reticula, X_uni_reticula);

    // SIMS het gamma = 0
    do{
        fprintf(sims_het_gamma_0, "%f\t%f\n", t_het_gamma_0, I_het_gamma_0);
        I_het_gamma_0 = 0;
        paso_rk4_sims(rho_het_gamma_0,mu_het_gamma_0,X_uni_het,A_uni_het,pa_het_gamma_0);
        for(i=0;i<pa_het_gamma_0.N;i++){
            I_het_gamma_0+=rho_het_gamma_0[i];
        }
        t_het_gamma_0 += dT;
        iter_het_gamma_0++;
    } while (iter_het_gamma_0 < (2000/dT));

    // SIMS het gamma = 003
    do{
        fprintf(sims_het_gamma_003, "%f\t%f\n", t_het_gamma_003, I_het_gamma_003);
        I_het_gamma_003 = 0;
        paso_rk4_sims(rho_het_gamma_003,mu_het_gamma_003,X_uni_het,A_uni_het,pa_het_gamma_003);
        for(i=0;i<pa_het_gamma_003.N;i++){
            I_het_gamma_003+=rho_het_gamma_003[i];
        }
        t_het_gamma_003 += dT;
        iter_het_gamma_003++;
    } while (iter_het_gamma_003 < (2000/dT));

    // SIMS hom gamma = 0
    do{
        fprintf(sims_hom_gamma_0, "%f\t%f\n", t_hom_gamma_0, I_hom_gamma_0);
        I_hom_gamma_0 = 0;
        paso_rk4_sims(rho_hom_gamma_0,mu_hom_gamma_0,X_uni_hom,A_uni_hom,pa_hom_gamma_0);
        for(i=0;i<pa_hom_gamma_0.N;i++){
            I_hom_gamma_0+=rho_hom_gamma_0[i];
        }
        t_hom_gamma_0 += dT;
        iter_hom_gamma_0++;
    } while (iter_hom_gamma_0 < (2000/dT));

    // SIMS hom gamma = 003
    do{
        fprintf(sims_hom_gamma_003, "%f\t%f\n", t_hom_gamma_003, I_hom_gamma_003);
        I_hom_gamma_003 = 0;
        paso_rk4_sims(rho_hom_gamma_003,mu_hom_gamma_003,X_uni_hom,A_uni_hom,pa_hom_gamma_003);
        for(i=0;i<pa_hom_gamma_003.N;i++){
            I_hom_gamma_003+=rho_hom_gamma_003[i];
        }
        t_hom_gamma_003 += dT;
        iter_hom_gamma_003++;
    } while (iter_hom_gamma_003 < (2000/dT));

    // SIMS reticula gamma = 0
    do{
        fprintf(sims_reticula_gamma_0, "%f\t%f\n", t_reticula_gamma_0, I_reticula_gamma_0);
        I_reticula_gamma_0 = 0;
        paso_rk4_sims(rho_reticula_gamma_0,mu_reticula_gamma_0,X_uni_reticula,A_uni_reticula,pa_reticula_gamma_0);
        for(i=0;i<pa_reticula_gamma_0.N;i++){
            I_reticula_gamma_0+=rho_reticula_gamma_0[i];
        }
        t_reticula_gamma_0 += dT;
        iter_reticula_gamma_0++;
    } while (iter_reticula_gamma_0 < (2000/dT));

    // SIMS reticula gamma = 003
    do{
        fprintf(sims_reticula_gamma_003, "%f\t%f\n", t_reticula_gamma_003, I_reticula_gamma_003);
        I_reticula_gamma_003 = 0;
        paso_rk4_sims(rho_reticula_gamma_003,mu_reticula_gamma_003,X_uni_reticula,A_uni_reticula,pa_reticula_gamma_003);
        for(i=0;i<pa_reticula_gamma_003.N;i++){
            I_reticula_gamma_003+=rho_reticula_gamma_003[i];
        }
        t_reticula_gamma_003 += dT;
        iter_reticula_gamma_003++;
    } while (iter_reticula_gamma_003 < (2000/dT));

    fclose(sims_het_gamma_0);
    fclose(sims_het_gamma_003);
    fclose(sims_hom_gamma_0);
    fclose(sims_hom_gamma_003);
    fclose(sims_reticula_gamma_0);
    fclose(sims_reticula_gamma_003);

    return 0;
}