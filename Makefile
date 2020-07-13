game.o : game.c
	gcc -o game.o -c game.c

general.o : general.c
	gcc -o general.o -c general.c

main.o : main.c
	gcc -o main.o -c main.c

main : main.o  game.o general.o
	gcc -o main game.o main.o general.o

all: main

run : 
	./main

clean:
	rm *.o
