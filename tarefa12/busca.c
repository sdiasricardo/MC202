#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no { // Definindo os nós da árvore
    char nome[21];
    int codigo;
    double valor;
    struct no *esq, *dir;
} no;

typedef struct no * p_no;


typedef struct no_lista{ // Definindo os nós das listas ligadas
    p_no no;
    struct no_lista *ant, *prox;
} no_lista;

typedef struct no_lista * p_no_l;


typedef struct lista{ // Definindo a lista
    p_no_l prim;
    p_no_l ult;
} lista;


typedef struct pesquisa{ // Definindo a estrutura dos produtos que serão pesquisados
    int codigo;
    double valor;
}pesquisa;


p_no cria_arvore(double valor, char nome[20], p_no esq, p_no dir, int codigo){ // Função que cria um árvore binária
    p_no novo = malloc(sizeof(no));
    strcpy(novo->nome, nome);
    novo->valor = valor;
    novo->codigo = codigo;
    novo->esq = esq;
    novo->dir = dir;
    return novo;
}


p_no adiciona_no(p_no raiz, double valor, char nome[20], int codigo){ // Função que adiciona, recursivamente, um nó a uma ABB
    if(raiz==NULL)
        return cria_arvore(valor, nome, NULL, NULL, codigo);
    if(valor < raiz->valor)
        raiz->esq = adiciona_no(raiz->esq, valor, nome, codigo);
    else
        raiz->dir = adiciona_no(raiz->dir, valor, nome, codigo);
    return raiz;
}


void libera_arvore(p_no raiz){ // Função que libera a árvore recursivamente
    if(raiz->esq == NULL && raiz->dir == NULL){
        free(raiz);
        return;
    }
    if(raiz->esq != NULL)
        libera_arvore(raiz->esq);
    if(raiz->dir != NULL)
        libera_arvore(raiz->dir);
    
    free(raiz);
    return;
}

int verifica_alfabetica(char *s1, char *s2){ // Função que verifica qual das strings é "menor" (ordem alfabética).
// Caso s1 deva vir antes de s2, retorna -1, e caso seja s2, retorna 1.
    int i = 0;
    while(s1[i] != '\0' && s2[i] != '\0'){
        if(s1[i] < s2[i])
            return -1;
        else if(s1[i] > s2[i])
            return 1;
        i++;
    }
    return 0;
}


lista* cria_lista(){ // Função que cria uma lista ligada
    lista *l;
    l = malloc(sizeof(lista));
    l->prim = l->ult = NULL;
    return l;
}


lista* add_lista_alfabetica(p_no no, lista* lista, p_no_l ult){ // Função que insere um item a uma lista, já na ordem alfabética
    // Tal função verifica se o elemento é "maior ou menor" do que "ult". Caso seja maior, o novo elemento
    // deve ser inserido após ult. Caso seja menor, a função é chamada recursivamente para o elemento anterior
    // a ult, realizando a mesma verificação, até que o elemento a ser inserido seja maior ou que se chegue aos casos
    // bases, em que ult == NULL (fim da lista), ou o caso em que a lista está vazia
    p_no_l novo;
    novo = malloc(sizeof(no_lista));
    novo->no = no;

    if(lista->prim == NULL && lista->ult == NULL){ // Caso em que a lista se encontra vazia
        lista->prim = lista->ult = novo;
        novo->prox = novo->ant = NULL;
        return lista;
    }else if(ult == NULL){ // Caso em o elemento a ser inserido deve ser o primeiro da lista (ult == NULL)
        lista->prim->ant = novo;
        novo->prox = lista->prim;
        novo->ant = NULL;
        lista->prim = novo;
        return lista;
    }else if(verifica_alfabetica(novo->no->nome, ult->no->nome) == 1){ // Por fim, caso em que o elemento a ser
    // inserido é maior do que ult, ou seja, deve ser inserido após ult
        novo->ant = ult;
        novo->prox = ult->prox;
        if(ult == lista->ult){
            lista->ult = novo;
        }
        ult->prox = novo;
        return lista;
    }
    free(novo);
    return add_lista_alfabetica(no, lista, ult->ant); // Casp nenhuma das condições seja cumprida, chama novamente
    // a função para o item anterior.
}


lista* add_lista(p_no no, lista*lista){ // Função que adiciona um elemento à uma lista, sempre ao final dela
    p_no_l novo;
    novo = malloc(sizeof(no_lista));
    novo->no = no;
    if(lista->prim == NULL && lista->ult == NULL){
        lista->prim = lista->ult = novo;
        novo->prox = novo->ant = NULL;
        return lista;
    }
    lista->ult->prox = novo;
    novo->ant = lista->ult;
    lista->ult = novo;
    novo->prox = NULL;
    return lista;
}


