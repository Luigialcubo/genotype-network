#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define dT 0.01
#define Tolerance 1e-8
#define Tolerance2 1e-18
#define MAX_ITER 1000000
#define n 20

#define resultados_beta "results/sis_txt/datos_sis.txt"
#define rho_tiempo "results/sis_txt/rho_tiempo.txt"
#define rho_R0 "results/sis_txt/rho_R0.txt"

// Parametros sistenma SIS
typedef struct SIS
{
    double beta; //Tasa transmision o infectividad
    double mu; //Tasa recuperacion (mu_0 = 0.1)
} ParametrosSIS;

// Parametros sistema SIMS
typedef struct SIMS
{
    double beta; //Tasa transmision o infectividad
    double mu_0; //Tasa recuperacion basal
    double Dx; // Tasa de mutacion 
    double alfa; // Tasa de inmunidad adquirida 
    double delta; // longitud de inmunidad cruzada característica 
    double gamma; // Tasa de disminucion de la inmunidad
    int N; // Numero de cepas 
    
} ParametrosSIMS;

//Funciones rk4_sis.c
double derivada_sis(double rho, ParametrosSIS p);
double paso_rk4(double rho_actual, ParametrosSIS p);

// Funcones rk4_sims.c
void derivada(double rho[],double mu[],double X_uni[],double A_uni[],ParametrosSIMS p,double drho[],double dmu[]);
void paso_rk4_sims(double rho[],double mu[],double X_uni[],double A_uni[],ParametrosSIMS p);
void cargar_red(const char *nombre_archivo, int N, double *A_uni, double *X_uni);

//Funciones main.c
void simulacion(double beta_inicio, double beta_final, double incremento);
    
