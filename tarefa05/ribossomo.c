#include <stdio.h>
#include <stdlib.h>

typedef struct ribossomo_alienigena{
    int *ID;
    char *dna;
    char *rna;
} tipo_de_alienigena;

void imprime_alienigena(int n, tipo_de_alienigena alienigena){ // Função que imprime e libera o espaço alocado para cada alienigena
    printf("ID: \t");
    for(int i=0; i<n; i++){
        if(i != n - 1){
            printf("%d\t", alienigena.ID[i]);
        } else{
            printf("%d\n", alienigena.ID[i]);
        }
    }
    free(alienigena.ID);

    printf("DNA:\t");
    for(int i=0; i<n; i++){
        if(i != n - 1){
            printf("%c\t", alienigena.dna[i]);
        } else{
            printf("%c\n", alienigena.dna[i]);
        }
    }
    free(alienigena.dna);

    printf("\t\t");
    for(int i=0; i<n; i++){
        if(i != n - 1){
            printf("|\t");
        } else{
            printf("|\n");
        }
    }

    printf("RNA:\t");
    for(int i=0; i<n; i++){
        if(i != n - 1){
            printf("%c\t", alienigena.rna[i]);
        } else{
            printf("%c\n", alienigena.rna[i]);
        }
    }
    free(alienigena.rna);
}


tipo_de_alienigena conversao_ribossomo(int n, char dna_padrao[], char rna_padrao[]){ // Função que realiza o sequenciamento de bases dos alienígenas e 
    // aloca os devidos espaços para cada um dos parâmetros da struct dos alienígenas, retornando a struct alienígena
    tipo_de_alienigena alienigena;
    alienigena.ID = malloc(n*sizeof(int));
    alienigena.dna = malloc((n + 1)*sizeof(char));
    alienigena.rna = malloc(n*sizeof(char));

    scanf("%s", alienigena.dna); // Lendo os valores do dna do alienígena.
    for(int i=0; i < n; i++){ // Iniciando a iteração sobre o dna para a conversão e identificação

        for(int k=0; k < 26; k++){ // Verificando a correspondência do dna do alienígena no dna padrão e convertendo-o para o rna
            if(alienigena.dna[i] == dna_padrao[k]){
                alienigena.rna[i] = rna_padrao[k];
                break;
            }
        }

        for(int j=i; j>=0; j--){ // Realizando a identificação do alienígena, com base na última identificação encontrada na lista de ID.
            if(i == 0){
                alienigena.ID[0] = 0;

            }else if(j == 0){
                alienigena.ID[i] = 0;

            }else if(alienigena.dna[i] == alienigena.dna[j - 1]){
                alienigena.ID[i] = alienigena.ID[j - 1] + 1;
                break;
            }
        }
        
    }
    return alienigena;
}


int main(){
    char dna_padrao[26], rna_padrao[26];

    int n;

    scanf("%s", dna_padrao); // Lendo a sequencia de dna padrão de cada um tipo
    scanf("%s", rna_padrao); // Lendo a correspondência de rna padrão
    scanf("%d", &n); // Lendo a quantidade de bases de dna

    tipo_de_alienigena alienigena;
    alienigena = conversao_ribossomo(n, dna_padrao, rna_padrao);
    imprime_alienigena(n, alienigena);
}