lista* procura_faixa(p_no raiz, double valor_base, lista* lista){ // Função que, dada uma árvore de um produto
// e um valor base, procura, naquela árvore, quais lojas possuem o produto em questão naquela faixa de preço,
// e adiciona as que possuem a uma lista de lojas, e a retorna

    if(raiz->valor <= valor_base*1.1){ // Caso o valor seja menor ou igual a faixa, é necessário adicionar
    // aquela aquela loja e verificar se existem outras lojas tanto na subárvore esquerda, quanto na direita.
        lista = add_lista_alfabetica(raiz, lista, lista->ult);
        if(raiz->esq != NULL)
            lista = procura_faixa(raiz->esq, valor_base, lista);
        
        if(raiz->dir != NULL)
            lista = procura_faixa(raiz->dir, valor_base, lista);
        
        return lista;
    }else{ // Caso o valor seja maior do que a faixa desejada, basta verificar se existem valores naquela faixa
    // na subárvore esquerda (valores menores do que a raiz);
        if(raiz->esq != NULL){
            lista = procura_faixa(raiz->esq, valor_base, lista);
        }
        return lista;
    }
}


void imprime_lista(lista* lista){ // Função que, dada uma lista de lojas, a imprime e libera.
    if(lista->ult == NULL && lista->prim == NULL)
        printf("nenhuma boa oferta foi encontrada");

    p_no_l aux;
    while(lista->prim != NULL && lista->ult != NULL){
        printf("%s %d %.2lf\n", lista->prim->no->nome, lista->prim->no->codigo, lista->prim->no->valor);
        aux = lista->prim;
        lista->prim = lista->prim->prox;
        free(aux);
    }

    free(lista);
}


p_no verifica_lista(lista* lista, int codigo){ // Verifica a existência de uma árvore de um produto em uma lista.
// Caso ela ja existe, retorna o endereço da raiz. Caso não exista, retorna NULL
    p_no_l atual = lista->prim;
    while(atual != NULL){
        if(atual->no->codigo == codigo)
            return atual->no;
        else
            atual = atual->prox;
    }
    return NULL;
}

int main(){
    int n, m;

    char nome[21];
    int codigo;
    double valor;
    p_no raiz;
    lista* lista_arvores;
    lista_arvores = cria_lista(); // Criando a lista que abrigará cada uma das árvores de cada produtos

    scanf("%d", &n);
    for(int i=0; i<n; i++){
        scanf("%s %d %lf", nome, &codigo, &valor);
        raiz = verifica_lista(lista_arvores, codigo); // Verificando se ja existe uma árvore para aquele produto
        
        if(raiz == NULL){ // Criando a árvore adicionadno ela a lista de árvores, caso ela não existe
            raiz = cria_arvore(valor, nome, NULL, NULL, codigo);
            lista_arvores = add_lista(raiz, lista_arvores);
        
        }else{ // Adicionando aquela loja á árvore do produto, caso ela ja exista
            raiz = adiciona_no(raiz, valor, nome, codigo);
        }
    }

    pesquisa* pesquisas;
    pesquisas = malloc(n*sizeof(pesquisa)); // Criando o vetor que abrigará os produtos e as faixas de preço
    // a serem pesquisadas

    scanf("%d", &m);
    for(int i=0; i<m; i++){ // Adicionando os valores das pesquisas ao vetor de pesquisas
        scanf("%d %lf", &codigo, &valor);
        pesquisas[i].codigo = codigo;
        pesquisas[i].valor = valor;
    }

    lista* lojas; // Criando uma lista de lojas, que abrigará cada uma das lojas que possui aquela faixa de 
    // preço de um determinado produto
    for(int i=0; i<m; i++){
        lojas = cria_lista();
        raiz = verifica_lista(lista_arvores, pesquisas[i].codigo); // Identificando a árvore do produto a ser pesquisado
        
        if(raiz != NULL)
            lojas = procura_faixa(raiz, pesquisas[i].valor, lojas); // Procurando pela faixa de preço na árvore do produto
            // e adicionadno cada uma das lojas que se encaixa na lista de lojas
        
        if(i != 0)
            printf("\n");
        
        printf("OFERTAS para %d:\n", pesquisas[i].codigo);
        imprime_lista(lojas); // Imprimindo as lojas que possuem o determinado produto naquela faixa de preço
    }

    
    p_no_l aux1, aux2= aux1 = lista_arvores->prim;
    while(aux1 != NULL){
        aux2 = aux1->prox;
        libera_arvore(aux1->no); // Liberando cada uma das árvores na lista de árvore
        free(aux1); // Liberando o nó que abrigava a árvore
        aux1 = aux2;
    }
    free(lista_arvores); // Liberando a lista de árvores
    free(pesquisas); // liberando o vetor dos produtos que devem ser pesquisados

    return 0;
}