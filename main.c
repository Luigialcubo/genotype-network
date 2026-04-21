#include "definiciones.h"

int main(){
    ParametrosSIS parametros;
    parametros.mu = 0.1;

    double beta = 0.0;

    double rho, rho_viejo, diff;
    double t;
    int iter;

    FILE *archivo;
    archivo = fopen(resultados_beta, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        return 1; 
    }

    fprintf(archivo, "beta\trho_estacionario\n");

    for(beta; beta <= 1; beta += 0.01){
        parametros.beta = beta;
        rho = 0.01;
        iter = 0;
        t = 0.0;

        //Queremos que deje de actualizar rho cuando la diferencia de la actual y la antigua sea menor que la tolerancia.
        do{
            rho_viejo = rho;
            rho = paso_rk4(rho, parametros);

            t += dT;
            diff = fabs(rho - rho_viejo);
            iter++;
        } while (diff > Tolerance && iter < MAX_ITER);

        fprintf(archivo, "%f\t%f\n", parametros.beta, rho);
        printf("Simulación beta = %.2f -> rho = %f\n", parametros.beta, rho);
    }

    fclose(archivo);
    printf("Simulación terminada. Datos guardados en %s\n", resultados_beta);

    return 0;
};