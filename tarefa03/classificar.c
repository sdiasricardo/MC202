#include <stdio.h>
#include <string.h>
#define MAX 100

void deletar_elemento(char array[MAX][31], int indice, int n){ // Função para deletar um item, pelo seu índice, de um array de tamanho n
    int i;
    for(i=indice; i<n; i++){
        strcpy(array[i], array[i + 1]);
    }
}


void ordena_array_alfabetica(char array[][31],int init, int fim){// Função que ordena o array das posição init à posição fim, por meio de um BubbleSort
    int i, j, k;
    char aux[31];
    for(i=init; i<fim; i++){
        for(j=fim; j>i;j--){
            for(k=0; k<31; k++){
                if(array[j - 1][k] > array[j][k]){
                    strcpy(aux, array[j]);
                    strcpy(array[j], array[j - 1]);
                    strcpy(array[j -1], aux);
                    break;
                }else if(array[j - 1][k] < array[j][k]){
                    break;
                }
            }
        }
    }
}


void ordena_array_sobrenome_alfabetica(char array[][31], int len){ // Função que, dado um array de tamanho len, o organiza por sobrenome em ordem alfabética,
    // por meio de um BubbleSort
    int i, j, g, h, indice_ant_j, indice_j;
    char aux[31];
    for(i=0; i<len -1; i++){

        for(j=len - 1; j>i;j--){

            for(int g=0; array[j-1][g] != '_'; g++) //Definindo o índice da primeira letra do sobrenome de [j - 1],
            // para que o BubbleSort seja feito apenas a partir da primeira letra do sobrenome
                if(array[j-1][g +1] == '_')
                    indice_ant_j = g + 1 ;

            for(int h=0; array[j][h] != '_'; h++) //Definindo o índice da primeira letra do sobrenome de [j],
            // para que o BubbleSort seja feito apenas a partir da primeira letra do sobrenome
                if(array[j][h + 1] == '_')
                    indice_j = h + 1 ;
            
            g = indice_ant_j;

            for(h = indice_j; array[j][h]!='\0'; g++, h++){
                if(array[j - 1][g] > array[j][h]){
                    strcpy(aux, array[j]);
                    strcpy(array[j], array[j - 1]);
                    strcpy(array[j -1], aux);
                    break;
                }else if(array[j - 1][g] < array[j][h]){
                    break;
                }
            }
        }
    }
}


int parentes(char entrada[][31], char saida[][31], int n){ // Função, dada uma lista de entrada de tamanho n, identifica os parentes nessa lista e os
    // adiciona a uma lista de saída, ja estando, dentro de cada sobrenome, as pessoas ordenadas em ordem alfabética. Retorna o tamanho do array saida
    int qtd_parentes = 0, qtd_parentes_atual;

    
    for(int i=0; i < n - 1; i++){
        int indice_i;
        qtd_parentes_atual = 0;
        int primeiro_indice;

        for(int k=0; entrada[i][k] != '_'; k++) //Definindo o índice da primeira letra do sobrenome da pessoa de indice [i], para realizar a comparação apenas dos sobrenomes.
            if(entrada[i][k +1] == '_')
                indice_i = k + 1 ;
        
        for(int j=i + 1; j<n; j ++){
            int indice_j;

            for(int g=0; entrada[j][g] != '_'; g++) //Definindo o índice da primeira letra do sobrenome da pessoa de inice[j], para realizar a comparação apenas dos sobrenomes
                if(entrada[j][g + 1] == '_')
                    indice_j = g + 1;
            
            int t=indice_j;
            for(int h=indice_i; entrada[i][h] != '\0'; h++, t++){

                if(entrada[i][h] != entrada[j][t]){ // Caso alguma das letras da string do sobrenome das pessoas de índice i e j sejam diferentes, elas não tem o mesmo sobrenome.
                // Portanto, quando a primeira diferença é encontrada, a comparação daquelas duas pessoas pode ser encerrada.
                    break;
                }else if(entrada[i][h+1] == '\0'){ // Caso a ultima letra seja igual, elas são homonimos.

                    if(qtd_parentes_atual == 0){ // Caso, para aquele determinado homonimo, ainda não exista ninguem adicionado na lista de saída(qtd_parentes_atual = 0), é necessário
                    // que o elemento base de comparação (entrada[i]) também seja adicionado na lista de saída.
                        strcpy(saida[qtd_parentes], entrada[i]);
                        primeiro_indice = qtd_parentes; // Guardando o índice do primeiro nome adicionado à lista de saída, para poder ordená-la mais tarde.
                        qtd_parentes_atual++;
                        qtd_parentes++;
                        strcpy(saida[qtd_parentes], entrada[j]);
                        qtd_parentes++;
                        deletar_elemento(entrada, j, n); // É necessário que o segundo elemento de comparação (entrada[j]) seja deletado da lista de entrada, para que o mesmo não seja,
                        // posteriormenet, um elemento base, e para que não hajam elementos repetidos na lista de saída
                        j--; // Como o elemento de indice j foi deletado, é necessário que esse contador também seja reduzido de 1.
                        n--; // Como o elemento de indice j foi deletado, é necessário que o tamanho da lista n também seja reduzido de 1.

                    }else{ // Caso ja exista um homonimo daquele respectivo nome na lista (qtd_parentes_atual != 0), basta que o segundo elemento de comparação (entrada[j]) seja adicionado
                        strcpy(saida[qtd_parentes], entrada[j]);
                        deletar_elemento(entrada, j, n);
                        qtd_parentes++;
                        j--;
                        n--;
                    }
                }
                

            }
        }
        if(qtd_parentes_atual>0){ // Caso hajam parentes daquele respectivo sobrenome, é necessário que a lista de saída seja ordenada a partir do indice do primeiro parente ao indice do ultimo.
            ordena_array_alfabetica(saida, primeiro_indice, qtd_parentes - 1);
        }
    }
    return qtd_parentes;
}


