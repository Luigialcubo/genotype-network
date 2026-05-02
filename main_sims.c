#include "definiciones.h"
int main(){
    ParametrosSIMS pa_sis,pa_sir,pa_sirs;
    double iter_sis,iter_sir,iter_sirs,diff,t_sis,t_sir,t_sirs,I_sis,I_sir,I_sirs,I_viejo;
    double A[n][n],X[n][n];
    double rho_sis[n],mu_sis[n],rho_sir[n],mu_sir[n],rho_sirs[n],mu_sirs[n];
    int i,j;
    pa_sis.mu_0=pa_sir.mu_0=pa_sirs.mu_0 = 0.1;
    pa_sis.beta=pa_sir.beta=pa_sirs.beta = 0.3;
    pa_sis.Dx=pa_sir.Dx=pa_sirs.Dx = 0.0;
    pa_sis.delta=pa_sir.delta=pa_sirs.delta = 0.0;
    pa_sis.alfa =pa_sis.gamma=pa_sir.gamma= 0.0;
    pa_sir.alfa=pa_sirs.alfa=0.03;
    pa_sirs.gamma = 0.02;
    pa_sis.N=pa_sir.N=pa_sirs.N=1;
    t_sis=t_sir=t_sirs = 0;
    iter_sis=iter_sir=iter_sirs=0;


    FILE *archivo_sis;
    FILE *archivo_sir;
    FILE *archivo_sirs;

    archivo_sis = fopen("results/fig1_1/datos_sis.txt", "w");
    archivo_sir=fopen("results/fig1_1/datos_sir.txt", "w");
    archivo_sirs=fopen("results/fig1_1/datos_sirs.txt", "w");


    if (archivo_sis == NULL || archivo_sir == NULL || archivo_sirs == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        return 1; 
    }

    fprintf(archivo_sis, "tiempo\tI(t)\n");
    fprintf(archivo_sir, "tiempo\tI(t)\n");
    fprintf(archivo_sirs, "tiempo\tI(t)\n");

    // Definimos las condiciones iniciales

    rho_sis[0]=rho_sir[0]=rho_sirs[0]=0.01;
    mu_sis[0]=mu_sir[0]=mu_sirs[0]=pa_sis.mu_0;
    I_sis=I_sir=I_sirs=0.01;

    // Defino la red lineal de genotipos
    // Primero obtenemos la matriz de adyacencia
    for (i = 0; i < n; i++) {
        for(j=0;j<n;j++){
            A[i][j] = 0;
        }
    } 
    for (int i = 0; i < (n - 1); i++) {
        A[i][i + 1] = 1.0;
        A[i + 1][i] = 1.0;
    } 

    // Luego obtenemos la matriz de distancias

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            X[i][j] = abs(i - j);
        }
    }

    
    // Hacemos el bucle en el que calculamos I(t) hasta alcanzar un estado estacionario
    do{
        fprintf(archivo_sis, "%f\t%f\n", t_sis, I_sis);
        I_viejo = I_sis;
        paso_rk4_sims(rho_sis,mu_sis,X,A,pa_sis);
        I_sis=rho_sis[0];
        t_sis += dT;
        diff = fabs(I_sis - I_viejo);
        iter_sis++;
    } while (diff > Tolerance2 && iter_sis < MAX_ITER);

    do{
        fprintf(archivo_sir, "%f\t%f\n", t_sir, I_sir);
        I_viejo = I_sir;
        paso_rk4_sims(rho_sir,mu_sir,X,A,pa_sir);
        I_sir=rho_sir[0];
        t_sir += dT;
        diff = fabs(I_sir - I_viejo);
        iter_sir++;
    } while (diff > Tolerance2 && iter_sir < MAX_ITER);

    do{
        fprintf(archivo_sirs, "%f\t%f\n", t_sirs, I_sirs);
        I_viejo = I_sirs;
        I_sirs=0;
        paso_rk4_sims(rho_sirs,mu_sirs,X,A,pa_sirs);
        I_sirs=rho_sirs[0];
        t_sirs += dT;
        diff = fabs(I_sirs - I_viejo);
        iter_sirs++;
    } while (diff > Tolerance2 && iter_sirs < MAX_ITER);


    fclose(archivo_sis);
    fclose(archivo_sir);
    fclose(archivo_sirs);

    return 0;
};