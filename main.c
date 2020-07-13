#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"game.h"
#include"general.h"


void randomizarPecas(){ //criar as peças e coloca-las no allPecas (seria o saco do jogo fisico)
    char f = 'a';
    char c = '1';
    for(int i = 0; i < 6; i++){
        formatos[i] = f;
        cores[i] = c;
        f++;
        c++;
    }
    int j = 0;
    int k = 0;
    for(int i = 0; i < 108; i++){
        if(i != 0){
            if(i % 3 == 0){
                k++;
            }
            if(i % 18 == 0){
                j++;
                k = 0;
            }
        }
        allPecas[i].form = formatos[j];
        allPecas[i].cor = cores[k];
    }
}


void checkPoints(peca **grid, jogador *jog){ //checa quantos pontos o jogador fez em seu turno
    if(jog->vjogs > 0){
        int pointsWon[jog->vjogs];
        int xAnt[jog->vjogs];
        int yAnt[jog->vjogs];
        int vlinha[jog->vjogs];
        int vcoluna[jog->vjogs];
        int qwirkle = 0;

        for(int i = 0; i < jog->vjogs; i++){ //apos algumas observaçoes e visivel que o jogo maximiza considerando suas jogadas
            pointsWon[i] = 0;
            vlinha[i] = 0;
            vcoluna[i] = 0;
            atoii(&(xAnt[i]), &(yAnt[i]), jog->lastPlay[i]);   
            xAnt[i] -= xMin;
            yAnt[i] -= yMin; 
            for(int j = xAnt[i]+1; grid[j][yAnt[i]].form != ' ' && j < xMax-xMin; j++){
                vlinha[i]++;
            }

            for(int j = xAnt[i]-1; grid[j][yAnt[i]].form != ' ' && j >= 0; j--){
                vlinha[i]++;
            }

            for(int j = yAnt[i]+1; grid[xAnt[i]][j].form != ' ' && j < yMax-yMin; j++){
                vcoluna[i]++;
            }
            for(int j = yAnt[i]-1; grid[xAnt[i]][j].form != ' ' && j >= 0; j--){
                vcoluna[i]++;
            }

            if(jog->vjogs > 0){
                pointsWon[i]++;
            }
            if(vlinha[i] && vcoluna[i]){
                pointsWon[i]++;
            }

            pointsWon[i] += vlinha[i];
            if(vlinha[i] + 1 == 6){
                qwirkle = 1;
                pointsWon[i] += 6;
            }
            pointsWon[i] += vcoluna[i];
            if(vcoluna[i] + 1 == 6){
                qwirkle = 1;
                pointsWon[i] += 6;
            }
        }

        if(qwirkle){
            printf("\033[1;31mQ\033[0mW\033[1;32mI\033[1;33mR\033[1;34mK\033[1;35mL\033[1;36mE\033[0m!!!\n");
        }

        int max = 0;
        for(int i = 0; i < jog->vjogs; i++){
            if(pointsWon[i] > max){
                max = pointsWon[i];
            }
        }
        printf("Pontos ganhos: +%d\n", max);
        jog->points += max;
    } else {
        printf("Pontos ganhos: +0\n");
    }
}


