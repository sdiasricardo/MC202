typedef struct data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct voo {
    int numero;
    char origem[4];
    char destino[4];
    Data data;
    double valor;
} Voo;

typedef struct par_voos{
    Voo voo1;
    Voo voo2;
    double valor;
} par_voos;

Data ler_data();
    // Função que le, atribui e retorna as entradas do teclado a uma data

int compara_data(Data data1, Data data2);
    // Função que, dado duas datas, retorna -1 caso data1 seja depois de data2,
    // 0 caso sejam a mesma data, e 1 caso a data2 seja depois da data1.

int diferenca_data(Data data1, Data data2);
    // Função que, dado duas datas, retorna a diferença, em dias, entre elas. Caso a diferença retornada seja positiva,
    // a segunda data é após a primeira. Caso seja negativa, a primeira é após a segunda.

int voos_viaveis(Voo voos[], int qtd_voos, par_voos par_voos_viaveis[], char aeroporto_partida[], int diferenca_data_desejada);
    // Funçãp que, dado uma lista de voos e uma diferença de datas desejada, adiciona à uma segunda lista de
    // pares de voos, aqueles que cumprem a diferença de dias passada. Os voos dentro dos pares estão sempre na
    // ordem voo de ida e voo de volta, respectivamente.

void registrar(Voo voos[], int qtd_voos, int numero_voo, char origem[4], char destino[4], Data data, double valor);
    // Função que adiciona um voo e suas respectivas informações à lista de voos

void cancelar(Voo voos[], int qtd_voos, int numero_cancelamento);
    // Função que, dado o número de um voo, o cancela e retira da lista de voos.

void alterar(Voo voos[], int qtd_voos, int numero_alteracao, double novo_valor);
    // Função que, dado o número de um voo, atualiza o seu valor

void planejar(Voo voos[], int qtd_voos, char aeroporto_partida[], Data data_inicio, Data data_fim);
    // Função que, dado o aeroporto de partida e as datas limite de ida e volta, imprime o código do voo
    // de ida e o código do voo de volta.