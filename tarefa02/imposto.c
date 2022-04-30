#include <stdio.h>
#define max 1000

void le_matriz(double m[][13], int n){ // Função que adiciona valores a uma matriz n x 13. Os 12 primeiros elementos de uma
    // linha da matriz representam os valores imputados pelo usuario, e o último é a soma dos 12 primeiros.
    int i, j;
    double soma;
    for(i=0; i < n; i++){
        soma = 0;
        for(j=0; j < 12; j++){
            scanf("%lf", &m[i][j]);
            soma += m[i][j];
        }
        m[i][12] = soma;
    }
}


void calcula_imposto(double entrada[][13], double saida[][15], int n){// Função que calcula os impostos, organizados em uma matriz
    // de enetrada, e os atribui a outra matriz de saída, sendo os 12 primeiros elementos de uma linha da matriz de saída os impostos
    // retidos na fonte, o decimo terceiro elementos a soma desses 12 primeiros, o décimo quarto o ajuste anual, e o décimo quinto a
    // diferença do décimo quarto e décimo terceiro.
    int i, j;
    double soma_retidos;
    for(i=0; i < n; i++){
        soma_retidos = 0;
        for(j=0; j < 12; j++){ // Iniciando o cálculo dos valores retidos na fonte de cada funcionario
            saida[i][j] = 0;
            if(1499.15 <= entrada[i][j] && entrada[i][j] < 2246.75)
                saida[i][j] = (entrada[i][j] - 1499.15)*0.075;

            else if(2246.75 <= entrada[i][j] && entrada[i][j] < 2995.7)
                saida[i][j] = 747.6*0.075 + (entrada[i][j] - 2246.75)*0.15;

            else if(2995.7 <= entrada[i][j] && entrada[i][j] < 3743.19)
                saida[i][j] = 747.6*0.075 + 748.95*0.15 + (entrada[i][j] - 2995.7)*0.225;

            else if(3743.19 <= entrada[i][j])
                saida[i][j] = 747.6*0.075 + 748.95*0.15 + 747.49*0.225 + (entrada[i][j] - 3743.19)*0.275;

            soma_retidos += saida[i][j];
        }
    
        saida[i][12] = soma_retidos; // Atualizando na matriz de saída qual será o valor total dos impostos retidos

        saida[i][13] = 0; // Calculando e atualizando na matriz de saída qual será o valor do ajuste anual
        if(entrada[i][12] >= 18000 && entrada[i][12] < 26400){
            saida[i][13] = (entrada[i][12] - 18000)*0.075;
        } else if(entrada[i][12] >= 26400 && entrada[i][12] < 36000){
            saida[i][13] = 8400*0.075 + (entrada[i][12] - 26400)*0.15;
        }else if(entrada[i][12] >= 36000 && entrada[i][12] < 44000){
            saida[i][13] = 8400*0.075 + 9600*0.15 + (entrada[i][12] - 36000)*0.225;
        }else if(entrada[i][12] >= 44000){
            saida[i][13] = 8400*0.075 + 9600*0.15 + 8400*0.225 + (entrada[i][12] - 44400)*0.275;
        }
        
        saida[i][14] = saida[i][13] - saida[i][12]; // Diferença do ajuste anual e do valor retido
    }
}


void imprime_matriz(double M[][15], int n){
    int i, j;
    for(i=0; i<n; i++){
        for(j=0; j<15; j++){
            if(j <14)
                printf("%.2lf\t", M[i][j]);
            else
                printf("%.2lf\n", M[i][j]);
        }
    }
}

int main(){
    int n;
    scanf("%d", &n);

    double rendimentos[max][13], impostos[max][15];
    le_matriz(rendimentos, n);

    
    int i;
    double abate;
    for(i=0; i < n; i++){
        scanf("%lf", &abate);
        rendimentos[i][12] -= abate;
    }

    calcula_imposto(rendimentos, impostos, n);
    
    printf("Jan Fev Mar Abr Mai Jun Jul Ago Set Out Nov Dez Retido Devido Ajuste\n");
    imprime_matriz(impostos, n);

}