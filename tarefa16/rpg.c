#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertice{ // Struct que definirá o nome e os requisitos de cada vértice do grafo (celas)
    char nome[30];
    int requisitos[6];
} vertice;


typedef struct{ // Struct que representará o grafo
    int **adj; // Matriz adjunta das arestas
    vertice *vertices; //  Lista que abrigará cada um dos vértices
    int n; // Quantidade total de vértices
    int qtd; // Quantidade atual de vértices
} Grafo;

typedef Grafo * p_grafo;


typedef struct player{ // Struct que define os dados de um player
    char nome[30];
    int atributos[6]; // Campo que abriga os atributos iniciais de cada player
    int *final; // Campo que abriga, conforme o índice, quais as possibilidades de término de um player
} player;


p_grafo cria_grafo(int n){ // Função que cria um grafo de n vértices
    p_grafo grafo;
    grafo = calloc(1, sizeof(Grafo));

    grafo->n = n;
    grafo->qtd = 0;
    grafo->adj = calloc(n, sizeof(int *));
    grafo->vertices = calloc(n, sizeof(vertice));
    for(int i=0; i<n; i++){
        grafo->adj[i] = calloc(n, sizeof(int));
    }
    return grafo;
}


void add_vertice(p_grafo grafo, char nome[30], int *requisitos){ // Função que adiciona um vértice ao grafo
    vertice novo;
    strcpy(novo.nome, nome);
    for(int i=0; i<6; i++){
        novo.requisitos[i] = requisitos[i];
    }
    grafo->vertices[grafo->qtd] = novo;
    grafo->qtd++;
}

void add_player(player *players, int indice, int atributos[6], char nome[30], int n){ // Função que adiciona
// um player a lista de players
    player novo;
    strcpy(novo.nome, nome);
    for(int i=0; i<6; i++){
        novo.atributos[i] = atributos[i];
    }
    novo.final = calloc(n, sizeof(int));
    players[indice] = novo;
}


int verifica_atributos(int atributos1[6], int atributos2[6]){ // Função que verifica se os atributos1 são
// menores do que os atributos2.
    int adicao = 0; // Variável que indicará se algum dos atributos ja foi incremetado conforme o lançamento
    // dos dados no inicio de cada rodada
    for(int i=0; i<6; i++){
        if(atributos1[i] < atributos2[i]){
            if(adicao != 0 || atributos1[i] + 1 != atributos2[i]){ // Caso adicao != 0, algum atributo ja foi
            // incremetado anteriormente, portanto o atributo atual nao pode ser incrementado. Para a outra
            // condicao, caso falhe, mesmo que ainda possa ser adicionado o incremento, caso a diferença seja
            // != de 1, ainda assim o atributo sera menor
                return 0;

            }else{ // Do contrário, basta mudar o indicador de incremento e seguir com a verificação
                adicao = 1;
            }
        }
    }
    return 1;
}


void constroi_grafo(p_grafo grafo){ // Função que constrói o grafo com base nos atributos, onde os arcos são
// estabelecidos caso os atributos da partida sejam maiores ou iguais que os atributos do destino
    vertice u, v;
    for(int i=0; i<grafo->n; i++){
        u = grafo->vertices[i]; // Estabelecendo o vértice de partida para comparação

        for(int j=0; j<grafo->n; j++){
            v = grafo->vertices[j]; // Estabalecendo o vértice de chegada para comparação
            grafo->adj[i][j] = verifica_atributos(u.requisitos, v.requisitos);
            
        }
    }
}


void finais(p_grafo grafo, player *players, int personagens){ // Função que preenche o vetor de finais possíveis
// para cada um dos players na lista de players
    player atual;
    int possivel;
    vertice u;

    for(int k=0; k<personagens; k++){ // Iterando sobre os players
        atual = players[k];
        
        for(int i=0; i<grafo->n; i++){ // Iterando sobre os vértices, para verificar por onde o player pode
        // comecar sua jogada
            u = grafo->vertices[i];
            possivel = verifica_atributos(atual.atributos, u.requisitos);

            if(possivel){ // Caso seja possivel comecar por aquela cela, o player pode encerrar o jogo (após a
            // 2 rodada) em todas as celas as quais é possível chegar partindo da cela em questão (arcos de ida
            // da cela u para a cela j)

                for(int j=0; j<grafo->n; j++){

                    if(!atual.final[j] && grafo->adj[i][j])
                        atual.final[j] = 1;
                }
            }


        }
    }
}


void imprime_finais_possiveis(p_grafo grafo, player *players, int personagens){ // Função que, dado um grafo e os players
// imprime todos as salas possíveis para se encerrar o jogo
    int impossivel = 0; // Variável que vai verificar se é possível terminar o jogo em 2 rodadas
    for(int i=0; i< grafo->n; i++){ // Iterando sobre cada uma das celas do jogo

        for(int j=0; j<personagens; j++){ // Iterando sobre os personagens e verificando se todos possuem, na sua
        // lista de finais possiveis, a cela correspondente ao indice i.

            if(!players[j].final[i]){ // Caso algum deles seja 0, já não é possível encerrar o jogo em duas
            // rodadas naquela cela, e a verificação pode prosseguir
                break;
            }else if(j == personagens - 1){
                impossivel = 1;
                printf("%s\n", grafo->vertices[i].nome);
            }
        }
    }

    if(!impossivel)
        printf("Impossivel terminar em duas rodadas.");
}


void libera_grafo(p_grafo grafo){ // Funçao que libera a struct alocada para o grafo
    for(int i=0; i<grafo->n; i++){
        free(grafo->adj[i]);
    }
    free(grafo->adj);
    free(grafo->vertices);
    free(grafo);
}


void libera_players(player *players, int personagens){ // Função que libera a struct alocada para os players
    for(int i=0; i<personagens; i++){
        free(players[i].final);
    }
    free(players);
}


int main(){
    int n, personagens, atributos[6];
    p_grafo grafo;
    char nome[30];
    player *players;

    scanf("%d", &n); // Lendo o número total de vertices
    grafo = cria_grafo(n);

    for(int i=0; i<n; i++){
        scanf("%s %d %d %d %d %d %d", nome, &atributos[0], &atributos[1], &atributos[2], &atributos[3], &atributos[4], 
        &atributos[5]);
        add_vertice(grafo, nome, atributos);
    }

    scanf("%d", &personagens);
    players = malloc(personagens*sizeof(player));

    for(int i=0; i< personagens; i++){
        scanf("%s %d %d %d %d %d %d", nome, &atributos[0], &atributos[1], &atributos[2], &atributos[3], &atributos[4], 
        &atributos[5]);
        add_player(players, i, atributos, nome, n);
    }

    constroi_grafo(grafo);
    finais(grafo, players, personagens);
    imprime_finais_possiveis(grafo, players, personagens);

    libera_grafo(grafo);
    libera_players(players, personagens);

}