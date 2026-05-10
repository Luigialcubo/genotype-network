#include "definiciones.h"
int main(){
    ParametrosSIMS pa_sis,pa_sir,pa_sirs,pa_simsb,pa_simsc;
    double iter_sis,iter_sir,iter_sirs,iter_simsb,iter_simsc,diff,t_sis,t_sir,t_sirs,t_simsb,t_simsc,I_simsb,I_simsc,R_efectivob,R_efectivoc;

    double A_uni[n*n], X_uni[n*n];

    double rho_sis[n],mu_sis[n],rho_sir[n],mu_sir[n],rho_sirs[n],mu_sirs[n];
    double rho_simsb[n],mu_simsb[n],rho_simsc[n],mu_simsc[n];
    int i,j;

    pa_sis.mu_0=pa_sir.mu_0=pa_sirs.mu_0 =pa_simsb.mu_0=pa_simsc.mu_0= 0.1;
    pa_sis.beta=pa_sir.beta=pa_sirs.beta =pa_simsb.beta =pa_simsc.beta=0.3;
    pa_sis.Dx=pa_sir.Dx=pa_sirs.Dx = 0.0;
    pa_simsb.Dx=pa_simsc.Dx=1.0e-5;
    pa_sis.delta=pa_sir.delta=pa_sirs.delta =pa_simsb.delta= 0.0;
    pa_simsc.delta=3;
    pa_sis.alfa =pa_sis.gamma=pa_sir.gamma=pa_simsb.gamma=pa_simsc.gamma= 0.0;
    pa_sir.alfa=pa_sirs.alfa=pa_simsb.alfa=pa_simsc.alfa=0.03;
    pa_sirs.gamma = 0.02;
    pa_sis.N=pa_sir.N=pa_sirs.N=1;
    pa_simsb.N=pa_simsc.N=n;
    t_sis=t_sir=t_sirs=t_simsb =t_simsc= 0;
    iter_sis=iter_sir=iter_sirs=iter_simsb=iter_simsc=0;


    FILE *archivo_sis;
    FILE *archivo_sir;
    FILE *archivo_sirs;
    FILE *archivo_simsb;
    FILE *archivo_simsc;
    FILE *archivo_R_efectb;
    FILE *archivo_R_efectc;

    archivo_sis = fopen("results/fig1_1/datos_sis.txt", "w");
    archivo_sir=fopen("results/fig1_1/datos_sir.txt", "w");
    archivo_sirs=fopen("results/fig1_1/datos_sirs.txt", "w");
    archivo_simsb=fopen("results/fig1_1/datos_simsb.txt", "w");
    archivo_simsc=fopen("results/fig1_1/datos_simsc.txt", "w");
    archivo_R_efectb=fopen("results/fig1_1/datos_Refectb.txt", "w");
    archivo_R_efectc=fopen("results/fig1_1/datos_Refectc.txt", "w");


    if (archivo_sis == NULL || archivo_sir == NULL || archivo_sirs == NULL || archivo_simsb == NULL || archivo_simsc == NULL || archivo_R_efectb == NULL || archivo_R_efectc == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        return 1; 
    }

    fprintf(archivo_sis, "tiempo\tI(t)\n");
    fprintf(archivo_sir, "tiempo\tI(t)\n");
    fprintf(archivo_sirs, "tiempo\tI(t)\n");
    fprintf(archivo_simsb, "tiempo\t");
    fprintf(archivo_R_efectb, "tb\tRb\n");
    fprintf(archivo_R_efectc, "tb\tRc\n");

    for(i=0;i<n;i++){
        fprintf(archivo_simsb, "rho(%d)\t",i+1);
    }
    fprintf(archivo_simsb, "\n");

    fprintf(archivo_simsc, "tiempo\t");
    for(i=0;i<n;i++){
        fprintf(archivo_simsc, "rho(%d)\t",i+1);
    }
    fprintf(archivo_simsc, "\n");


    // Definimos las condiciones iniciales

    for(i=0;i<n;i++){
        rho_simsb[i]=0;
        mu_simsb[i]=pa_simsb.mu_0;
        rho_simsc[i]=0;
        mu_simsc[i]=pa_simsc.mu_0;

    }

    rho_sis[0]=rho_sir[0]=rho_sirs[0]=rho_simsb[0]=rho_simsc[0]=0.01;
    mu_sis[0]=mu_sir[0]=mu_sirs[0]=pa_sis.mu_0;
    I_simsb=I_simsc=0.01;

    // Defino la red lineal de genotipos
    /*
    // Primero obtenemos la matriz de adyacencia
    for (i = 0; i < n; i++) {
        for(j=0;j<n;j++){
            A[i][j] = 0;
        }
    } 
    for (int i = 0; i < (n - 1); i++) {
        A[i][i + 1] = 1.0;
        A[i + 1][i] = 1.0;
    } */

    // Primero obtenemos la matriz de adyacencia
    for(i=0; i<(n*n); i++){
        A_uni[i] = 0;
    }
    //Parte derecha diagonal
    for (i=n; i<(n*n); i = i+n){
        A_uni[i + i/n - 1] = 1.0;
    }
    //Parte izquierda diagonal
    for (i=0; i<(n*n)-n; i = i+n){
        if (i>0){
            A_uni[i + i/n +1] = 1.0;
        } else {
            A_uni[i+1] = 1.0;
        }
    }
    
    /*
    // Luego obtenemos la matriz de distancias

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            X[i][j] = abs(i - j);
        }
    }*/

    // Luego obtenemos la matriz de distancias

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            X_uni[i*n + j] = abs(i - j);
        }
    }

    
    // Hacemos el bucle en el que calculamos I(t) hasta alcanzar un estado estacionario

    // SIS(1.1.a)
    do{
        fprintf(archivo_sis, "%f\t%f\n", t_sis, rho_sis[0]);
        paso_rk4_sims(rho_sis,mu_sis,X_uni,A_uni,pa_sis);
        t_sis += dT;
        iter_sis++;
    } while (iter_sis < (200/dT));


    // SIR(1.1.a)
    do{
        fprintf(archivo_sir, "%f\t%f\n", t_sir, rho_sir[0]);
        paso_rk4_sims(rho_sir,mu_sir,X_uni,A_uni,pa_sir);
        t_sir += dT;
        iter_sir++;
    } while (iter_sir < (200/dT));

    //SIRS(1.1.a)
    do{
        fprintf(archivo_sirs, "%f\t%f\n", t_sirs, rho_sirs[0]);
        paso_rk4_sims(rho_sirs,mu_sirs,X_uni,A_uni,pa_sirs);
        t_sirs += dT;
        iter_sirs++;
    } while (iter_sirs < (200/dT));


    // SIMS sin inmunidad cruzada(1.1.b)
    do{
        R_efectivob=pa_simsb.beta*(1-I_simsb)/mu_simsb[10];
        fprintf(archivo_simsb, "%f\t", t_simsb);
        for(i=0;i<n;i++){
            fprintf(archivo_simsb, "%f\t", rho_simsb[i]);
        }
        fprintf(archivo_R_efectb, "%f\t%f",t_simsb,R_efectivob);
        fprintf(archivo_simsb, "\n");
        fprintf(archivo_R_efectb, "\n");

        I_simsb=0;
        paso_rk4_sims(rho_simsb,mu_simsb,X_uni,A_uni,pa_simsb);
        for(i=0;i<n;i++){
            I_simsb+=rho_simsb[i];
        }
        t_simsb += dT;
        iter_simsb++;
    } while (iter_simsb < (1000/dT));

    // SIMS con inmunidad cruzada (1.1.c)
    do{
        R_efectivoc=pa_simsc.beta*(1-I_simsc)/mu_simsc[10];
        fprintf(archivo_simsc, "%f\t", t_simsc);
        for(i=0;i<n;i++){
            fprintf(archivo_simsc, "%f\t", rho_simsc[i]);
        }

        fprintf(archivo_R_efectc, "%f\t%f", t_simsc,R_efectivoc);
        fprintf(archivo_simsc, "\n");
        fprintf(archivo_R_efectc, "\n");
        I_simsc=0;
        paso_rk4_sims(rho_simsc,mu_simsc,X_uni,A_uni,pa_simsc);
        for(i=0;i<n;i++){
            I_simsc+=rho_simsc[i];
        }
        t_simsc += dT;
        iter_simsc++;
    } while (iter_simsc < (1000/dT));


    fclose(archivo_sis);
    fclose(archivo_sir);
    fclose(archivo_sirs);
    fclose(archivo_simsb);
    fclose(archivo_simsc);

    return 0;
};