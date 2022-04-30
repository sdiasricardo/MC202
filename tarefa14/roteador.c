#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definindo os valores dos pais e dos filhos de cada item no MaxHeap
#define PAI(i) ((i-1)/2)
#define F_ESQ(i) (2*i+1)
#define F_DIR(i) (2*i+2)

typedef struct item{ // Definindo cada item do max heap, que será implementado utilizando vetores
    int id, prioridade, incremento;
} Item;

typedef struct heap{ // Definindo a estrutura do MaxHeap
    Item *itens;
    int n, tamanho;
} heap;


void troca(Item *a, Item *b){ // Função que troca dois elementos de lugar no vetor do MaxHeap
    Item t = *a;
    *a = *b;
    *b = t;
}


void sobe_no_heap(heap *heap, int k){ // Função que, dado um heap e um indíce k, verifica se o elemento associado
// ao índice precisa subir, e o faz caso precise
    if (k > 0 && heap->itens[PAI(k)].prioridade < heap->itens[k].prioridade) {
        troca(&heap->itens[k], &heap->itens[PAI(k)]);
        sobe_no_heap(heap, PAI(k));
    }
}


heap *add_item(heap *roteador, int id, int prioridade, int incremento){ // Função que adiciona um elemento ao heap
    roteador->itens[roteador->n].id = id;
    roteador->itens[roteador->n].prioridade = prioridade;
    roteador->itens[roteador->n].incremento = incremento;
    sobe_no_heap(roteador, roteador->n); // Como o elemento é sempre adicionado na ultima posiçao n, é necessário
    // verificar se é preciso que ele suba no Heap, de forma a mantê-lo como um MaxHeap
    roteador->n++;
    return roteador;
}


void desce_no_heap(heap *heap, int k){ // Função que verifica se um elemento necessita descer de posição no heap,
// de forma a mantê-lo como um MaxHeap
    int maior_filho;
    if (F_ESQ(k) < heap->n) { // Verificando se o elemento possui filhos
        maior_filho = F_ESQ(k);
        if (F_DIR(k) < heap->n && heap->itens[maior_filho].prioridade < heap->itens[F_DIR(k)].prioridade){ // Verificando
        // se o filho direito é maior do que o filho esquerdo
            maior_filho = F_DIR(k);
        }
        if (heap->itens[k].prioridade < heap->itens[maior_filho].prioridade){ // Verificando se o maior dos filhos é
        // maior do que o pai, e portanto, se há necessidade de descer o pai com ele
            troca(&heap->itens[k], &heap->itens[maior_filho]);
            desce_no_heap(heap, maior_filho);
        }
    }
}


Item extrai_maximo(heap *roteador){ // Função que extrai o máximo de um max heap
    Item max = roteador->itens[0];
    troca(&roteador->itens[0], &roteador->itens[roteador->n - 1]); // Trocando o primeiro elemento (que foi extraído)
    // pelo último, para manter o Heap
    roteador->n--;
    desce_no_heap(roteador, 0); // Descendo o primeiro elemento para manter a estrtutura do MaxHeap
    return max;
}


void imprime_prioridades(heap *roteador, int k){ // Função que imprime os k primeiros elemento do heap
    Item pacote;
    int limite = k;
    if(roteador->n < k) // Verificando se k é, de fato, o limite de valores que serão impressos
        limite = roteador->n;

    for(int i=0; i<limite; i++){ // Iterando e imprimindo os valores
        pacote = extrai_maximo(roteador);
        printf("%d\t%d\t%d\n", pacote.id, pacote.prioridade, pacote.incremento);
    }
}


void add_incremento(heap *roteador){ // Função que, dado um roteador, realiza os incrementos de prioridades e o 
// reorganiza conforme as novas prioridades para manter o MaxHeap
    for(int i=0; i<roteador->n; i++){
        roteador->itens[i].prioridade += roteador->itens[i].incremento;
        sobe_no_heap(roteador, i);
    }
}


int main(){
    int k, m;
    heap *roteador;

    scanf("%d %d", &k, &m);

    roteador = malloc(sizeof(heap)); // Criando o MaxHeap
    roteador->itens = malloc(m*sizeof(Item));
    roteador->n = 0;
    roteador->tamanho = m;

    int id, prioridade, incremento, tick = 1;

    while(scanf("%d %d %d", &id, &prioridade, &incremento) != EOF){
        if(id == 0 && prioridade == 0 && incremento == 0){ // Caso id == prioridade == incremento == 0, temos
        // TICK, e portanto uma rodada de impressão e, depois, uma rodada de incrementos
            printf("\nTICK %d\n", tick);
            tick++;
            imprime_prioridades(roteador, k);
            add_incremento(roteador);
        }else{ // Do contrário, temos a chegada de um novo pacote
            if(roteador->n != roteador->tamanho){ // Casp p vetor do MaxHeap não esteja cheio, basta adicionar
            // o novo pacote ao MaxHeap do roteador
                roteador = add_item(roteador, id, prioridade, incremento);
            }
        }
    }
    // Liberando o roteador e seu MaxHeap
    free(roteador->itens);
    free(roteador);
}