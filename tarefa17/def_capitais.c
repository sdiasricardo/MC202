#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 100000


typedef struct vertice{ // Struct que definirá o nome e os requisitos de cada vértice do grafo (celas)
    int indice; // Indice no vetor do grafo
    char nome[50]; // Nome da cidade
    double pop; // Populaçao de cada cidade
    double fator; // Campo que armazena o fator de centralidade
    double alcance; // Campo que armazena a população no alcance da cidade
} vertice;

typedef vertice *p_ver;


typedef struct{ // Struct que representará o grafo
    double **adj; // Lista adjunta das arestas
    int n; // Quantidade total de vértices
} Grafo;

typedef Grafo * p_grafo;


int busca_indice(p_ver cidades, int tamanho, char cidade[50]){ // Fun;cão que, dado um vetor de vertices (cidades)
// e o nome de uma cidade, retorna o seu indice
    for(int i=0; i<tamanho; i++){
        if(strcmp(cidades[i].nome, cidade) == 0){
            return i;
        } 
    }
    return -1;
}


p_grafo cria_grafo(int n){ // Função que cria um grafo de tamanho n
    p_grafo novo = malloc(sizeof(Grafo));
    novo->n = n;
    novo->adj = malloc(sizeof(double *)*(n));
    for(int i=0; i<n; i++){ // Alocando a matriz adjunta, e inicializando os valores das distâncias como infinito
        novo->adj[i] = malloc(sizeof(double)*(n));
        for(int j=0; j<n; j++){
            if(i!=j)
                novo->adj[i][j] = INT_MAX;
            else
                novo->adj[i][j] = 0;
        }
    }
    return novo;
}


void add_caminho(p_grafo grafo, p_ver cidades, char cidade1[50], char cidade2[50], double peso){ // Fun;cão
// que, dado duas cidades adjacentes, estabelece o caminho entre elas com seu peso
    int indice1 = busca_indice(cidades, grafo->n, cidade1);
    int indice2 = busca_indice(cidades, grafo->n, cidade2);

    if(grafo->adj[indice1][indice2] > peso){
        grafo->adj[indice1][indice2] = peso;

    }
}


void calc_fator(p_grafo grafo, p_ver itens){ // Função que calcula o fator de centralidade de uma cidade
    double num, den;
    for(int i=0; i<grafo->n; i++){
        num = den = 0;

        for(int j=0; j<grafo->n; j++){ // Calculando os valores do denominador e numerador para as cidades que
        // são conectados

            if(grafo->adj[i][j] != INT_MAX){
                num += (itens[j].pop*grafo->adj[i][j]);
                den += itens[j].pop;
            }
        }
        itens[i].fator = num/den;
        itens[i].alcance = den;
    }

}


void floyd_warshall(p_grafo grafo){ // Calculando a matriz de adjacências pelo algoritmo de floyd-warshall
    for(int i=0; i<grafo->n; i++){

        for(int j=0; j<grafo->n; j++){

            for(int k=0; k<grafo->n; k++){

                if(grafo->adj[j][k] > grafo->adj[j][i] + grafo->adj[i][k]){
                    grafo->adj[j][k] = grafo->adj[j][i] + grafo->adj[i][k];
                }
            }
        }
    }
}


void imprime(p_ver cidades, int n, double total){ // Função que ordena e imprime o vetor de cidades
    vertice t;

    for(int i=0; i<n - 1; i++){ // Organizando o vetor de cidades utilizando bubblesort
        for(int j=0; j<n-i-1; j++){
            if(cidades[j].fator > cidades[j + 1].fator){
                t = cidades[j];
                cidades[j] = cidades[j + 1];
                cidades[j + 1] = t;
            }else if(cidades[j].fator == cidades[j + 1].fator){
                if(strcmp(cidades[j].nome, cidades[j + 1].nome) > 0){
                    t = cidades[j];
                    cidades[j] = cidades[j + 1];
                    cidades[j + 1] = t;
                }
            }
        }
    }

    for(int i=0; i<n; i++){ // imprimindo os vetores que cumprem a condição do alcance
        if(cidades[i].alcance*2 >= total)
            printf("%s %.2lf\n", cidades[i].nome, cidades[i].fator);
    }

}


void libera_grafo(p_grafo grafo){
    for(int i=0; i<grafo->n; i++){
        free(grafo->adj[i]);
    }
    free(grafo->adj);
    free(grafo);
}


void libera_itens(p_ver itens){
    free(itens);
}


int main(){
    p_grafo grafo;
    p_ver itens;
    int n_cidades;
    char cidade1[50], cidade2[50];
    double distancia, total=0;

    scanf("%d", &n_cidades); // Lendo o numero de cidades e alocando seu vetor
    grafo = cria_grafo(n_cidades);
    itens = malloc(sizeof(vertice)* (n_cidades));

    for(int i=0; i< n_cidades; i++){ // Lendo as cidades e suas populações
        scanf("%s %lf", itens[i].nome, &itens[i].pop);
        total += itens[i].pop;
    }

    while(scanf("%s %s %lf", cidade1, cidade2, &distancia) != EOF){ // Adicionando o caminho entre as cidades
        add_caminho(grafo, itens, cidade1, cidade2, distancia);
        add_caminho(grafo, itens, cidade2, cidade1, distancia);
    }

    floyd_warshall(grafo); // Executando o algoritmo de floyd warshall para o caminho e distância dos grafos


    calc_fator(grafo, itens); // Calculando o fator de cada uma das cidades
    imprime(itens, n_cidades, total); // Imprimindo os itens

    libera_grafo(grafo);
    libera_itens(itens);
}
