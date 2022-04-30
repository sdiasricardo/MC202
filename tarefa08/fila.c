#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct no{ // struct que guarda os atributos de um integrantes da fila
    int documento;
    char profissao[26];
    char vacina[26];
    struct no *prox;
} no;


typedef struct no * p_no;


p_no criar_lista(){ // Função que cria uma lista ligada circular com cabeça e retorna o endereço da cabeça
    p_no cabeca;
    cabeca = malloc(sizeof(no));
    cabeca->prox = cabeca;
    return cabeca;
}


void adiciona_pessoa(p_no lista, p_no * end_fim, p_no * end_lanterna, p_no pessoa){ // Função que adiciona alguem em na fila
    
    if (lista->prox == lista){ // Caso da fila estar vazia (apenas a cabeça)
        lista->prox = pessoa;
        pessoa->prox = lista;
        *end_lanterna = pessoa;
        *end_fim = pessoa;

    }else{ // Outros casos

        if(strcmp(pessoa->profissao, "coach") == 0 || strcmp(pessoa->profissao, "ex-bbb") == 0
           || strcmp(pessoa->profissao, "youtuber-financas") == 0 || strcmp(pessoa->profissao, "filho-politico") == 0
           || strcmp(pessoa->profissao, "herdeiro") == 0){ // Casos especiais, em que o integrante inserido pega a lanterna
           // e assume a primeira posição da fila
            pessoa->prox = lista->prox;
            lista->prox = pessoa;
            *end_lanterna = pessoa;

        }else if(strcmp(pessoa->profissao, "funcionario") == 0){ // Caso dos funcionários, que sempre entra no fim da fila,
            // portanto o endereço do fim da fila deve ser atualizado.
            (*end_fim)->prox = pessoa;
            pessoa->prox = lista;
            (*end_lanterna) = pessoa;
            (*end_fim) = pessoa;

        }else if(strcmp(pessoa->profissao, "fiscal") == 0){ // Casos dos fiscais, que verificam a vacinação da fila
            if((strcmp((*end_lanterna)->profissao, "fiscal") == 0 && strcmp(pessoa->vacina, "vacinado") == 0)
               || strcmp((*end_lanterna)->profissao, "fiscal") != 0){ // Verificando se o integrante anterior ao fiscal é também
                // um fiscal. Caso seja e o fiscal a ser adicionado não esteja vacinado, ele não é adicionado.

                pessoa->prox = (*end_lanterna)->prox;
                (*end_lanterna)->prox = pessoa;
                (*end_lanterna) = pessoa;
                if(pessoa->prox == lista){ // Verificando se é necessária uma atualização no endereço da última posição.
                    (*end_fim) = pessoa;
                }
            
                p_no deletado;
                while(strcmp((pessoa->prox)->vacina, "vacinado") != 0 && pessoa->prox != lista){ // Removendo os integrantes
                // da fila imediatamente após o fiscal que não estejam vacinados.
                    deletado = pessoa->prox;
                    pessoa->prox = deletado->prox;
                    free(deletado);
                }
            }

        }else{
            if((strcmp((*end_lanterna)->profissao, "fiscal") == 0 && strcmp(pessoa->vacina, "vacinado") == 0)
               || strcmp((*end_lanterna)->profissao, "fiscal") != 0){ // Verificando se o integrante anterior à pessoa é um
                // um fiscal. Caso seja e a pessoa a ser adicionada não esteja vacinada, ela não é adicionada.

                pessoa->prox = (*end_lanterna)->prox;
                (*end_lanterna)->prox = pessoa;
                (*end_lanterna) = pessoa;
                if(pessoa->prox == lista){ // Verificando se é necessária uma atualização no endereço da última posição.
                    (*end_fim) = pessoa;
                }
            }
        }
    }
}


p_no deleta_prim_item(p_no lista){ // Função que deleta o primeiro item (após a cabeça) de uma lista circular.
    p_no item;
    item = lista->prox;
    lista->prox = item->prox;
    free(item);
    return lista;
}


void imprime_prim_no(p_no lista){ // Função que imprime o primeiro item (após a cabeça) de uma lista circular.
    printf("%d %s %s\n", lista->prox->documento, lista->prox->profissao, lista->prox->vacina);
}


int main(){
    p_no lista, pessoa, lanterna, fim;
    p_no *end_lanterna, *end_fim; // Criando a variável que abrigará o endereço da variável que abriga o endereço da lanterna,
    // para possibilitar a atualização de mais de uma variável na passagem por referência de funções.
    // Essas variáveis são "ponteiros duplos", ou seja, um ponteiro que aponta para outro ponteiro, para que o endereço que
    // o segundo abriga possa ser atualizado nas funções.

    lista = lanterna = fim = criar_lista();
    end_fim = &fim; // Armazenando o endereço do fim da lista
    end_lanterna = &lanterna;// Armazenando o endereço da lanterna.

    char profissao_aux[26], vacina_aux[26];
    int documento_aux;

    while(scanf("%d %s %s", &documento_aux, profissao_aux, vacina_aux) != EOF){ // Lendo as linhas do arquivo de entrada
        pessoa = malloc(sizeof(no));
        pessoa->documento = documento_aux;
        strcpy(pessoa->profissao, profissao_aux);
        strcpy(pessoa->vacina, vacina_aux);
        adiciona_pessoa(lista, end_fim, end_lanterna, pessoa);
    }

    while(lista->prox != lista){ // Imprimindo e deletando os elementos da lista, um a um.
        imprime_prim_no(lista);
        lista = deleta_prim_item(lista);
    }
    free(lista); // Deletando a cabeça da lista.
}