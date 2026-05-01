#include "definiciones.h"
int main(){
    ParametrosSIMS parametros;
    double iter,diff,t,I,I_viejo;
    double rho[n],mu[n],A[n][n],X[n][n];
    int i,j;
    parametros.mu_0 = 0.1;
    parametros.beta = 0.3;
    parametros.alfa = 0.03;
    parametros.gamma = 0.0;
    parametros.Dx = 1e-5;
    parametros.delta = 3.0;
    t = 0;
    iter=0;


    FILE *archivo;
    archivo = fopen("datos_sims.txt", "w");


    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        return 1; 
    }

    fprintf(archivo, "tiempo\tI(t)\n");

    // Definimos las condiciones iniciales
    for (i = 0; i < n; i++) {
        rho[i] = 0.0;
        mu[i] = parametros.mu_0;
    }
    rho[0]=0.01;
    I=0.01;


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
        fprintf(archivo, "%f\t%f\n", t, I);
        I_viejo = I;
        paso_rk4_sims(rho,mu,X,A,parametros);
        for(i=0;i<n;i++){
            I+=rho[i];
        }
        t += dT;
        diff = fabs(I - I_viejo);
        iter++;
    } while (diff > Tolerance && iter < MAX_ITER);

    fclose(archivo);

    return 0;
};