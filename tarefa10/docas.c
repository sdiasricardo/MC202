#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct navio{
    char nome[21]; // Nome do navio
    char operacao[21]; // Se ele descarrega ou carrega
    char mercadoria[21]; // O que ele transporta
    int qtd; // Quanto de mercadoria ele possui
    int max; // Quantidade máxima de conteineres que ele pode carregar
} navio;


typedef struct doca{
    char **pilha; // Pilha de conteiners (vetor)
    int topo; // Indice do topo da fila (onde deve ser adicionado o próximo item)
    int tamanho; // Tamanho máximo comportado pela doca
} doca;


typedef struct fila_navios{
    navio **fila; // Fila de navios (vetor circular)
    int primeiro; // Indice do primeiro da fila
    int ultimo; // Indice do último (onde deve ser adicionado o próximo navio)
    int tamanho; // Tamanho atual da fila
    int n; // Tamanho máximo da fila (tamanho do vetor alocado);
    int recomeco; // Váriavel que identifica quantos navios voltaram a fila em sequência
} Navios;


Navios* cria_fila(Navios *navios, int n_navios){ // Criando a fila de navios
    navios->fila = malloc(n_navios*sizeof(navio*));
    navios->n = n_navios;
    navios->tamanho = 0;
    navios->ultimo = 0;
    navios->primeiro = -1;
    navios->recomeco = 0;
    return navios;
}


Navios *enqueue(Navios *navios, navio *navio){ // Função que enfileira um navio na posição [ultimo]
    navios->fila[navios->ultimo] = navio;
    navios->ultimo = (navios->ultimo + 1)%(navios->n);
    navios->tamanho++;
    return navios;
}

Navios *dequeue(Navios *navios){
    navios->primeiro = (navios->primeiro + 1) % navios->n;
    navios->tamanho--;
    return navios;
}


Navios * add_navio(char nome[21], char operacao[21], char mercadoria[21], int qtd, Navios *navios){ // Função que adiciona um navio
// ã fila pela primeira vez, com seus parâmetros setados conforme sua operação
    navio *novo;
    novo = malloc(sizeof(navio));
    strcpy(novo->nome, nome);
    strcpy(novo->operacao, operacao);
    strcpy(novo->mercadoria, mercadoria);
    novo->max = qtd;

    if(strcmp(novo->operacao, "descarrega") == 0){ // Caso em que o navio descarregará
        novo->qtd = qtd;
    }else{ // Caso em que o navio carregará
        novo->qtd = 0;
    }

    if(navios->primeiro == -1){ // Verificando se a fila se encontra vazia
        navios->primeiro = 0;
        navios->ultimo = 0;
        navios = enqueue(navios, novo);
    }else{
        navios = enqueue(navios, novo);
    }
    return navios;
}


doca **cria_docas(doca **docas, int n_docas, int capacidade){ // Funcão que cria e retorna um vetor de docas organizados por índices
    int i, j;
    for(i=0; i<n_docas; i++){ // Alocando cada uma das n docas no vetor de docas
        docas[i] = malloc(sizeof(doca));
        docas[i]->pilha = malloc(capacidade*sizeof(char*));
        for(j=0; j<capacidade; j++){ // Alocando o vetor que abrigará as strings
            docas[i]->pilha[j] = malloc(21*sizeof(char));
        }
        docas[i]->tamanho = capacidade;
        docas[i]->topo = 0;
    }
    return docas;
}


doca **push(doca **docas, int indice, char mercadoria[21]){ // Função que cria 
    strcpy(docas[indice]->pilha[docas[indice]->topo], mercadoria);
    docas[indice]->topo++;
    return docas;
}


doca **pop(doca **docas, int indice){
    docas[indice]->topo--;
    return docas;
}


void libera_docas(doca **docas, int n_docas){
    int i, j;
    for(i=0; i<n_docas; i++){
        for(j=0; j<docas[i]->tamanho; j++){
            free(docas[i]->pilha[j]);
        }
        free(docas[i]->pilha);
        free(docas[i]);
    }
    free(docas);
}


void libera_navios(Navios *navios){
    while(navios->tamanho != 0){
        free(navios->fila[navios->primeiro]);
        navios->primeiro = (navios->primeiro + 1) % navios->n;
        navios->tamanho--;
    }
    free(navios->fila);
    free(navios);
}


