#include<stdio.h>
#include<stdlib.h>

void fgetss(char str[], int n){ //uma funçao para receber inputs que gera menos erros de buffer
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

void atoii(int *x, int *y, char input[20]){ //funçao que transforma o x e y do input do jogador em inteiros a serem usados
    int cnt = 0;
    char posX[6];
    char posY[6];
    for(int i = 0; i < 6; i++){
        posY[i] = 'a'; //apenas inicializar para nao terem problemas
    }
    posY[5] = '\0';
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
                                posX[kx] = input[j];
                                kx++;
                            }
                        } else if(cnt >= 3){
                            if(input[j] == '-'){
                                negy = 1;
                            } else {
                                posY[ky] = input[j];
                                ky++;
                            }
                        }
                    }
                }
                posX[kx] = '\0';
                posY[ky] = '\0';
                break;
            }
        }
    }
    *x = atoi(posX);
    *y = atoi(posY);
    if(negx){
	    *x *= -1;
    }
    if(negy){
	*y *= -1;
    }
}
