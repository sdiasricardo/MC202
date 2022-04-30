#include <stdio.h>
#include <stdlib.h>
#include "minecraft.h"


int **calcularAltitudes(int m, int n, int seed){
    int i, j, **matriz;
    matriz = malloc(m * (sizeof(int*)));
    for(i=0; i<m; i++){
        matriz[i] = malloc(n *(sizeof(int)));
        for(j=0; j<n; j++){
            matriz[i][j] = ((seed * (202 + i + j)) + 12345 + i + j) % 256;
        }
    }
    return matriz;
}


int calcula_bloco(int seed, int x, int y, int z){
        return ((seed * (202 + x + y + z)) + x + y + z) % 33;
}


Bloco ***criarMundo(int m, int n, int **altitudes, int seed){
    int i, j, k;
    Bloco ***mundo;

    mundo = malloc(m * sizeof(Bloco **));
    for(i=0; i<m; i++){
        mundo[i] = malloc(n * sizeof(Bloco *));

        for(j=0; j<n; j++){
            mundo[i][j] = malloc(256 * sizeof(Bloco));

            for(k=0; k<256; k++){
                mundo[i][j][k].valor = calcula_bloco(seed, i, j, k);
                
            }
        }
    }

    return mundo;
}


void explorarMundo(Bloco ***mundo, int m, int n, int **altitudes, double tempoPorBloco,
    int *qtdDiamante, int *qtdOuro, int *qtdFerro, int *qtdBlocos, double *tempo_total){
    int i, j, k;

    for(i=0; i<m; i++){

        for(j=0; j<n; j++){

            for(k=0; k<=altitudes[i][j]; k++){
                if(mundo[i][j][k].valor < 21){
                    *qtdBlocos = *qtdBlocos + 1;
                    if(mundo[i][j][k].valor == 0){
                        *qtdDiamante = *qtdDiamante + 1;
                    }else if(mundo[i][j][k].valor == 1 || mundo[i][j][k].valor == 2){
                        *qtdOuro = *qtdOuro + 1;
                    }else if(mundo[i][j][k].valor == 3 || mundo[i][j][k].valor == 3 || mundo[i][j][k].valor == 4
                    || mundo[i][j][k].valor == 5){
                        *qtdFerro = *qtdFerro + 1;
                    }
                }
            }
            free(mundo[i][j]);
        }
        free(mundo[i]);
        free(altitudes[i]);
    }
    free(mundo);
    free(altitudes);


    *tempo_total = *qtdBlocos * tempoPorBloco;
}


int main(){
    int m, n, seed;
    double tempo_por_bloco;

    scanf("%d %d", &m, &n);
    scanf("%d", &seed);
    scanf("%lf", &tempo_por_bloco);

    int **altitudes;
    altitudes = calcularAltitudes(m, n, seed);

    Bloco ***mundo;
    mundo = criarMundo(m, n, altitudes, seed);

    int *qtd_diamante, *qtd_ouro, *qtd_ferro, *qtd_blocos;
    qtd_diamante = malloc(sizeof(int));
    qtd_ouro = malloc(sizeof(int));
    qtd_ferro = malloc(sizeof(int));
    qtd_blocos = malloc(sizeof(int));
    *qtd_diamante = *qtd_ouro = *qtd_ferro = *qtd_blocos = 0;

    double *tempo_total;
    tempo_total = malloc(sizeof(double));
    *tempo_total = 0;

    explorarMundo(mundo, m, n, altitudes, tempo_por_bloco, qtd_diamante, qtd_ouro, qtd_ferro, qtd_blocos, tempo_total);

    printf("Total de Blocos: %d\n", *qtd_blocos);
    printf("Tempo total: %.2lfs\n", *tempo_total);
    printf("Diamantes: %d\n", *qtd_diamante);
    printf("Ouros: %d\n", *qtd_ouro);
    printf("Ferros: %d\n", *qtd_ferro);
    
    free(qtd_diamante);
    free(qtd_ouro);
    free(qtd_ferro);
    free(qtd_blocos);
    free(tempo_total);

}