void descarregamento(Navios *navios, doca **docas, int n_docas, char mercadoria[21]){
    int i, contador = 0;
    navio *aux;
    for(i=0; i<n_docas; i++){ // Percorrendo cada uma das docas

        if(docas[i]->topo < docas[i]->tamanho){ // Caso a operação seja verdadeira, a doca pode receber um conteiner

            while(docas[i]->topo < docas[i]->tamanho && navios->fila[navios->primeiro]->qtd != 0){ // Adicionando um conteiner a doca enquanto ela não estiver cheia
                docas = push(docas, i, mercadoria);
                navios->fila[navios->primeiro]->qtd --;
                contador++;
            }

            //Imprimindo a mensagem de descarregamento
            printf("%s descarrega %s doca: %d conteineres: %d\n", navios->fila[navios->primeiro]->nome,
            navios->fila[navios->primeiro]->mercadoria, i, contador);

            if(navios->fila[navios->primeiro]->qtd == 0){ // Verificando se o navio foi esvaziado. Caso seja,
            // basta que ele seja retirado da fila
                aux = navios->fila[navios->primeiro];
                navios = dequeue(navios);
                navios->recomeco = 0; // Caso ele tenha descarregado, a sequência de reinserção foi quebrada, portanto recomeço volta a 0
                free(aux);
                return;
            }else{ // Do contrario, ele precisa ser reinserido no final da fila
                aux = navios->fila[navios->primeiro];
                navios = dequeue(navios);
                navios = enqueue(navios, aux);
                navios->recomeco = 0;// Caso ele tenha descarregado, a sequência de reinserção foi quebrada, portanto recomeço volta a 0
                return;
            }
        }
    }

    // Caso nenhuma doca tenha espaço para ser carregado, basta reinserir o navio ao fim da fila
    aux = navios->fila[navios->primeiro];
    navios = dequeue(navios);
    navios = enqueue(navios, aux);
    navios->recomeco++; // Caso ele não descarregue nada, ele deve voltar ao começo da fila, e portanto o contador de reinserção deve ser acrescido.
    return;
}


void carregamento(Navios *navios, doca **docas, int n_docas, char mercadoria[21]){
    int i, contador = 0;
    navio *aux;
    for(i=0; i<n_docas; i++){

        if(docas[i]->topo != 0){ // Caso o topo seja 0, a doca está vazia

            if(strcmp(mercadoria, docas[i]->pilha[docas[i]->topo - 1]) == 0){ // Verificando se alguma das docas
            // possui, no topo, a mercadoria desejada;

                while((strcmp(mercadoria, docas[i]->pilha[docas[i]->topo - 1]) == 0) 
                && (navios->fila[navios->primeiro]->qtd != navios->fila[navios->primeiro]->max)){ // Carregando o navio enquanto
                // a mercadoria do topo for a mercadoria que ele carregam e ele ainda não estiver cheio, e atualizando sua quantidade

                    docas = pop(docas, i);
                    navios->fila[navios->primeiro]->qtd++;
                    contador++;

                    if(docas[i]->topo == 0){ // Caso o topo se torne 0, a doca volta a ficar vazia
                        break;
                    }
                }

                //Imprimindo a mensagem de carregamento
                printf("%s carrega %s doca: %d conteineres: %d\n", navios->fila[navios->primeiro]->nome,
                navios->fila[navios->primeiro]->mercadoria, i, contador);

                if(navios->fila[navios->primeiro]->qtd == navios->fila[navios->primeiro]->max){ // Caso, ao fim da iteração, o navio tenha atingido
                // a quantidade máxima que ele pode carregar, basta remove-lo da fila
                    aux = navios->fila[navios->primeiro];
                    navios = dequeue(navios);
                    navios->recomeco = 0; // Caso ele tenha carregado, a sequência de reinserção foi quebrada, portanto recomeço volta a 0
                    free(aux);
                    return;
                }else{ // Do contrário, é necessário reinseri-lo ao fim da fila
                    aux = navios->fila[navios->primeiro];
                    navios = dequeue(navios);
                    navios = enqueue(navios, aux);
                    navios->recomeco = 0; // Caso ele tenha carregado, a sequência de reinserção foi quebrada, portanto recomeço volta a 0
                    return;
                }
                break;
            }
        }
    }

    // Caso nenhuma doca possua o item que o navio carrega, basta remove-lo da lista
    aux = navios->fila[navios->primeiro];
    navios = dequeue(navios);
    navios = enqueue(navios, aux);
    navios->recomeco++; // Caso ele não carregue nada, ele deve voltar ao começo da fila, e portanto o contador de reinserção deve ser acrescido.
    return;
}


int main(){
    int n_docas, capacidade, n_navios;
    scanf("%d", &n_docas);
    scanf("%d", &capacidade);
    scanf("%d", &n_navios);

    Navios *navios;
    navios = malloc(sizeof(Navios));
    navios = cria_fila(navios, n_navios); // Criando a fila de navios

    char navio[21], operacao[21], mercadoria[21];
    int i, qtd;
    for(i=0; i<n_navios; i++){ // Adicionando cada um dos navios e suas informações à fila de navios
        scanf("%s %s %s %d", navio, operacao, mercadoria, &qtd);
        navios = add_navio(navio, operacao, mercadoria, qtd, navios);
    }

    doca **docas;
    docas = malloc(n_docas*sizeof(doca*));
    docas = cria_docas(docas, n_docas, capacidade); // Criando o vetor de docas, com cada doca inicalmente vazio

    while(navios->tamanho != 0){
        if(strcmp(navios->fila[navios->primeiro]->operacao, "carrega") == 0){ // Verificando se é uma operação de carga ou descarga
            carregamento(navios, docas, n_docas, navios->fila[navios->primeiro]->mercadoria); 
        }else{
            descarregamento(navios, docas, n_docas, navios->fila[navios->primeiro]->mercadoria);
        }
        if(navios->recomeco == navios->tamanho && navios->tamanho != 0){ // Caso o recomeço seja igual ao tamanho da fila, temos que cada
        // um dos navios presente na fila ja entrou e saiu uma vez, ou seja, não há mais operações possíveis com os navios da fila.
            printf("ALERTA: impossivel esvaziar fila, restam %d navios.", navios->tamanho);
            break;
        }
    }

    libera_docas(docas, n_docas);
    libera_navios(navios);
}
