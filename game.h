
//unidade basica do jogo, preferi ao inves de criar uma string de tamanho dois para a forma e cor para o programa ter maior claridade
typedef struct peca {
    char form;
    char cor;
} peca;

//todas as informaçoes usadas pelo jogador 
typedef struct jogador{
    char nome[20]; //nome do jogador
    peca jogadas[6]; //pecas que o jogador tem no momento
    int points; //pontos
    int vjogs; //vezes que jogou neste turno
    char **lastPlay; //os ultimos inputs do jogador nesse turno
} jogador;

char formatos[6]; //apenas os "formatos" das peças que sao "a, b, c, d, e , f"
char cores[6]; // "cores das peças "1,2,3,4,5,6"

int npecas; //basicamente uma forma de saber em qual turno estamos, que acaba sendo muito usada para saber se e o primeiro turno do jogo

peca allPecas[108]; //todas as 108 pecas do jogo que vao sendo removidas desse "saco"

char cheatMode; //diz se o cheatmode esta ativo, melhor que passar como parametro
int xMax; // essas quatro variaveis sao para saber qual sera o tamanho do tabuleiro do momento, de -1 a 1, por exemplo.
int xMin; // decidi deixa-las como variaveis globais por serem usadas em quase todos as funçoes, apesar de so serem alteradas
int yMax; //na funcao gameMove
int yMin;

void colorize(peca pecaCor);// apenas para colorir cada peça na hora de imprimir

void GameState(peca **grid); //imprimir o tabuleiro para o terminal

int validMove(peca **grid, jogador *jog, char input[20]); //checa se o movimento que o jogador quer fazer e valido

int validCommand(peca **grid, jogador *jog, char input[20]); //checa se o comando imposto pelo jogador e valido

void gameMove(peca **grid, jogador *jog, char input[20]); //controla o turno do jogador em questao

