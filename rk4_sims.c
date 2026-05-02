#include "definiciones.h"

//Esta funcion se encarga de hacer la derivada de rho_i  y mu_i en funcion del tiempo para calcular las k
void derivada(double rho[],double mu[],double X[][n],double A[][n],ParametrosSIMS p,double drho[],double dmu[]){
    double S,mutacion,lij,k[p.N],inmunidad,cruzada;
    int i,j;
    S=1.0;
    mutacion=lij=inmunidad=cruzada=0.0;

    // Obtengo el número de susceptibles 
    for(i=0;i<p.N;i++){
        S=S-rho[i];
    }

    // Obtengo el grado de cada nodo de la red de genotipos 
    for(i=0;i<p.N;i++){
        k[i]=0;
    }
    for(i=0;i<p.N;i++){
        for(j=0;j<p.N;j++){
            k[i]+=A[i][j];
        }
    }
    // calculo la derivada de rho y de mu
    for(i=0;i<p.N;i++){
        for(j=0;j<p.N;j++){
            if(i==j){
                lij=1.0;
            }
            if(k[i]>0){
                lij=lij-A[i][j]/k[i];
            }
            
            if(p.delta==0){
                if(i==j){
                    cruzada=1;
                }
                else{
                    cruzada=0;
                }
            }
            else{
                cruzada=exp(-X[i][j]/p.delta);
            }
            
            mutacion+=rho[j]*lij;
            inmunidad+=rho[j]*cruzada;

        }
        drho[i]=p.beta*rho[i]*S-mu[i]*rho[i]-p.Dx*mutacion;
        dmu[i]=p.alfa*inmunidad-p.gamma*(mu[i]-p.mu_0);
    }
};



//Esta funcion se encarga de obtener una nuevo rho y mu a partir de la anterior funcion
void paso_rk4_sims(double rho[],double mu[],double X[][n],double A[][n],ParametrosSIMS p){
    double k1_rho[p.N], k2_rho[p.N], k3_rho[p.N], k4_rho[p.N];
    double k1_mu[p.N], k2_mu[p.N], k3_mu[p.N], k4_mu[p.N];
    double rho_aux[p.N],mu_aux[p.N];
    int i;

    // Primero calculo el k1,k2,k3 y k4 para rho y mu de cada cepa
    //k1
    derivada(rho,mu,X,A,p,k1_rho,k1_mu);

    //k2
    for(i=0;i<p.N;i++){
        rho_aux[i]=rho[i]+0.5*k1_rho[i]*dT;
        mu_aux[i]=mu[i]+0.5*k1_mu[i]*dT;
    }
    derivada(rho_aux,mu_aux,X,A,p,k2_rho,k2_mu);

    //k3
    for(i=0;i<p.N;i++){
        rho_aux[i]=rho[i]+0.5*k2_rho[i]*dT;
        mu_aux[i]=mu[i]+0.5*k2_mu[i]*dT;
    }
    derivada(rho_aux,mu_aux,X,A,p,k3_rho,k3_mu);

    //k4
    for(i=0;i<p.N;i++){
        rho_aux[i]=rho[i]+k3_rho[i]*dT;
        mu_aux[i]=mu[i]+k3_mu[i]*dT;
    }
    derivada(rho_aux,mu_aux,X,A,p,k4_rho,k4_mu);

    // Luego calculo el rho y new en el nuevo paso temporal (t+dt)
    for(i=0;i<p.N;i++){
        rho[i]+=dT*(k1_rho[i]+2*k2_rho[i]+2*k3_rho[i]+k4_rho[i])/6;
        mu[i]+=dT*(k1_mu[i]+2*k2_mu[i]+2*k3_mu[i]+k4_mu[i])/6;
    }
};