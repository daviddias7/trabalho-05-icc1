#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct peca {
    char form;
    char cor;
} peca;

typedef struct jogador{
    char nome[20];
    peca jogadas[6];
    int points;
    int vjogs;
    char **lastPlay;
} jogador;

char formatos[6];
char cores[6];
int npecas = 108;
peca allPecas[108];
char cheatMode;

void colorize(peca pecaCor){
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

void randomizarPecas(){
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


void fgetss(char str[], int n){
    for(int i = 0; i < n; i++){
        str[i] = '1';
    }
    char a;
    int i;
    for(i = 0; i < n; i++){
        a = getc(stdin);
        if(a == '\n'){
            break;
        }
        str[i] = a;  
    }
    str[i] = '\0';
    while(a != '\n'){
        a = getc(stdin);
    }
}

void GameState(peca grid[108][108]){
    int max = 55;
    int min = 54;
    int i;
    for(i = 0; i < 108; i++){
        for(int j = 0; j < 108; j++){
            if(grid[i][j].form != ' '){
                if(i >= max-1 || j >= max-1 || i <= min+1 || j <= min+1){
                    min--;
                    max++;
                }
            }
        }
    }
    for(i = min; i < max; i++){
        if(i == min){
            printf("   %d  ", i-54);
        } else {
            printf(" %d  ", i-54);
        }
    }
    printf("\n");
    for(i = min; i < max; i++){
        printf("%d", i - 54);
        printf("|");
        for(int j = min; j < max; j++){
            colorize(grid[i][j]);
            printf("%c%c ", grid[i][j].form, grid[i][j].cor);
            printf("\033[0m");
            printf("|");
        } 
        printf("\n");
        for(int j = min; j < max; j++){
            if(j == 0){
                printf("  ---");
            } else {
                printf(" ---");
            }
        }
        printf("\n");
    }
}

void atoii(int *x, int *y, char input[20]){
    int cnt = 0;
    char posx[6];
    char posy[6];
    for(int i = 0; i < 6; i++){
        posy[i] = 'a';
    }
    posy[5] = '\0';
    int negx = 0;
    int negy = 0;
    for(int i = 0; i < 20; i++){
        if(input[i] == ' '){
            cnt++;
        } else {
            if(cnt == 2){
                int kx = 0;
                int ky = 0;
                for(int j = i; j < 20; j++){
                    if(input[j] == '\0'){
                        break;
                    }
                    if(input[j] == ' '){
                        cnt++;
                    } else {
                        if(cnt == 2){
                            if(input[j] == '-'){
                                negx = 1;
                            } else {
                                posx[kx] = input[j];
                                kx++;
                            }
                        } else if(cnt >= 3){
                            if(input[j] == '-'){
                                negy = 1;
                            } else {
                                posy[ky] = input[j];
                                ky++;
                            }
                        }
                    }
                }
                posx[kx] = '\0';
                posy[ky] = '\0';
                break;
            }
        }
    }

    *x = atoi(posx);
    *y = atoi(posy);
    if(negx){
        *x *= -1;
    }
    if(negy){
        *y *= -1;
    }
    *x += 54;
    *y += 54;
}

void checkPoints(peca grid[108][108], jogador *jog){
    int pointsWon[jog->vjogs];
    int xAnt[jog->vjogs];
    int yAnt[jog->vjogs];
    int vlinha[jog->vjogs];
    int vcoluna[jog->vjogs];/// maximiza seu pontos
    int qwirkle = 0;
    for(int i = 0; i < jog->vjogs; i++){
        pointsWon[i] = 0;
        vlinha[i] = 0;
        vcoluna[i] = 0;
        atoii(&(xAnt[i]), &(yAnt[i]), jog->lastPlay[i]);    
        for(int j = xAnt[i]+1; grid[j][yAnt[i]].form != ' ' && j < 108; j++){
            vlinha[i]++;
        }

        for(int j = xAnt[i]-1; grid[j][yAnt[i]].form != ' ' && j >= 0; j--){
            vlinha[i]++;
        }

        for(int j = yAnt[i]+1; grid[xAnt[i]][j].form != ' ' && j < 108; j++){
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
            max = i;
        }
    }
    printf("Pontos ganhos: +%d\n", pointsWon[max]);
    jog->points += pointsWon[max];
}

int validMove(peca grid[108][108], jogador *jog, char input[20]){
    printf("validmove\n");
    int x;
    int y;
    atoii(&x, &y, input);

    peca pecaJogada;
    pecaJogada.form = input[2];
    pecaJogada.cor = input[3];

    if(npecas == 108){
        if(x != 54 || y != 54){
            printf("Movimento invalido\n");
            return 0;
        }
        for(int i = 0; i < 6; i++){
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
        printf("Movimento invalido\n");
        return 0;
    }

    if(grid[x][y].form != ' '){
        printf("Posicao ocupada\n");
        return 0;
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////

    for(int i = x+1; i < 108; i++){
        if(pecaJogada.form == grid[i][y].form && pecaJogada.cor == grid[i][y].cor){
            printf("Movimento invalido\n");
            return 0;
        }
    }


    for(int i = x-1; grid[i][y].form != ' ' && i >= 0; i--){
        if(pecaJogada.form == grid[i][y].form && pecaJogada.cor == grid[i][y].cor){
            printf("Movimento invalido\n");
            return 0;
        }
    }

    for(int i = y+1; grid[x][i].form != ' ' && i < 108; i++){
        if(pecaJogada.form == grid[x][i].form && pecaJogada.cor == grid[x][i].cor){
            printf("Movimento invalido\n");
            return 0;
        }
    }
    for(int i = y-1; grid[x][i].form != ' ' && i >= 0; i--){
        if(pecaJogada.form == grid[x][i].form && pecaJogada.cor == grid[x][i].cor){
            printf("Movimento invalido\n");
            return 0;
        }
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int vazio = 0;

    if(grid[x+1][y].form != ' ' || grid[x+1][y].cor != ' '){
        if((pecaJogada.form != grid[x+1][y].form && pecaJogada.cor != grid[x+1][y].cor)){
            printf("Movimento invalido\n");
            return 0;
        }
    } else {
        vazio++;  
    }

    if(grid[x-1][y].form != ' ' || grid[x-1][y].cor != ' '){
        if((pecaJogada.form != grid[x-1][y].form && pecaJogada.cor != grid[x-1][y].cor)){
            printf("Movimento invalido\n");
            return 0;
        }
    } else {
        vazio++;  
    }

    if(grid[x][y+1].form != ' ' || grid[x][y+1].cor != ' '){
        if((pecaJogada.form != grid[x][y+1].form && pecaJogada.cor != grid[x][y+1].cor)){
            printf("Movimento invalido\n");
            return 0;
        }
    } else {
        vazio++;  
    }

    if(grid[x][y-1].form != ' ' || grid[x][y-1].cor != ' '){
        if((pecaJogada.form != grid[x][y-1].form && pecaJogada.cor != grid[x][y-1].cor)){
            printf("Movimento invalido\n");
            return 0;
        }
    } else {
        vazio++;  
    }

    if(vazio >= 4){
        printf("Movimento invalido\n");
        return 0;
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////

    if(jog->vjogs > 0){
        int xAnt[2];
        int yAnt[2];
        for(int i = 0; i < 2; i++){
            atoii(&(xAnt[i]), &(yAnt[i]), jog->lastPlay[i]);
        }

        if(jog->vjogs >= 2){
            if(xAnt[0] == xAnt[1] && x != xAnt[0]){
                printf("Movimento invalido\n");
                return 0;
            }
            if(yAnt[0] == yAnt[1] && y != yAnt[0]){
                printf("Movimento invalido\n");
                return 0;
            }
        }   
    }
        
    for(int i = 0; i < 6; i++){
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

int validCommand(peca grid[108][108], jogador *jog, char input[20]){
    printf("validcomand\n");
    switch(input[0]){
        case 'p':
            return 1;
        case 't':
            if(jog->vjogs == 0){
                if(input[2] == 'a' && (input[3] == '\0' || input[3] == ' ')){
                    for(int i = 0; i < 6; i++){
                        jog->jogadas[i].form = formatos[rand() % 6];
                        jog->jogadas[i].cor = cores[rand() % 6];
                    }
                    return 1;
                }
                for(int i = 0; i < 6; i++){
                    if(input[2] == jog->jogadas[i].form && input[3] == jog->jogadas[i].cor){
                        jog->jogadas[i].form = formatos[rand() % 6];
                        jog->jogadas[i].cor = cores[rand() % 6];
                        return 1;
                    }
                }
                printf("Comando invalido\n");
                return 0;
            } else {
                printf("Comando invalido\n");
                return 0;
            }
        case 'j':
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
            printf("Comando invalido\n");
            return 0;
        default:
            printf("Comando invalido\n");
            return 0;
    }

}

void gameMove(peca grid[108][108], jogador *jog, char input[20]){
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

    printf("Digite o que quer fazer:\n");
    char a;
    int cnt = 0;
    int flag = 0;
    input[0] = ' ';
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

    if(!validCommand(grid, jog, input)){
        GameState(grid);
        gameMove(grid, jog, input);
    }
    if(input[0] == 'j'){
        int x;
        int y;
        atoii(&x, &y, input);
        grid[x][y].form = input[2];
        grid[x][y].cor = input[3];
        npecas--;
        strcpy(jog->lastPlay[jog->vjogs], input);
        jog->vjogs++;
    }
    return;
}

int main(int argc, char *argv[]){
    randomizarPecas();
    srand(time(NULL));
    peca grid[108][108];
    for(int i = 0; i < 108; i++){
        for(int j = 0; j < 108; j++){
            grid[i][j].form = ' ';
            grid[i][j].cor = ' ';
        }
    }

    int nJogs;
    printf("Digite numero de jogadores: \n");
    scanf("%d", &nJogs);
    getchar();
    jogador jog[nJogs];
    for(int i = 0; i < nJogs; i++){
        jog[i].points = 0;
        printf("Digite nome do jogador %d\n", i);
        fgetss(jog[i].nome, 20);
    }

    printf("Cheat mode(s/n)\n");
    cheatMode = getc(stdin);
    getchar();

    for(int i = 0; i < nJogs; i++){
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

    int flag = 1;
    while(npecas > 0 && flag){
        for(int i = 0; i < nJogs; i++){
            char input[20];
            jog[i].vjogs = 0;
            int acabou = 0;
            for(int j = 0; j < 6; j++){
                if(jog[i].jogadas[j].form == ' ' || jog[i].jogadas[j].cor == ' '){
                    int a = rand() % 108;
                    acabou++;
                    while(allPecas[a].form == ' ' || allPecas[a].cor == ' '){
                        a = rand() % 108;
                        acabou++;
                        if(acabou == 108){
                            for(int k = 0; k < 108; k++){
                                if(allPecas[k].form != ' ' && allPecas[k].cor != ' '){
                                    a = k;
                                    break;
                                }
                            }
                        flag = 0;
                        break;
                        } 
                    }
                    jog[i].jogadas[j].form = allPecas[a].form;
                    jog[i].jogadas[j].cor = allPecas[a].cor;
                    allPecas[a].form = ' ';
                    allPecas[a].cor = ' ';
                }
            }
            if(!flag){
                break;
            }
            int max = 6;
            jog[i].lastPlay = (char **) malloc(sizeof(char *) * max);
            if(jog[i].lastPlay == NULL){
                printf("Erro de alocacao de memoria\n");
                exit(1);
            }
            for(int j = 0; j < 6; j++){
                jog[i].lastPlay[j] = (char *) malloc(sizeof(char) * 20);
                if(jog[i].lastPlay[j] == NULL){
                    printf("Erro de alocacao de memoria\n");
                    exit(1);
                }
            }
            do{
                GameState(grid);
                gameMove(grid, &(jog[i]), input);
                if(jog[i].vjogs >= max){
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
        }
    }
    int max = 0;
    for(int i = 0; i < nJogs; i++){
        if(jog[i].points > max){
            max = i;
        }
    }
    printf("O vencedor foi o Jogador %d de nome %s\n", max, jog[max].nome);
    for(int i = 0; i < nJogs; i++){
        free(jog[i].lastPlay[i]);
    }
}
