#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"game.h"
#include"general.h"




void colorize(peca pecaCor){// apenas para colorir cada peça na hora de imprimir
    switch(pecaCor.cor){
        case '1':
            printf("\033[1;31m");
            break;
        case '2':
            printf("\033[1;32m");
            break;
        case '3':
            printf("\033[1;33m");
            break;
        case '4':
            printf("\033[1;34m");
            break;
        case '5':
            printf("\033[1;35m");
            break;
        case '6':
            printf("\033[1;36m");
            break;
    }
}
void GameState(peca **grid){ //imprimir o tabuleiro para o terminal
    
    int i;

    for(i = 0; i < yMax-yMin; i++){
        if(i == 0){
            printf("   %d  ", i+yMin);
        } else {
            printf(" %d  ", i+yMin);
        }
    }
    printf("\n");
    for(i = 0; i < xMax-xMin; i++){
        if(i+xMin >= 0){
            printf(" %d", i+xMin);
        } else {
            printf("%d", i+xMin);
        }
        printf("|");
        for(int j = 0; j < yMax-yMin; j++){
            colorize(grid[i][j]);
            printf("%c%c ", grid[i][j].form, grid[i][j].cor);
            printf("\033[0m");
            printf("|");
        } 
        printf("\n");
        for(int j = 0; j < yMax-yMin; j++){
            if(j == 0){
                printf("  ---");
            } else {
                printf(" ---");
            }
        }
        printf("\n");
    }
}

