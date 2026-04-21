#include "definiciones.h"

//Esta funcion se encarga de hacer la derivada de rho en funcion del tiempo para calcular las k
double derivada_sis(double rho, ParametrosSIS p){
    return p.beta*rho*(1.0-rho) - p.mu*rho;
};

//Esta funcion se encarga de obtener una rho nueva a partir de la del anterior paso
double paso_rk4(double rho_actual, ParametrosSIS p){
    double k1, k2, k3, k4;
    double rho;

    k1 = derivada_sis(rho_actual, p);
    k2 = derivada_sis(rho_actual + (dT/2)*k1, p);
    k3 = derivada_sis(rho_actual + (dT/2)*k2, p);
    k4 = derivada_sis(rho_actual + dT*k3, p);

    rho = rho_actual + (dT/6)*(k1 + 2*k2 + 2*k3 + k4);

    return rho;
};