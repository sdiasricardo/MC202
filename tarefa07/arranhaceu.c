#include <stdio.h>
#include <stdlib.h>

int **le_matriz(int n){ // Função que le as entradas da matriz e a guarda
    int i, j, **m;
    m = malloc((n + 2)*sizeof(int *));
    for(i=0; i < n + 2; i++){
        m[i] = malloc((n + 2)*sizeof(int));
        for(j=0; j < n + 2; j++){
            scanf("%d", &m[i][j]);
        }
    }

    return m;
}


void libera_matriz(int **m, int n){ // Função que libera o espaço alocado para uma matriz
    int i;
    for(i=0; i<(n + 2); i++){
        free(m[i]);
    }
    free(m);
}


void imprime_matriz(int **m, int n){
    int i, j;
    for(i=1; i<=n; i++){
        for(j=1; j<=n; j++){
            if(j != n){
                printf("%d ", m[i][j]);
            }else{
                printf("%d\n", m[i][j]);
            }
        }
    }
}


void proxima_posicao(int i, int j, int *ni, int *nj, int n){ // Funcão que atualiza os valores de ni e nj,
// que são as próximas posições das linhs e colunas, respectivamente
    if(j < n){
        *ni = i;
        *nj = j + 1;
    } else{
        *ni = i + 1;
        *nj = 1;
    }
}


int qtd_arranhaceu(int **m, int i, int j, int n, int valor, int identificador){//Função que
// verifica os valores de visualização da esquerda e direita, retornando 0 caso 1 deles falhe,
// e 1 caso os 2 sejam verificados. Identificador recebe 1 quando a verificação deve ocorrer em uma Linha
// e 2 caso a verificação deva ocorrer em uma coluna


    int k;
    int maior_esquerda = 0, maior_direita = 0, qtd_esquerda = 0, qtd_direita = 0, indice;
    m[i][j] = valor;

    if(identificador == 1){ // Caso das linhas
        for(k = 1; k<=n; k++){ // Buscando o indice do maior arranhaceu, que é o último que será visto por ambos
        // os lados.
            if(m[i][k] == n){
                indice = k;
                break;
            }
        }
        for(k = 1; k<=indice; k++){ // Caontabilizando quantos arranhaceus o valor da esquerda visualiza
            if(m[i][k] > maior_esquerda){
                qtd_esquerda++;
                maior_esquerda = m[i][k];
            }
        }
        
        if(qtd_esquerda != m[i][0]){ // Caso seja diferente do esperado, deve retornar 0
            return 0;
        }

        for(k=n; k>=indice; k--){ // Verificando quantos arranhaceus o valor da direita visualiza
            if(m[i][k] > maior_direita){
                qtd_direita++;
                maior_direita = m[i][k];
            }
        }
        if(qtd_direita != m[i][n+1]){
            return 0;
        }
        
    }else if(identificador == 2){ // Caso das colunas. Lógica é exatamente a mesma das linhas, apenas é mudado
    // as variações em i e j.
        for(k = 1; k<=n; k++){
            if(m[k][j] == n){
                indice = k;
                break;
            }
        }
        for(k=1; k<=indice; k++){
            if(m[k][j] > maior_esquerda){
                qtd_esquerda++;
                maior_esquerda = m[k][j];
            }
        }
        
        if(qtd_esquerda != m[0][j]){
            return 0;
        }

        for(k=n; k>=indice; k--){
            if(m[k][j] > maior_direita){
                qtd_direita++;
                maior_direita = m[k][j];
            }
        }
        if(qtd_direita != m[n+1][j]){
            return 0;
        }
    }
    // Caso nada falhe, deve retornar 1
    return 1;
}



int valor_valido(int **m, int n, int i, int j, int valor){ // Funcão que verifica se determinado valor ja
// existe na linha ou coluna na qual está sendo verificado.
    int k;
    for(k=1; k<i; k++){
        if(m[k][j] == valor){
            return 0;
        }
    }
    for(k=1; k<j; k++){
        if(m[i][k] == valor){
            return 0;
        }
    }

    return 1;
}


int arranha_ceuR(int **m, int n, int i, int j){ // Função que monta o quadro de arranhceu e o imprime. Retorna
// 1 caso o valor a ser inserido seja válido, e 0 caso não seja.
    
    int ni, nj, valor;
    if(i == n + 1){ // Quando i == n+1, chegou-se a última linha da matriz, ou seja, o quadro de arranhaceu
    // ja foi montado por completo.
        imprime_matriz(m, n);
        libera_matriz(m, n);
        return 1;
    }
    proxima_posicao(i, j, &ni, &nj, n); // Atualizando a próxima posição
    
    for(valor = 1; valor <= n; valor++){ // Tentando cada um dos n valores. Caso não seja possível, o próximo
    // valor é testado, até que se tenha um valor possível. Caso nenhum deles seja possível, é necessário retornar
    //0, para que o valor da chamada recursiva anterior seja alterado (backtracking)
        
        if(j == n){ // Caso da última fila do quadro de arranhaceu, onde os valores de visualização devem ser
        // verificados

            if(i!=n){ // Quando i!=n, basta verificar os valores de visualização das linhas e se o valor é valido
                if(valor_valido(m, n, i, j, valor) && qtd_arranhaceu(m, i, j, n, valor, 1)){
                    m[i][j] = valor;
                    if(arranha_ceuR(m, n, ni, nj)){ // Verificando se a próxima posição também é válida
                        return 1;
                    }
                }
            
            }else{ // Quadno i == n, é necessário verificar, também, os valores de visualização das colunas
                if(valor_valido(m, n, i, j, valor) && qtd_arranhaceu(m, i, j, n, valor, 1) && qtd_arranhaceu(m, i, j, n, valor, 2)){
                    m[i][j] = valor;
                    if(arranha_ceuR(m, n, ni, nj)){
                        return 1;
                    }
                }
            }
        }else if(i == n){ // Quando i == n, é necessário verificar os valores de visualização da coluna e se o valor é válido
            if(valor_valido(m, n, i, j, valor)){
                if(qtd_arranhaceu(m, i, j, n, valor, 2)){ 
                    m[i][j] = valor;
                    if(arranha_ceuR(m, n, ni, nj)){// Verificando se a próxima posição também é válida
                    return 1;
                    }
                
                }
            }
        }else{ // Quando é um valor do meio do quadro, basta verificar se ele já existe no quadro
            if(valor_valido(m, n, i, j, valor)){
                m[i][j] = valor;
                if(arranha_ceuR(m, n, ni, nj)){
                    return 1;
                }
            }   
        }
        
    }
    m[i][j] = 0;
    return 0;
}




int main(){
    int **m, n;
    scanf("%d\n", &n);

    m = le_matriz(n);
    arranha_ceuR(m, n, 1, 1);
    

}