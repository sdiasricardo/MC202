#include <stdio.h>
#include "planejar.h"
#include <math.h>
#include <string.h>

int main(){
    char comando[15];
    Voo voos[100];
    int qtd_voos = 0;
    int numero;
    char origem[4], destino[4];
    Data data;
    double valor;
    Data data_inicio, data_fim;

    do
    {
        scanf("%s ", comando);
        if(strcmp(comando, "registrar") == 0){
            scanf("%d ", &numero);
            scanf("%s %s ", origem, destino);
            scanf("%d/%d/%d ", &data.dia, &data.mes, &data.ano);
            scanf("%lf ", &valor);
            registrar(voos, qtd_voos, numero, origem, destino, data, valor);
            qtd_voos++;
        }else if(strcmp(comando, "cancelar") == 0){
            scanf("%d ", &numero);
            cancelar(voos, qtd_voos, numero);
            qtd_voos--;
        }else if(strcmp(comando, "alterar") == 0){
            scanf("%d %lf ", &numero, &valor);
            alterar(voos, qtd_voos, numero, valor);
        }else if(strcmp(comando, "planejar") == 0){
            scanf("%s ", origem);
            scanf("%d/%d/%d %d/%d/%d", &data_inicio.dia, &data_inicio.mes,
            &data_inicio.ano, &data_fim.dia, &data_fim.mes, &data_fim.ano);
            planejar(voos, qtd_voos, origem, data_inicio, data_fim);
            printf("\n");
        }
    } while(strcmp(comando, "planejar") != 0);
}