int homonimos(char entrada[][31], char saida[][31], int n){// Função que, dada uma lista de entrada de tamanho n, identifica os homonimos nessa lista e os
    // adiciona a uma lista de saída. Retorna o tamanho do array saida
    int qtd_homonimos = 0, qtd_homonimos_atual;
    int j;

    for(int i=0; i < n - 1; i++){
        qtd_homonimos_atual = 0;

        for(j=i + 1; j<=n; j ++){
            for(int h=0; entrada[i][h] != '_'; h++){

                if(entrada[i][h] != entrada[j][h]){// Caso alguma das letras da string do nome das pessoas de índice i e j sejam diferentes, elas não tem o mesmo nome.
                // Portanto, quando a primeira diferença é encontrada, a comparação daquelas duas pessoas deve ser encerrada.
                    break;
                }else if(entrada[i][h] == entrada[j][h] && entrada[i][h + 1] == '_'){

                    if(qtd_homonimos_atual == 0){ // Caso, para aquele determinado homonimo, ainda não exista ninguem adicionado na lista de saída(qtd_parentes_atual = 0), é necessário
                    // que o elemento base de comparação (entrada[i]) também seja adicionado na lista de saída.
                        strcpy(saida[qtd_homonimos], entrada[i]);
                        qtd_homonimos_atual++;
                        qtd_homonimos++;
                        strcpy(saida[qtd_homonimos], entrada[j]);
                        qtd_homonimos++;
                        deletar_elemento(entrada, j, n);
                        j--; // Como o elemento de indice j foi deletado, é necessário que esse contador também seja reduzido de 1.
                        n--; // Como o elemento de indice j foi deletado, é necessário que o tamanho da lista n também seja reduzido de 1.
                    }else{
                        strcpy(saida[qtd_homonimos], entrada[j]);
                        qtd_homonimos++;
                        qtd_homonimos_atual++;
                        deletar_elemento(entrada, j, n);
                        j--;
                        n--;
                    }
                }
                

            }
        }
    }
    return qtd_homonimos;
}

void imprime_array_char(char array[][31], int len){ // Função que, dado um array de strings, o imprime.
    for(int i=0; i<len; i++){
        printf("%s\n", array[i]);
    }
}




int main(){
    char classificacao[9];
    char nomes[MAX][31];
    char grupos[MAX][31];
    int qtd_homonimos, qtd_parentes;
    int n;
    
    scanf("%d", &n); // Lendo a quantidade de nomes que serão imputados
    scanf("%s", classificacao); // Lendo como os nomes devem ser organizados
    for(int i=0; i < n; i++){ // Lendo cada um dos n nomes.
        scanf("%s", nomes[i]);
    }
    

    if(strlen(classificacao) == 9){ // Caso o tamanho da string de classificação seja igual a 9, a entrada foi "homonimos".
        qtd_homonimos = homonimos(nomes, grupos, n);
        ordena_array_alfabetica(grupos, 0, qtd_homonimos - 1);
        imprime_array_char(grupos, qtd_homonimos);
    }else{ // Do contrário, a entrada foi "parentes"
        qtd_parentes = parentes(nomes, grupos, n);
        ordena_array_sobrenome_alfabetica(grupos, qtd_parentes);
        imprime_array_char(grupos, qtd_parentes);
    }
    
}