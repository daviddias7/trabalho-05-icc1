#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void GameState(char grid[5][5]){
    for(int i = 0; i < 5; i++){
        printf("|");
        for(int j = 0; j < 5; j++){
            printf("|");
            printf(" aa ");
            printf("|");
        }
        printf("|\n");
    }
}

int main(int argc, char *argv[]){
    char grid[5][5];
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            grid[i][j] = ' ';
        }
    }
    GameState(grid);
    char pecas[6][2];
    char f = 'a';
    for(int i = 0; i < 6; i++){
        char a = '1';
        for(int j = 0; j < 6; j++){
            pecas[i][0] = f;
            pecas[i][1] = a;
            a++;
        }
        f++;
    }
    int npecas = 108;
    while(npecas > 0){
        npecas--;
    }
}
