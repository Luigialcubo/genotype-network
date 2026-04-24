#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define dT 0.01
#define Tolerance 1e-8
#define MAX_ITER 1000000

#define resultados_beta "resultados/datos_sis.txt"

// Parametro sistenma SIS
typedef struct SIS
{
    double beta; //Tasa transmision o infectividad
    double mu; //Tasa recuperacion (mu_0 = 0.1)
} ParametrosSIS;

//Funciones rk4_sis.c
double derivada_sis(double rho, ParametrosSIS p);
double paso_rk4(double rho_actual, ParametrosSIS p);

//Funciones main.c
void simulacion(double beta_inicio, double beta_final, double incremento);
