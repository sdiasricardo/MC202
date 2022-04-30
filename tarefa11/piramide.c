#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct no { // Definindo os nós da árvore
    double valor;
    char nome[21];
    struct no *esq, *dir, *pai;
    int nivel;
} no;


typedef struct no * p_no;


typedef struct no_fila{ // Definindo os nós da fila, que será impletada por lista ligada para impressão
// Utilizando BFS
    p_no no;
    struct no_fila * prox;
} no_fila;


typedef struct no_fila * f_no; // Definindo o ponteiro do nó da fila


typedef struct fila{ // Definindo a fila
    f_no inicio;
    f_no fim;
} fila;


p_no cria_arvore(double valor, char nome[20], p_no esq, p_no dir, p_no pai, int nivel){ // Função que cria a árvore
    p_no novo = malloc(sizeof(no));
    strcpy(novo->nome, nome);
    novo->valor = valor;
    novo->esq = esq;
    novo->dir = dir;
    novo->pai = pai;
    novo->nivel = nivel;
    return novo;
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


fila* criar_fila(p_no no){ // Função que cria a fila, dado um nó inicial 
    fila * f = malloc(sizeof(fila));
    f_no novo = malloc(sizeof(no_fila));
    novo->no = no;
    novo->prox = NULL;
    f->inicio = novo;
    f->fim = novo;
    return f;
}


int fila_vazia(fila * fila){ // Função que verifica se a fila está vazia, e retorn 1 caso esteja e 0 caso não esteja
    if(fila->fim == NULL && fila->inicio == NULL)
        return 1;
    return 0;
}


void enqueue(p_no no, fila* fila){ // Função que adiciona um nó à fila
    f_no novo = malloc(sizeof(no_fila));
    novo->no = no;
    novo->prox = NULL;
    if(fila_vazia(fila)){
        fila->inicio = novo;
        fila->fim = novo;
        return;
    }
    fila->fim->prox = novo;
    fila->fim = novo;
}


p_no dequeue(fila *fila){ // Função que remove um nó da fila e o libera, retornando o nó(árvore) do primeiro da fila
    f_no temp;
    p_no retorno;
    temp = fila->inicio;
    retorno = temp->no;
    fila->inicio = temp->prox;
    if(fila->inicio == NULL){
        fila->fim = NULL;
    }
    free(temp);
    return retorno;
}




void libera_fila(fila *fila){ // Função que libera a fila
    free(fila);
}


void imprime_arvore(p_no raiz){ // Função que imprime os nós de uma árvore por meio de BFS
    fila * fila = criar_fila(raiz);
    int nivel = 0;
    while(!fila_vazia(fila)){
        raiz = dequeue(fila); // Desenfileirando o primeiro elemento da fila
        if(raiz != NULL){
            if(raiz->esq != NULL) // Verificando se o elemento possui filho esquerdo e adicionando ela a fila
                enqueue(raiz->esq, fila);
            if(raiz->dir != NULL) // Verificando se o elemento possui filho direito e adicionando ela a fila
                enqueue(raiz->dir, fila);
            if(raiz->nivel != nivel){ // Caso o nível do nó atual seja diferente do nível que estava sendo
            // impresso, é necessário criar uma nova linha de impressão
                printf("Nivel %d: [%s %.2lf]", raiz->nivel, raiz->nome, raiz->valor);
                nivel = raiz->nivel;
            }else{ // Do contrário, é necessário adicionar as informações à mesma linha de impressão
                printf(" [%s %.2lf]", raiz->nome, raiz->valor);
            }
            
            if(fila_vazia(fila)){ // Formatando a saída
                printf("\n\n");
            }else if(nivel != fila->inicio->no->nivel){
                printf("\n");
            }
        }
    }
    libera_fila(fila); // Liberando a fila
}


p_no procurar_no(char nome[20], p_no raiz){ // Função que procura e retorna o nó, dado o seu nome
    p_no esq;
    if(raiz == NULL || strcmp(raiz->nome, nome) == 0){
        return raiz;
    }
    esq = procurar_no(nome, raiz->esq);
    if(esq != NULL)
        return esq;
    return procurar_no(nome, raiz->dir);
}


void remuneracao(p_no recrutado, double percentual){ // Função que aplica as remunerações quando há dois recrutamentos
    
    if(percentual == 6){ // Quando percentual == 6, o pai deve receber porcentagens de ambos os filhos
        recrutado->pai->valor += (percentual/100)*(recrutado->valor);
        recrutado->valor = (recrutado->valor)*(1 - percentual/100);
        recrutado->pai->valor += (percentual/100)*(recrutado->pai->esq->valor);
        recrutado->pai->esq->valor = (recrutado->pai->esq->valor)*(1 - percentual/100);

        if(recrutado->pai->pai == NULL){ // Caso o pai do pai seja NULL, chegou-se à raiz e acaba o processo
            return;
        }else{ // Do contrário, basta repetir o processo para o pai
             return remuneracao(recrutado->pai, percentual -1);
        }
    }

    // Caso percentual != 6, o pai deve receber apenas do filho em questão
    recrutado->pai->valor += (percentual/100)*(recrutado->valor);
    recrutado->valor = (recrutado->valor)*(1 - percentual/100);
    if(percentual == 1 || recrutado->pai->pai == NULL){ // Para tais condições, chegou-se ao fim do processo de
        return;
    }else{// Do contrário, basta repetir o processo para o pai
        return remuneracao(recrutado->pai, percentual-1);
    }
}


p_no recrutamento(p_no raiz, char recrutador[20], char recrutado[20], double valor){ // Função que preenche
// a árvore após um recrutamento, e realiza as operações em questão
    p_no rec = procurar_no(recrutador, raiz);

    rec->valor -= valor*0.1;
    valor = valor*1.1;

    if(rec->esq == NULL){ // Caso rec->esq == NULL, temos que a posição de filho da esquerda está "vaga"
        rec->esq = cria_arvore(valor, recrutado, NULL, NULL, rec, rec->nivel + 1);
    }else if(rec->dir == NULL){ // Do contrário, deve ser adicionado na direita e realizada a remuneração dos pais
        rec->dir = cria_arvore(valor, recrutado, NULL, NULL, rec, rec->nivel + 1);
        remuneracao(rec->dir, 6);
    }
    imprime_arvore(raiz); // Ao fim, é impressa a árvore em questão, até a última inserção
    return raiz;

}


int main(){
    char recrutado[21], recrutador[21];
    double valor;
    p_no raiz;

    scanf("%*s %s %lf", recrutado, &valor); // Lendo a raiz e criando a árvore
    raiz = cria_arvore(valor, recrutado, NULL, NULL, NULL, 1);
    imprime_arvore(raiz);

    while(scanf("%s %*s %s %lf", recrutador, recrutado, &valor) != EOF){ // Lendo os recrutamentos adicionais
        raiz = recrutamento(raiz, recrutador, recrutado, valor);
    }
    
    libera_arvore(raiz); // Liberando a árvore
    return 0;
}