#include "definiciones.h"

int main(){
    ParametrosSIS parametros;
    double beta,rho,rho_viejo,diff,t;
    int iter;
    parametros.mu= 0.1;
    double R0;

    FILE *archivo;
    FILE *archivo2;
    FILE *archivo3;

    archivo = fopen(resultados_beta, "w");
    archivo2 = fopen(rho_tiempo, "w");
    archivo3 = fopen(rho_R0, "w");

    if (archivo == NULL || archivo2 == NULL || archivo3 == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        return 1; 
    }


    fprintf(archivo, "beta\trho_estacionario\n");
    fprintf(archivo2, "tiempo\trho\n");
    fprintf(archivo3, "R0\trho\n");
    


    for(beta; beta <= 1; beta += 0.01){
        parametros.beta = beta;
        rho = 0.01;
        iter = 0;
        t = 0.0;
        R0 = parametros.beta/parametros.mu;

        //Queremos que deje de actualizar rho cuando la diferencia de la actual y la antigua sea menor que la tolerancia.
        do{
            if(fabs(beta-0.3) <= 1e-9){
                fprintf(archivo2, "%f\t%f\n", t, rho);
            }
            rho_viejo = rho;
            rho = paso_rk4(rho, parametros);

            t += dT;
            diff = fabs(rho - rho_viejo);
            iter++;
        } while (diff > Tolerance && iter < MAX_ITER);

        fprintf(archivo, "%f\t%f\n", parametros.beta, rho);
        fprintf(archivo3, "%f\t%f\n", R0, rho);
        printf("Simulación beta = %.2f -> rho = %f\n", parametros.beta, rho);
    }

    fclose(archivo);
    fclose(archivo2);
    fclose(archivo3);
    printf("Simulación terminada. Datos guardados en %s, en %s y en %s \n", resultados_beta, rho_tiempo, rho_R0);

    return 0;
};