int main(int argc, char *argv[]){
    npecas = 108;
    xMax = 1;
    xMin = 0;
    yMax = 1;
    yMin = 0;
    randomizarPecas();
    srand(time(NULL));
    int maxX = 6; //tamanho real da matriz
    int maxY = 6; //guardam sempre o mesmo valor
    peca **grid;
    grid = (peca **) malloc(sizeof(peca *) * maxX);
    if(grid == NULL){
        printf("Erro de alocacao de memoria\n");
        exit(1);
    }
    for(int i = 0; i < maxX; i++){
        grid[i] = (peca *) malloc(sizeof(peca) * maxY);
        if(grid == NULL){
            printf("Erro de alocacao de memoria\n");
            exit(1);
        }
    }
    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
            grid[i][j].form = ' ';
            grid[i][j].cor = ' ';
        }
    }

    int nJogs;
    printf("Digite numero de jogadores: \n");
    scanf("%d", &nJogs);
    getchar();//limpar o buffer do \n
    jogador jog[nJogs];
    for(int i = 0; i < nJogs; i++){
        jog[i].points = 0;
        printf("Digite nome do jogador %d\n", i);
        fgetss(jog[i].nome, 20);
    }

    printf("Cheat mode(s/n)\n");
    cheatMode = getc(stdin);
    getchar();
    if(cheatMode > 'A' && cheatMode < 'Z'){
	    cheatMode = cheatMode - 'A' + 'a';
    }

    for(int i = 0; i < nJogs; i++){ //randomizar as peças de cada jogador e tira-las do allPecas(saco de peças)
        for(int j = 0; j < 6; j++){
            int a = rand() % 108;
            while(allPecas[a].form == ' ' && allPecas[a].cor == ' '){
                a = rand() % 108;
            }
            jog[i].jogadas[j].form = allPecas[a].form;
            jog[i].jogadas[j].cor = allPecas[a].cor;
            allPecas[a].form = ' ';
            allPecas[a].cor = ' ';
        }
    }
    
    int flag = 0; //garantir que saira do loop quando o jogo acabar
    while(npecas > 0 && !flag){
        for(int i = 0; i < nJogs; i++){
            char input[20];
            jog[i].vjogs = 0;
            int acabou = 0;
            for(int j = 0; j < 6; j++){ //repor as pecas que o jogador tinha faltando apos usa-las no ultimo turno
                if(jog[i].jogadas[j].form == ' ' || jog[i].jogadas[j].cor == ' '){
                    int a = rand() % 108;
                    acabou++;
                    while(allPecas[a].form == ' ' || allPecas[a].cor == ' '){
                        a = rand() % 108;
                        acabou++; //caso demore muito para achar pecas, ele entra em um loop e ve se ainda tem alguma peca
                        if(acabou == 108){// se ainda tiver o jogador recebe essa peca
                            int k;
                            for(k = 0; k < 108; k++){
                                if(allPecas[k].form != ' ' && allPecas[k].cor != ' '){
                                    a = k;
                                    break;
                                }
                            }
                            if(k == 108){
                                flag = 1; //se nenhuma peça foi encontrada no loop, sair de todos os loop
                                break;
                            }
                        } 
                    }
                    jog[i].jogadas[j].form = allPecas[a].form;
                    jog[i].jogadas[j].cor = allPecas[a].cor;
                    allPecas[a].form = ' ';
                    allPecas[a].cor = ' ';
                }
            }
            if(flag){
                break;
            }
            //agora aloca memoria para guardar todas as jogadas do player no turno
            int max = 6;
            jog[i].lastPlay = (char **) malloc(sizeof(char *) * max);
            if(jog[i].lastPlay == NULL){
                printf("Erro de alocacao de memoria\n");
                exit(1);
            }
            for(int j = 0; j < max; j++){
                jog[i].lastPlay[j] = (char *) malloc(sizeof(char) * 20);
                if(jog[i].lastPlay[j] == NULL){
                    printf("Erro de alocacao de memoria\n");
                    exit(1);
                }
            }

            do{

                if(xMax - xMin > maxX - 2 || yMax - yMin >= maxY - 2){ //aumentar o tamanho real do tabuleiro se necessario
                    maxX *= 2;
                    grid = (peca **) realloc(grid, sizeof(peca *) * maxX);
                    if(grid == NULL){
                        printf("Erro de alocacao de memoria\n");
                        exit(1);
                    }
                    maxY *= 2;
                    for(int j = 0; j < maxX; j++){
                        grid[j] = (peca *) realloc(grid[j], sizeof(peca) * maxY);
                        if(grid[j] == NULL){
                            printf("Erro de alocacao de memoria\n");
                            exit(1);
                        }
                    }
                    for(int j = 0; j < maxX; j++){ 
                        //colocar um espaço vazio em todos os lugares que nao tem peças caso aumente o tabuleiro
                        for(int k = 0; k < maxY; k++){
                            int l;
                            for(l = 0; l < 108; l++){
                                if(grid[j][k].form == allPecas[l].form && grid[j][k].cor == allPecas[l].cor){
                                    break;
                                }
                            }
                            if(l == 108){
                                grid[j][k].form = ' ';
                                grid[j][k].cor = ' ';
                            }
                        }
                    }
                }

                GameState(grid);
                gameMove(grid, &(jog[i]), input);

                if(jog[i].vjogs >= max){ //aumentar memoria para guardar as jogadas do player, se necessario
                    max += jog[i].vjogs;
                    jog[i].lastPlay = (char **) realloc(jog[i].lastPlay, sizeof(char *) * max);
                    if(jog[i].lastPlay == NULL){
                        printf("Erro de alocacao de memoria\n");
                        exit(1);
                    }
                    for(int j = 0; j < 6; j++){
                        jog[i].lastPlay[j] = (char *) realloc(jog[i].lastPlay[j], sizeof(char) * 20);
                        if(jog[i].lastPlay[j] == NULL){
                            printf("Erro de alocacao de memoria\n");
                            exit(1);
                        }
                    }
                }

            } while(input[0] != 'p');

            checkPoints(grid, &(jog[i]));

            for(int j = 0; j < max; j++){ //liberar a memoria alocada para as jogadas do player
                free(jog[i].lastPlay[j]);
            }
            free(jog[i].lastPlay); 

        }
    }
    int win = 0;
    for(int i = 0; i < nJogs; i++){
        if(jog[i].points > win){
            win = i;
        }
    }
    printf("O vencedor foi o Jogador %d de nome %s\n", win, jog[win].nome);

    for(int i = 0; i < maxX; i++){
        free(grid[i]);
    }
    free(grid); //liberar memoria do tabuleiro
}
