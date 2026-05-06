#include "definiciones.h" 

int main(){
    int i,m;
    double mu_0,R_0,beta,eta_[m],I_estrella;
    mu_0 = 1.0;
    m=9;
    R_0=0;

    for(i=0;i<m;i++){
        eta_[i]=1*pow(10,(-4+i));
    }

    FILE *f1;
    
    f1 = fopen("results/fig1_2/datos_figura1_2a.txt", "w");

    if (f1 == NULL){
        printf("Error: No se pudo abrir el archivo.\n");
        return 1; 
    }

    fprintf(f1, "R0");
    for(i=0;i<m;i++){
        fprintf(f1,"\t%.1e",eta_[i]);
    }
    fprintf(f1,"\n");

    while(R_0<8.0){
        fprintf(f1,"%f\t",R_0);

        for(i=0;i<m;i++){
            if(R_0<=1.0){
                I_estrella=0;
            }
            else{
            beta=R_0*mu_0;
            I_estrella=(beta-mu_0)/(beta+eta_[i]);
            }
            fprintf(f1,"%f\t",I_estrella);
        }
        fprintf(f1,"\n");
        R_0+=0.01;
    }

    fclose(f1);
    return 0;
}



