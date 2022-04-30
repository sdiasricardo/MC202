#include <stdio.h>
#include "planejar.h"
#include <string.h>

int compara_data(Data data1, Data data2){
    if(data1.ano == data2.ano){
        if(data1.mes == data2.mes){
            if(data1.dia == data2.dia){
                return 0;
            }else if(data1.dia > data2.dia){
                return -1;
            } else{
                return 1;
            }
        }else if(data1.mes > data2.mes){
            return -1;
        }else{
            return 1;
        }
    }else if(data1.ano > data2.ano){
        return -1;
    }else{
        return 1;
    }
}

// 10/03/2021
// 15/04/2022

int diferenca_data(Data data1, Data data2){
    int soma = 0;
    while(data1.dia != data2.dia || data1.mes != data2.mes || data1.ano != data2.ano){
        data1.dia++;
        soma++;
        if(data1.mes == 1 || data1.mes == 3 || data1.mes == 5 || data1.mes == 7 || data1.mes == 8 || data1.mes == 10 || data1.mes == 12){
            if(data1.dia == 32){
                data1.dia = 1;
                data1.mes++;
            }
        }else{
            if(data1.dia == 31){
                data1.dia = 1;
                data1.mes++;
            }
        }
        if(data1.mes == 13){
            data1.ano++;
            data1.mes = 1;
        }
    }
    return soma;
}


int voos_viaveis(Voo voos[], int qtd_voos, par_voos par_voos_viaveis[], char aeroporto_partida[], int diferenca_data_desejada){
    int k, i, qtd_pares = 0;
    for(k=0; k<qtd_voos - 1; k++){
        for(i=k+1; i<qtd_voos; i++){
            if(compara_data(voos[k].data, voos[i].data) == 1){
                if(diferenca_data(voos[k].data, voos[i].data) + 1 >= diferenca_data_desejada){
                    if((strcmp(voos[k].origem, voos[i].destino) == 0) && (strcmp(voos[k].origem, aeroporto_partida) == 0) && (strcmp(voos[k].destino, voos[i].origem) == 0)){
                    par_voos_viaveis[qtd_pares].voo1 = voos[k];
                    par_voos_viaveis[qtd_pares].voo2 = voos[i];
                    par_voos_viaveis[qtd_pares].valor = voos[i].valor + voos[k].valor;
                    qtd_pares++;
                    }
                }
            }else if(compara_data(voos[k].data, voos[i].data) == -1){
                if(diferenca_data(voos[i].data, voos[k].data) + 1 >= diferenca_data_desejada){
                    if((strcmp(voos[i].origem, voos[k].destino) == 0) && (strcmp(voos[i].origem, aeroporto_partida) == 0) && (strcmp(voos[i].destino, voos[k].origem) == 0)){
                        par_voos_viaveis[qtd_pares].voo1 = voos[i];
                        par_voos_viaveis[qtd_pares].voo2 = voos[k];
                        par_voos_viaveis[qtd_pares].valor = voos[i].valor + voos[k].valor;
                        qtd_pares++;
                    }
                }
            }
        }
    }
    return qtd_pares;
}


void registrar(Voo voos[], int qtd_voos, int numero_voo, char origem[4], char destino[4], Data data, double valor){
    voos[qtd_voos].numero = numero_voo;
    strcpy(voos[qtd_voos].origem, origem);
    strcpy(voos[qtd_voos].destino, destino);
    voos[qtd_voos].data = data;
    voos[qtd_voos].valor = valor;
}


void cancelar(Voo voos[], int qtd_voos, int numero_cancelamento){
    int k, indice;
    for(k=0; k<qtd_voos; k++){
        if(voos[k].numero == numero_cancelamento){
            indice = k;
            break;
        }
    }
    
    for(k=indice; k<qtd_voos - 1; k++){
        voos[k].numero = voos[k + 1].numero;
        strcpy(voos[k].origem, voos[k + 1].origem);
        strcpy(voos[k].destino, voos[k + 1].destino);
        voos[k].data = voos[k + 1].data;
        voos[k].valor = voos[k + 1].valor;
    }
}


void alterar(Voo voos[], int qtd_voos, int numero_alteracao, double novo_valor){
    int k;
    for(k=0; k<qtd_voos; k++){
        if(voos[k].numero == numero_alteracao){
            voos[k].valor = novo_valor;
        }
    }
}


void planejar(Voo voos[], int qtd_voos, char aeroporto_partida[], Data data_inicio, Data data_fim){
    int k, i;
    
    
    for(k=0; k<qtd_voos; k++){
        if((compara_data(voos[k].data, data_inicio) == 1) || (compara_data(data_fim, voos[k].data) == 1)){
            cancelar(voos, qtd_voos, voos[k].numero);
            qtd_voos--;
            k--;
        }
    }
    
    par_voos par_voos_viaveis[9900];
    int qtd_pares_viaveis, indice_par_mais_barato = 0;

    qtd_pares_viaveis = voos_viaveis(voos, qtd_voos, par_voos_viaveis, aeroporto_partida, 4);

    
    for(i=1; i<qtd_pares_viaveis; i++){
        if(par_voos_viaveis[i].valor < par_voos_viaveis[indice_par_mais_barato].valor){
            indice_par_mais_barato = i;
        }
    }
    
    printf("%d\n", par_voos_viaveis[indice_par_mais_barato].voo1.numero);
    printf("%d", par_voos_viaveis[indice_par_mais_barato].voo2.numero);
}