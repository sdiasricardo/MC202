#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum COR {VERMELHO, PRETO};

typedef struct no { // Estrutura do nó da árvore rubro-negra esquerdista de busca. No caso da árvore de 
// usuários, o campo dominio é vazio, e no caso dos sites, o campo tentativas é vazio
    char ip[26];
    char dominio[101];
    int tentativas;
    enum COR cor;
    struct no *esq, *dir;
} no;

typedef struct no * p_no;


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



int vermelho (p_no dns){ // Função que verifica se o nó é vermelho
    if(dns == NULL)
        return 0;
    return dns->cor == VERMELHO;
}


int preto (p_no dns){ // Função que verifica se o nó é preto
    if(dns == NULL)
        return 1;
    return dns->cor == PRETO;
}


p_no rot_esq(p_no raiz){ // Função que rotaciona a árvore para a esquerda
    p_no x = raiz->dir;
    raiz->dir = x->esq;
    x->esq = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return x;
}


p_no rot_dir(p_no raiz){ // Função que rotaciona a árvore para a direita
    p_no x = raiz->esq;
    raiz->esq = x->dir;
    x->dir = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return x;
}


void sobe_ver(p_no raiz) { // Função que sobe os nós, caso ambos sejam vermelhos
    raiz->cor = VERMELHO;
    raiz->esq->cor = PRETO;
    raiz->dir->cor = PRETO;
}


p_no inserir_user_rec(p_no raiz, char ip[26]){ // Função que insere o usuário na árvore de usuários
    p_no novo;
    if (raiz == NULL) { // Caso base, em que a raiz é null, ou seja, se chegou a uma folha vazia
        novo = malloc(sizeof(no));
        novo->esq = novo->dir = NULL;
        novo->tentativas = 1;
        strcpy(novo->ip, ip);
        novo->cor = VERMELHO;
        return novo;
    }
    if (strcmp(ip, raiz->ip) < 0) // Caso em que o nó é menor do que a raiz, e deve ser inserido à esquerda
        raiz->esq = inserir_user_rec(raiz->esq, ip);
    else // Caso em que o nó é menor do que a raiz, e deve ser inserido à direita
        raiz->dir = inserir_user_rec(raiz->dir, ip);

    // corrigindo a árvore, para que seja uma rubro-negra esquerdista
    if (vermelho(raiz->dir) && preto(raiz->esq))
        raiz = rot_esq(raiz);
    if (vermelho(raiz->esq) && vermelho(raiz->esq->esq))
        raiz = rot_dir(raiz);
    if (vermelho(raiz->esq) && vermelho(raiz->dir))
        sobe_ver(raiz);

    return raiz;
}


p_no inserir_user(p_no raiz, char ip[26]) { // Casca da função de inserção
    raiz = inserir_user_rec(raiz, ip);
    raiz->cor = PRETO;
    return raiz;
}


p_no inserir_dns_rec(p_no raiz, char ip[26], char dominio[101]){ // Função que insere um site à árvore de sites
    p_no novo;
    if (raiz == NULL) { // Caso base, em que a raiz é null, ou seja, se chegou a uma folha vazia
        novo = malloc(sizeof(no));
        novo->esq = novo->dir = NULL;
        strcpy(novo->dominio, dominio);
        strcpy(novo->ip, ip);
        novo->cor = VERMELHO;
        return novo;
    }
    if (strcmp(dominio, raiz->dominio) < 0) // Caso em que o nó é menor do que a raiz, e deve ser inserido à esquerda
        raiz->esq = inserir_dns_rec(raiz->esq, ip, dominio);
    else // Caso em que o nó é menor do que a raiz, e deve ser inserido à direita
        raiz->dir = inserir_dns_rec(raiz->dir, ip, dominio);
    
    // corrigindo a árvore, para que seja uma rubro-negra esquerdista
    if (vermelho(raiz->dir) && preto(raiz->esq))
        raiz = rot_esq(raiz);
    if (vermelho(raiz->esq) && vermelho(raiz->esq->esq))
        raiz = rot_dir(raiz);
    if (vermelho(raiz->esq) && vermelho(raiz->dir))
        sobe_ver(raiz);
    
    return raiz;
}


p_no inserir_dns(p_no raiz, char ip[26], char dominio[101]){ // Casca da função de inserção
    raiz = inserir_dns_rec(raiz, ip, dominio);
    raiz->cor = PRETO;
    return raiz;
}


p_no verifica_dns(p_no raiz, char dominio[101]){ // Função que localiza e retorna um dns, dado seu dominio
    if(raiz == NULL){
        return raiz;
    }
    if(strcmp(raiz->dominio, dominio) == 0){
        return raiz;
    }
    if(strcmp(dominio, raiz->dominio) < 0)
        return verifica_dns(raiz->esq, dominio);
    else
        return verifica_dns(raiz->dir, dominio);
}


p_no verifica_user(p_no raiz, char ip[26]){ // FUnção que localiza e retorna um usuário, dado seu ip
    if(raiz == NULL){
        return raiz;
    }
    if (strcmp(raiz->ip, ip) == 0){
        return raiz;
    }
    if(strcmp(ip, raiz->ip) < 0)
        return verifica_user(raiz->esq, ip);
    else
        return verifica_user(raiz->dir, ip);
}


int main(){
    int u, n, m;
    p_no arv_sites = NULL, arv_users = NULL;
    char ip[26], dominio[101];
    p_no site, user;

    scanf("%d", &u); // Lendo o valor de u
    scanf("%d", &n); // Lendo o valor de u
    for(int i=0; i<n; i++){
        scanf("%s %s", dominio, ip); // Lendo os dns e os guardando na árvore de dns
        arv_sites = inserir_dns(arv_sites, ip, dominio);
    }

    scanf("%d", &m);
    for(int i=0; i<m; i++){
        scanf("%*s %s %*s %s", dominio, ip); // Lendo os usuários e os dominios que eles querem acessar
        site = verifica_dns(arv_sites, dominio); // Buscando e guardando o nó do domínio desejado
        user = verifica_user(arv_users, ip); // Buscando e guardando o usuário e suas infos

        if(user != NULL){ // Caso a busca não retorne NULL, o usuário ja existe, e portanto ja realizou tentativas
            if(user->tentativas >= u){ // Caso em que suas tentativas são maiores do que as permitidas
                printf("FORBIDDEN %s FROM %s\n", dominio, ip);
            }else if(site != NULL){ // Caso em que a busca do site não resulta em NULL, ou seja, ele existe
                printf("ACCEPTED %s FROM %s RESPOND %s\n", dominio, ip, site->ip);
                user->tentativas++;
            }else{ // Caso em que o site não existe
                printf("NOTFOUND %s FROM %s\n", dominio, ip);
                user->tentativas++;
            }
        
        }else if(user == NULL){ // Caso o user seja NULL, ele ainda não buscou nada, ou seja, basta verificar
        // se o site existe
            arv_users = inserir_user(arv_users, ip);
            if(site == NULL){ // Caso em que a busca do site resulta em NULL, ou seja, ele não existe
                printf("NOTFOUND %s FROM %s\n", dominio, ip);
            }else{ // Casi en que o site existe
                printf("ACCEPTED %s FROM %s RESPOND %s\n", dominio, ip, site->ip);
            }
        }
    }
    // Liberando ambas as árvores
    libera_arvore(arv_sites);
    libera_arvore(arv_users);

}
