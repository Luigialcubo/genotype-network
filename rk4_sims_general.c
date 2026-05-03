#include "definiciones.h"   // Para tener ParametrosSIMS, dT, etc.
#include <math.h>           // exp, fabs (si se usan)

/*
 * Función derivada – versión genérica (acepta cualquier número de cepas)
 * Parámetros:
 *   rho[]   : fracción de infectados por cepa (longitud p.N)
 *   mu[]    : tasa de recuperación por cepa (longitud p.N)
 *   X, A    : matrices de distancias genéticas y adyacencia (p.N x p.N)
 *   p       : parámetros del modelo SIMS (incluye el tamaño p.N)
 *   drho[]  : derivadas de rho (salida)
 *   dmu[]   : derivadas de mu  (salida)
 */
void derivada_general(double rho[], double mu[], double **X, double **A,
              ParametrosSIMS p, double drho[], double dmu[]) {
    double S, mutacion, lij, k[p.N], inmunidad, cruzada;
    int i, j;

    // Fracción de susceptibles
    S = 1.0;
    for (i = 0; i < p.N; i++) {
        S -= rho[i];
    }

    // Grado de cada nodo
    for (i = 0; i < p.N; i++) {
        k[i] = 0.0;
    }
    for (i = 0; i < p.N; i++) {
        for (j = 0; j < p.N; j++) {
            k[i] += A[i][j];      // A[i][j] funciona con double **
        }
    }

    // Derivadas de rho y mu
    for (i = 0; i < p.N; i++) {
        mutacion = 0.0;
        inmunidad = 0.0;
        for (j = 0; j < p.N; j++) {
            // Laplaciano normalizado L_ij = δ_ij - A_ij / k_j
            lij = 0.0;
            if (i == j) {
                lij = 1.0;
            }
            if (k[j] > 0) {
                lij -= A[i][j] / k[j];
            }

            // Inmunidad cruzada
            if (p.delta == 0.0) {
                if (i == j) {
                    cruzada = 1.0;
                } else {
                    cruzada = 0.0;
                }
            } else {
                cruzada = exp(-X[i][j] / p.delta);
            }

            mutacion  += rho[j] * lij;
            inmunidad += rho[j] * cruzada;
        }
        drho[i] = p.beta * rho[i] * S - mu[i] * rho[i] - p.Dx * mutacion;
        dmu[i]  = p.alfa * inmunidad - p.gamma * (mu[i] - p.mu_0);
    }
}

/*
 * Función paso_rk4_sims – versión genérica
 * Realiza un paso de Runge-Kutta de orden 4.
 * Los argumentos son los mismos que en derivada, más el paso temporal dT (macro).
 */
void paso_rk4_sims_general(double rho[], double mu[], double **X, double **A,
                   ParametrosSIMS p) {
    double k1_rho[p.N], k2_rho[p.N], k3_rho[p.N], k4_rho[p.N];
    double k1_mu[p.N], k2_mu[p.N], k3_mu[p.N], k4_mu[p.N];
    double rho_aux[p.N], mu_aux[p.N];
    int i;

    // --- k1 ---
    derivada_general(rho, mu, X, A, p, k1_rho, k1_mu);

    // --- k2 ---
    for (i = 0; i < p.N; i++) {
        rho_aux[i] = rho[i] + 0.5 * k1_rho[i] * dT;
        mu_aux[i]  = mu[i]  + 0.5 * k1_mu[i]  * dT;
    }
    derivada_general(rho_aux, mu_aux, X, A, p, k2_rho, k2_mu);

    // --- k3 ---
    for (i = 0; i < p.N; i++) {
        rho_aux[i] = rho[i] + 0.5 * k2_rho[i] * dT;
        mu_aux[i]  = mu[i]  + 0.5 * k2_mu[i]  * dT;
    }
    derivada_general(rho_aux, mu_aux, X, A, p, k3_rho, k3_mu);

    // --- k4 ---
    for (i = 0; i < p.N; i++) {
        rho_aux[i] = rho[i] + k3_rho[i] * dT;
        mu_aux[i]  = mu[i]  + k3_mu[i]  * dT;
    }
    derivada_general(rho_aux, mu_aux, X, A, p, k4_rho, k4_mu);

    // --- Actualización final ---
    for (i = 0; i < p.N; i++) {
        rho[i] += dT * (k1_rho[i] + 2.0 * k2_rho[i] + 2.0 * k3_rho[i] + k4_rho[i]) / 6.0;
        mu[i]  += dT * (k1_mu[i]  + 2.0 * k2_mu[i]  + 2.0 * k3_mu[i]  + k4_mu[i])  / 6.0;
    }
}