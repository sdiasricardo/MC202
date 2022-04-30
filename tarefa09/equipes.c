#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

typedef struct no{
    int altura;
    struct no *ant;
    struct no *prox;
} no;

typedef struct no* p_no;

int par(int valor){//Função que retorna 1 caso o valor passado seja par e 0 se não for.
    if(valor%2 == 0){
        return 1;
    }
    return 0;
}

p_no criar_lista(int h){ // Função que, dada uma altura "h", cria uma lista duplamente ligada com essa altura e 
    // retorna seu endereço. O primeiro item da lista é a cabeça, que apenas aponta para o primeiro dado.
    p_no cabeca, item;

    cabeca = malloc(sizeof(no));
    item = malloc(sizeof(no));
    cabeca->prox = item;
    
    item->altura = h;
    item->prox = item;
    item->ant = item;

    return cabeca;
}


p_no add_pessoa(p_no lista, int h, p_no *end_inicio, p_no *end_fim){ // Função que, dada uma altura "h", adiciona uma pessoa
    // à lista de pessoas a serem sorteadas, sempre no final da fila.
    p_no item;

    item = malloc(sizeof(no));
    item->altura = h;

    (*end_fim)->prox = item;
    (*end_inicio)->ant = item;
    item->prox = (*end_inicio);
    item->ant = (*end_fim);
    (*end_fim) = item;
    return lista;
}


p_no seleciona_pessoa(p_no lista, p_no *end_inicio, p_no *end_fim, int posicao, int *vetor, int m, int id){ 
    //Função que seleciona alguem da lista e adiciona ela ao vetor dos times. "posicao" é o parâmetro que indica em qual posiçaõ ele deve 
    // ser inserido no vetor em questão. Id recebe 0, caso deva contar para a esquerda, e 1 caso deva contar para a direita
    p_no contador = (*end_inicio);
    int i;

    if(id == 0){
        for(i=0; i<m-1; i++){
            contador = contador->ant;
        }
    }else{
        for(i=0; i<m-1; i++){
            contador = contador->prox;
        }
    }

    if(contador == lista->prox){ // Caso a pessoa selecionado (que será retirada da lista ligada) for o primeiro da lista,
    // O ponteiro que guarda o início da fila deve ser alterado.
        lista->prox = contador->prox;
        (contador->ant)->prox = contador->prox;
        (contador->prox)->ant = contador->ant;
        (*end_inicio) = contador->prox;
        vetor[posicao] = contador->altura;
        free(contador);
        return lista;

    }else if(contador == (*end_fim)){// Caso a pessoa selecionado (que será retirada da lista ligada) for o último da lista,
    // O ponteiro que guarda o fim da fila deve ser alterado.
        (contador->ant)->prox = contador->prox;
        (contador->prox)->ant = contador->ant;
        (*end_fim) = contador->ant;
        vetor[posicao] = contador->altura;
        free(contador);
        return lista;
    }else{// Do contrário, basta remover o integrante sorteado, rearranjado os ponteiros prox e ant da pessoa imediatamente
    // antes e imediatamente depois, respectivamente.
        (contador->ant)->prox = contador->prox;
        (contador->prox)->ant = contador->ant;
        vetor[posicao] = contador->altura;
        free(contador);
        return lista;
    }
}


void imprime_vetor(int *vetor, int tamanho){// Função que, dado um vetor e seu tamanho, o imprime e o libera.
    int i;
    for(i=0; i<tamanho; i++){
        if(i==tamanho-1){
            printf("%d\n", vetor[i]);
            free(vetor);
        }else{
            printf("%d ", vetor[i]);
        }
    }
}


void organiza_imprime_vetor(int *vetor, int tamanho){// Função que organiza as alturas de um vetor em ordem crescente, utilizando
    // BubbleSort, e chama a função que o imprime ao fim da organização.
    int i, j;

    int aux;
    for(i=0; i<tamanho; i++){
        for(j=(tamanho - 1); j>i; j--){
            if(vetor[j] < vetor[j-1]){
                aux = vetor[j];
                vetor[j] = vetor[j-1];
                vetor[j-1] = aux;
            }
    
        }
    }
    imprime_vetor(vetor, tamanho);

}


int main(){
    int i, m, altura;
    p_no lista, inicio, fim;
    p_no *end_fim, *end_inicio; // Ponteiros que guardarão o endereço da variável que guarda o endereço do inicio e fim da lista,
    // para possibilitar a passagem por referência e sua alteração dentro de funções.

    scanf("%d", &m); // Lendo o valor m de participantes da seleção
    for(i=0; i<m; i++){
        if(i != m-1){
            scanf("%d ", &altura);
        }else{
            scanf("%d", &altura);
        }
        if(i==0){ // i==0 indica que é o primeiro elemento sendo adicionado, portanto a lista precisa ser criada primeiramente.
            lista = criar_lista(altura);
            inicio = lista->prox;
            fim = lista->prox;
            end_fim = &fim;
            end_inicio = &inicio;
        }else{ // Do contrário, basta adicionar o elemento à lista ja criada
            lista = add_pessoa(lista, altura, end_inicio, end_fim);
        }
    }


    int *v1, *v2, tam_v1 = 0, tam_v2 = 0; // Declarando os vetores e seus tamanhos que receberão as alturas de cada equipe.
    if(par(m)){ // Caso o número de integrantes seja par, cada vetor tera o mesmo tamanho m/2
        v1 = malloc((m/2)*sizeof(int));
        v2 = malloc((m/2)*sizeof(int));
    }else{ // Do contrário, o primeiro vetor tera 1 posição a mais do que o segundo.
        v1 = malloc(((m/2)+1)*sizeof(int));
        v2 = malloc((m/2)*sizeof(int));
    }

    int valores;
     for(i=0; i<m; i++){ // Lendo os valores sorteados nos dados, para realizar a seleção de cada time.
        if(i != m-1){
            scanf("%d ", &valores);
        }else{
            scanf("%d", &valores);
        }
        if(par(i)){ // Caso i seja par, temos que a contagem deve ser realizada para a esquerda, e selecionado deve ser adicionado
        // no time 1, representado por v1.
            lista = seleciona_pessoa(lista, end_inicio, end_fim, tam_v1, v1, valores, 0);
            tam_v1++;
        }else{ // Do contrário, a contagem é feita para a direita e o jogador será adicionado no time 2, representado por v2.
            lista = seleciona_pessoa(lista, end_inicio, end_fim, tam_v2, v2, valores, 1);
            tam_v2++;
        }

    }
    free(lista);
    organiza_imprime_vetor(v1, tam_v1);
    organiza_imprime_vetor(v2, tam_v2);
}