#include <stdio.h>
#include <stdlib.h>  //biblioteca para utilização da função "abs()"

void criar_listas(int n, int lista[]){ //Função que cria uma vetor com elementos indo de 0 a n.
    int i;
    for(i=0; i<n; i++){
        lista[i] = i + 1;
    }
}


int busca(int vetor[], int valor, int tamanho_vetor){ //Função que busca e retorna o valor do índice de um número dentro do vetor
    int i;
    for(i=0; i<tamanho_vetor; i++){
        if(vetor[i] == valor){
            return i;
        }
    }
    return -1;
}


void conversa(int a, int b, int fila[], int n){ //Função que modifica a fila caso ocorra um grupo de conversa
    // Identificando os índices iniciais e finais do grupo
    int indice_inicial = busca(fila, a, n);
    int indice_final = busca(fila, b, n);
    
    int fila_auxiliar[10000]; // Criando uma lista auxiliar, que contém apenas a as pessoas do grupo que irão conversar,
    // em sua ordem incial
    int k;
    for(k=0; k<=abs(indice_inicial - indice_final); k++){
        fila_auxiliar[k] = fila[indice_inicial + k];
    }

    int i; // Utilizando a lista auxiliar para realizar a troca de posições da fila definitiva, visto que o grupo será inserido
    // na posição inversa da lista auxiliar
    for(i=0; i < abs(indice_inicial - indice_final) + 1; i++){
        fila[indice_inicial + i] = fila_auxiliar[abs(indice_inicial - indice_final) - i];
    }
}


void espera(int a, int b, int fila[], int n){ // Função que modifica a lista caso alguém deseje esperar por outra pessoa
    // Identificando as índices da primeira pessoa (que vai esperar) e da última (que vai ser esperado)
    int primeiro_indice = busca(fila, a, n);
    int segundo_indice = busca(fila, b, n);
    
    // Como a pessoa que vai sair da fila entrará após a pessoa esperada, todos as pessoas (inclusive a pessoa esperada) entre eles
    // deverá avançar uma posição, ou seja, se ocupava incialmente a posição fila[k], agora ocupara a posição fila[k - 1]
    int i;
    for(i=0; i < segundo_indice - primeiro_indice; i ++){
        fila[primeiro_indice + i] = fila[primeiro_indice + i + 1];
    }
    fila[segundo_indice] = a; // Por fim, trocamos o valor da pessoa esperada pela que estava esperando, pois ela entrará imediatamente atrás
    // da pessoa esperada, e esta ocupa, agora, a posição fila[segundo_indice - 1]
}


int main(){
    int n; // Lendo o tamanho da fila e a criando
    scanf("%d", &n);
    int fila[10000];
    criar_listas(n, fila);

    int m; // Lendo a quantidade de entradas
    scanf("%d", &m);

    int i;
    for(i=0; i<m; i++){ // Lendo a quantidade m de entradas e executando as modificações na fila conforme o que ocorre nela. Processo
    // pode assumir dois valores: 1 (conversa de 2 ou mais pessoas) ou 2 (uma pessoa esperando outra).
        int processo, a, b;
        scanf("%d %d %d", &processo, &a, &b);
        if(processo == 1){
            conversa(a, b, fila, n);
        }else if(processo == 2){
            espera(a, b, fila, n);
        }
    }
    int k;
    for(k=0; k<n; k++){ // Por fim, imprimindo a fila e sua ordem
        printf("%d ", fila[k]);
    }
}