int validMove(peca **grid, jogador *jog, char input[20]){ //checa se o movimento que o jogador quer fazer e valido
    int x;
    int y;
    atoii(&x, &y, input);

    x -= xMin;
    y -= yMin;

    if(x < -1 || y < -1 || x > xMax-xMin || y > yMax-yMin){ // Nao causar erros no uso das matrizes, possivelmente passando de seus limites
        printf("------Movimento invalido------\n");
        return 0;
    }

    peca pecaJogada;
    pecaJogada.form = input[2];
    pecaJogada.cor = input[3];

    if(npecas == 108){ // se ainda nao passou nenhum turno, a unica jogada possivel e no centro do tabuleiro
        if(x != 0 || y != 0){
            printf("------Movimento invalido------\n");
            return 0;
        }
        for(int i = 0; i < 6; i++){ //checar se o jogador tem a peca jogada e ajeitar as pecas disponiveis de acordo
            if(pecaJogada.form == jog->jogadas[i].form && pecaJogada.cor == jog->jogadas[i].cor){ 
                for(int j = 0; j < 5; j++){
                    if(j <= i - 1){
                        continue;
                    }
                    jog->jogadas[j].form = jog->jogadas[j+1].form;
                    jog->jogadas[j].cor = jog->jogadas[j+1].cor;
                }
                jog->jogadas[5].form = ' ';
                jog->jogadas[5].cor = ' ';
                return 1;
            }
        }
        if(cheatMode == 's'){
            return 1;
        }
        printf("------Movimento invalido------\n");
        return 0;
    }

    if(grid[x][y].form != ' '){ //checa se existe alguma peça no local
        printf("------Movimento invalido------\n");
        return 0;
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////
//essa sequencia checa se as linhas e colunas onde o jogador quer jogar ja tem uma peca igual na fila
    for(int i = x+1; i < xMax; i++){
        if(pecaJogada.form == grid[i][y].form && pecaJogada.cor == grid[i][y].cor){
            printf("------Movimento invalido------\n");
            return 0;
        }
    }

    if(x-1 > 0){
        for(int i = x-1; grid[i][y].form != ' ' && i >= 0; i--){
            if(pecaJogada.form == grid[i][y].form && pecaJogada.cor == grid[i][y].cor){
                printf("------Movimento invalido------\n");
                return 0;
            }
        }
    }

    for(int i = y+1; grid[x][i].form != ' ' && i < yMax; i++){
        if(pecaJogada.form == grid[x][i].form && pecaJogada.cor == grid[x][i].cor){
            printf("------Movimento invalido------\n");
            return 0;
        }
    }

    if(y-1 > 0){
        for(int i = y-1; grid[x][i].form != ' ' && i >= 0; i--){
            if(pecaJogada.form == grid[x][i].form && pecaJogada.cor == grid[x][i].cor){
                printf("------Movimento invalido------\n");
                return 0;
            }
        }
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// essa sequencia checa se as peças adjacentes a jogada tem mesmo formato ou cor
    int vazio = 0;

    if(grid[x+1][y].form != ' ' || grid[x+1][y].cor != ' '){
        if((pecaJogada.form != grid[x+1][y].form && pecaJogada.cor != grid[x+1][y].cor)){
            printf("------Movimento invalido------\n");
            return 0;
        }
    } else {
        vazio++;  
    }

    if(x-1 >= 0 && (grid[x-1][y].form != ' ' || grid[x-1][y].cor != ' ')){
        if((pecaJogada.form != grid[x-1][y].form && pecaJogada.cor != grid[x-1][y].cor)){
            printf("------Movimento invalido------\n");
            return 0;
        }
    } else {
        vazio++;  
    }

    if(grid[x][y+1].form != ' ' || grid[x][y+1].cor != ' '){
        if((pecaJogada.form != grid[x][y+1].form && pecaJogada.cor != grid[x][y+1].cor)){
            printf("------Movimento invalido------\n");
            return 0;
        }
    } else {
        vazio++;  
    }

    if(y-1 >= 0 && (grid[x][y-1].form != ' ' || grid[x][y-1].cor != ' ')){
        if((pecaJogada.form != grid[x][y-1].form && pecaJogada.cor != grid[x][y-1].cor)){
            printf("------Movimento invalido------\n");
            return 0;
        }
    } else {
        vazio++;  
    }

    if(vazio >= 4){
        printf("------Movimento invalido------\n");
        return 0;
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////
//Aqui checa se a jogada tem mesma linha ou coluna das jogadas anteriores do jogador
    if(jog->vjogs > 0){
        if(jog->vjogs == 1){
            int xAnt;
            int yAnt;
            atoii(&xAnt, &yAnt, jog->lastPlay[0]);
            xAnt -= xMin;
            yAnt -= yMin;
            if(x != xAnt && y != yAnt){
                printf("------Movimento invalido------\n");
                return 0;
            }
        } else {
            int xAnt[jog->vjogs];
            int yAnt[jog->vjogs];
            int notAdj = 0;
            for(int i = 0; i < jog->vjogs; i++){
                atoii(&(xAnt[i]), &(yAnt[i]), jog->lastPlay[i]);
                xAnt[i] -= xMin;
                yAnt[i] -= yMin;
                if(x != xAnt[i]-1 && x != xAnt[i]+1 && y != yAnt[i]-1 && y != yAnt[i]+1){
                    notAdj++;
                }
            }
            
            if(notAdj == jog->vjogs){
                printf("------Movimento invalido------\n");
                return 0;
            }

            if(jog->vjogs >= 2){
                if(xAnt[0] == xAnt[1] && x != xAnt[0]){ //a primeira e segunda jogada do turno definem a linha ou coluna
                    printf("------Movimento invalido------\n");
                    return 0;
                }
                if(yAnt[0] == yAnt[1] && y != yAnt[0]){
                    printf("------Movimento invalido------\n");
                    return 0;
                }
            } 	    
        }
    }

///////////////////////////////////////////////////////////////////////

    for(int i = 0; i < 6; i++){//checar se o jogador tem a peca jogada e ajeitar as pecas disponiveis de acordo
        if(input[2] == jog->jogadas[i].form && input[3] == jog->jogadas[i].cor){
            for(int j = 0; j < 5; j++){
                if(j > i - 1){
                    jog->jogadas[j].form = jog->jogadas[j+1].form;
                    jog->jogadas[j].cor = jog->jogadas[j+1].cor;
                }
            }
            jog->jogadas[5].form = ' ';
            jog->jogadas[5].cor = ' ';
            return 1;
        }
    }

    if(cheatMode == 's'){
        return 1;
    } else {
        return 0;
    }
}

int validCommand(peca **grid, jogador *jog, char input[20]){ //checa se o comando imposto pelo jogador e valido
    switch(input[0]){
        case 'p': // passar o turno
            return 1;
        case 't': // trocar peça
            if(jog->vjogs == 0){//somente antes de jogar
                for(int i = 0; i < 6; i++){
                    if(input[2] == jog->jogadas[i].form && input[3] == jog->jogadas[i].cor){
                        jog->jogadas[i].form = formatos[rand() % 6];
                        jog->jogadas[i].cor = cores[rand() % 6];
                        return 1;
                    }
                }
                printf("------Movimento invalido------\n");
                return 0;
            } else {
                printf("------Movimento invalido------\n");
                return 0;
            }
        case 'j': // jogar e ver se o input de jgoada esta correto
            for(int i = 0; i < 6; i++){
                if(cheatMode == 's'){
                    for(int i = 0; i < 108; i++){
                        if(input[2] == allPecas[i].form && input[3] == allPecas[i].cor){
                            if(!validMove(grid, jog, input)){
                                return 0;
                            } else {
                                return 1;
                            }
                        }
                    }
                }    
                if((input[2] == jog->jogadas[i].form && input[3] == jog->jogadas[i].cor)){
                    if(!validMove(grid, jog, input)){
                        return 0;
                    } else {
                        return 1;
                    }
                }
            }
            printf("------Movimento invalido------\n");
            return 0;
        default:
            printf("------Movimento invalido------\n");
            return 0;
    }

}

void gameMove(peca **grid, jogador *jog, char input[20]){ //controla o turno do jogador em questao
    printf("Jogada de %s--------------------------------------------------------\n", jog->nome);
    printf("Pontos de %s: %d-----------------------------------------------------\n", jog->nome, jog->points);
    printf("Essas sao suas pecas: ");
    int i;
    for(i = 0; i < 6; i++){
        colorize(jog->jogadas[i]);
        printf("%c%c ",jog->jogadas[i].form, jog->jogadas[i].cor);
        printf("\033[0m");
    }
    printf("\n");
    printf("Opcoes: jogar p1 x y | passar");
    if(jog->vjogs == 0){
        printf("| trocar p1");
    }
    printf("\n");

    printf("Digite o que quer fazer:\n");
    char a;
    int cnt = 0;
    int flag = 0;
    input[0] = ' '; //sequencia que pega o input do jogador e deixa da forma desejada
    for(int i = 0; i < 20; i++){
        a = getc(stdin);
        if(a == '\n'){
            input[cnt] = '\0';
            break;
        }
        if(a == ' '){
            flag = 1;
        }
        if(i > 0 && !flag){
            continue;
        }
        input[cnt] = a;
        cnt++;    
    }
    if(i == 20){
        input[i-1] = '\0';
    }

    if(!validCommand(grid, jog, input)){ //checa se o comando e valido, senao, repete a funcao
        GameState(grid);
        gameMove(grid, jog, input);
    }

    if(input[0] == 'j'){ //particao grande da funcao que de fato aplica o input do jogador no tabuleiro e corrige o tamanho desse
        int x;
        int y;
        atoii(&x, &y, input);
        strcpy(jog->lastPlay[jog->vjogs], input);
        jog->vjogs++;
        

        if(npecas == 108){ // no primeiro turno todas as coordenadas aumentam e como a matriz tem tamanho [6][6] incialmente nao ha problema
            xMin--;
            yMin--;
            xMax++;
            yMax++;
        } else {
            //como nao e possivel aceitar coordenadas negativas em uma matriz, e necessario deslocar todas as pecas e aumentar o tamanho, quando o input e negativo o que leva a diversos casos analogos, de deslocar na horizontal, vertical e diagonal.

            if(x <= xMin+1 && y <= yMin+1 && x != 0 && y != 0){
                //começando com a diagonal, e necessario deslocar a matriz na horizontal e depois na vertical. Isso ocorrera quando o input do jogador for igual ao tamanho minimo das linhas+1 e o tamanho minimo das colunas+1.

                peca aux1 = grid[0][1];
                peca aux2;
                for(int i = 1; i <= yMax-yMin; i++){
                    for(int j = 1; j <= xMax-xMin; j++){
                        if(j % 2 == 0){
                            aux1 = grid[j][i];
                            grid[j][i] = aux2;
                        } else {
                            aux2 = grid[j][i];
                            grid[j][i] = aux1;
                        }
                    }
                }
                aux1 = grid[1][0];//nao e necessario mudar aux2 visto que tera seu valor mudado antes de ser usado
                for(int i = 1; i <= xMax-xMin; i++){
                    for(int j = 1; j <= yMax-yMin; j++){
                        if(i % 2 == 0){
                            aux1 = grid[i][j];
                            grid[i][j] = aux2;
                        } else {
                            aux2 = grid[i][j];
                            grid[i][j] = aux1;
                        }
                    }
                }
                xMin--;
                yMin--;
            } else {
                if(x <= xMin+1 && x != 0){ //deslocar na vertical para baixo
                    peca aux1 = grid[0][1];
                    peca aux2;
                    for(int i = 1; i < yMax-yMin; i++){
                        for(int j = 1; j <= xMax - xMin; j++){
                            if(j % 2 == 0){
                                aux1 = grid[j][i];
                                grid[j][i] = aux2;
                            } else {
                                aux2 = grid[j][i];
                                grid[j][i] = aux1;
                            }
                        }
                    }
                    xMin--;
                } else if(y <= yMin+1 && y != 0){ //deslocar na horizontal para a direita
                    peca aux1 = grid[1][0];
                    peca aux2;
                    grid[1][y-yMin] = grid[0][y-yMin];
                    for(int i = 1; i <= xMax-xMin; i++){
                        for(int j = 1; j <= yMax-yMin; j++){
                            if(j % 2 == 0){
                                aux1 = grid[i][j];
                                grid[i][j] = aux2;
                            } else {
                                aux2 = grid[i][j];
                                grid[i][j] = aux1;
                            }
                        }
                    }
                    yMin--;
                }
            }
            if(x >= xMax-1 && y >= yMax-1 && x != 0 && y != 0){ //se forem do tamanho limite, so precisa aumentar o limite
                xMax++;
                yMax++;
            } else {
                if(x >= xMax-1 && x != 0){
                    xMax++;
                } else if(y >= yMax-1 && y != 0){
                    yMax++;
                }
            }
        }
        grid[x-xMin][y-yMin].form = input[2];
        grid[x-xMin][y-yMin].cor = input[3];
        npecas--;
    }
    return;
}

