#include "definiciones.h" 

int main(){
    int i,m,N;
    double mu_0,R_0,beta,beta_2,eta_[m],I_estrella1,I_estrella2,eta[m];
    mu_0 = 0.1;
    m=9;
    beta_2=0.3;
    R_0=0;

    for(i=0;i<m;i++){
        eta_[i]=eta[i]=1*pow(10,(-4+i));

    }

    FILE *f1;
    FILE *f2;
    
    f1 = fopen("results/fig1_2/datos_figura1_2a.txt", "w");
    f2 = fopen("results/fig1_2/datos_figura1_2b.txt", "w");

    if (f1 == NULL || f2 == NULL){
        printf("Error: No se pudo abrir el archivo.\n");
        return 1; 
    }

    // Fig 1_2a

    fprintf(f1, "R0");
    for(i=0;i<m;i++){
        fprintf(f1,"\t%.1e",eta_[i]);
    }
    fprintf(f1,"\n");


    while(R_0<8.0){
        fprintf(f1,"%f\t",R_0);

        for(i=0;i<m;i++){
            if(R_0<=1.0){
                I_estrella1=0;
            }
            else{
            beta=R_0*mu_0;
            I_estrella1=(beta-mu_0)/(beta+eta_[i]);
            }
            fprintf(f1,"%f\t",I_estrella1);
        }
        fprintf(f1,"\n");
        R_0+=0.01;
    }


    // Fig 1_2a

    fprintf(f2, "n");
    for(i=0;i<m;i++){
        fprintf(f2,"\t%.1e",eta[i]);
    }
    fprintf(f2,"\n");

    for(N=0;N<150;N++){
        fprintf(f2,"%d\t",N);

        for(i=0;i<m;i++){
            if(N==0){
                I_estrella2=0;
            }
            else{
            I_estrella2=(beta_2-mu_0)/(beta_2+eta[i]*(1+(N-1)*exp(-100.0))/N);
            }
            fprintf(f2,"%f\t",I_estrella2);
        }
        fprintf(f2,"\n");
    }


    fclose(f1);
    fclose(f2);
    return 0;
}



