#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define PAI(i) ((i-1)/2)
#define F_ESQ(i) (2*i+1)
#define F_DIR(i) (2*i+2)

typedef struct no{ // Struct que abriga a palavra e suas ocorrências
    char palavra[51];
    int ocorrencia;
} no;

typedef struct no * p_no;


typedef struct hash{ // Struct que abriga a hashtable, o tamanho do vetor m e a quantidade real de palavras qtd
    p_no * vetor;
    int m;
    int qtd;
} Hash;


Hash *criar_hash(int m){ // Função que cria a hashtable
    Hash * hash;
    hash = malloc(sizeof(Hash));
    hash->vetor = malloc(m*sizeof(p_no));
    hash->m = m;
    hash->qtd = 0;
    for(int i=0; i<m; i++) // Iniciando os elementos da Hashtable com NULL
        hash->vetor[i] = NULL;
    return hash;
}


int hashing(char palavra[51], int m){ // Função que calcula o valor de hashing de uma string pelo metodo da divisão
    int i, n = 0;
    for (i = 0; i < strlen(palavra); i++)
        n = (256 * n + palavra[i]) % m;
    return n;
}


Hash* add_palavra(Hash * hash, char palavra[51]){ // Função que adiciona uma palavra à Hashtable, pelo método da
//sondagem linear
    int valor;
    valor = hashing(palavra, hash->m); // Calculando o valor do hashing

    if(hash->vetor[valor] == NULL){ // Caso em que a posição do Hashing é NULL, ou seja, não existe palavra
    //na hashtable com aquele hashing especifico
        hash->vetor[valor] = malloc(sizeof(no));
        strcpy(hash->vetor[valor]->palavra, palavra);
        hash->vetor[valor]->ocorrencia = 1;
        hash->qtd++;

    }else{ // Caso em que ja existe outra palavra com aquele hashing
        while(hash->vetor[valor] != NULL ){ // Atualizando o valor do hashing para chegar ao próximo espaço vazio
            if(strcmp(hash->vetor[valor]->palavra, palavra) == 0) // Para este caso, aquela palavra específica ja
            //esta inserida na hashtable, e o loop pode ser quebrado
                break;

            valor = (valor + 1) % hash->m;
        }

        if(hash->vetor[valor] != NULL){ // Caso seja diferente de NULL, a palavra ja existe e basta incrementar sua
        // ocorrencia na hashtable
            hash->vetor[valor]->ocorrencia++;

        }else{ // Do contrário, basta inserir no local vazio encontra no loop anterior
            hash->vetor[valor] = malloc(sizeof(no));
            strcpy(hash->vetor[valor]->palavra, palavra);
            hash->vetor[valor]->ocorrencia = 1;
            hash->qtd++;
        }
    }
    return hash;
}


void troca(p_no a, p_no b){ // Função que troca dois elementos de lugar
    no t = *a;
    *a = *b;
    *b = t;
}


int compara_no(p_no no1, p_no no2){ // Função que compara dois nós, para dizer qual deles deve ser inserido antes
    if(no1->ocorrencia > no2->ocorrencia){
        return 0;
    }else if(no1->ocorrencia < no2->ocorrencia){
        return 1;
    }else{
        if(strcmp(no1->palavra, no2->palavra) < 0){
            return 0;
        }else{
            return 1;
        }
    }
}


void desce_no_heap(p_no *heap, int n, int k){ // Função que, dado um valor, desce ele no MaxHeap, caso necessário
    int maior_filho;
    if (F_ESQ(k) < n){ // Verificando se o filho esquerdo existe
        maior_filho = F_ESQ(k);
        if (F_DIR(k) < n && compara_no(heap[F_ESQ(k)], heap[F_DIR(k)])) // Verificando se o filho direito existe
        // e se ele é maior do que o esquerdo
            maior_filho = F_DIR(k);
        if(compara_no(heap[k], heap[maior_filho])){ // Verificando se o maior dos filhos é maior do que o pai
            troca(heap[k], heap[maior_filho]);
            desce_no_heap(heap, n, maior_filho);
        }
    }
}


void HeapSort(Hash *hash, p_no *vetor){ // Função que realiza um HeapSort em outro vetor, com base na Hashtable
    int k, i = 0, n = hash->qtd;

    for(k=0; k <hash->m; k++){ // Preenchendo o vetor que abrigará os dados da hashtable
        if(hash->vetor[k] != NULL){
            vetor[i] = hash->vetor[k];
            i++;
        }
    }


    for (k = hash->qtd/2; k >= 0; k--) // transformando em heap 
        desce_no_heap(vetor, n, k);


    while (n > 1) { // extraindo o máximo
        troca(vetor[0], vetor[n - 1]);
        n--;
        desce_no_heap(vetor, n, 0);
    }
}



int busca_stopword(char **vetor, int fim, char palavra[51]){ // Função que, dada uma palavra, verifica se ela
// é uma stopword
    int i = 0;
    for(i = 0; i<=fim; i++){
        if(strcmp(vetor[i], palavra) == 0){
            return 1;
        }
    }
    return 0;
}


void deleta_caracter(char palavra[51], int indice){ // Função que, dada uma palavra e um índice, deleta o caracter
// daquele indice
    int j = indice;
    while(j < strlen(palavra)){
        palavra[j] = palavra[j+1];
        j++;
    }
}


void padroniza(char palavra[51]){ // Função que, dada uma palavra, a padroniza, removendo os símbolos e a colocando
// em letra minúscula
    int i = 0;
    while(i < strlen(palavra)){
        if(palavra[i]>='A' && palavra[i]<='Z'){
            palavra[i] = palavra[i] + ('a' - 'A');
            i++;
        }else if(!(palavra[i]>='a' && palavra[i] <= 'z') && palavra[i]>= '!'){
            deleta_caracter(palavra, i);
        }else{
            i++;
        }
    }
}


void imprime(p_no *vetor, int n){ // Função que imprime os 50 maiores valores da lista
    for(int i = n - 1; i>n - 51; i--){
        printf("%s %d\n", vetor[i]->palavra, vetor[i]->ocorrencia);
    }
}


void libera_hash(Hash *hash){ // Função que libera a hashtable criada
    for(int i=0; i<hash->m; i++){
        if(hash->vetor[i] != NULL)
            free(hash->vetor[i]);
    }
    free(hash->vetor);
    free(hash);
}


void libera_vetor(char **vetor, int tamanho){ // Função que libera o vetor de palavras
    for(int i=0; i<tamanho; i++){
        free(vetor[i]);
    }
    free(vetor);
}


int main(){
    int m, n;
    Hash *hash;
    char **stopwords;
    char palavra[51];
    p_no *maiores_ocorrencias;

    scanf("%d", &m);
    hash = criar_hash(m);
    
    scanf("%d", &n);
    stopwords = malloc(n*sizeof(char*));
    for(int i=0; i<n; i++){
        stopwords[i] = malloc(51*sizeof(char));
        scanf("%s ", stopwords[i]);
    }

    while(scanf("%s ", palavra) != EOF){
        padroniza(palavra);
        if(!busca_stopword(stopwords, n - 1, palavra) && strlen(palavra) > 1){
            hash = add_palavra(hash, palavra);
        }
    }

    maiores_ocorrencias = malloc(hash->qtd*sizeof(p_no));
    HeapSort(hash, maiores_ocorrencias);
    imprime(maiores_ocorrencias, hash->qtd);
    libera_vetor(stopwords, n);
    libera_hash(hash);
    free(maiores_ocorrencias);
}