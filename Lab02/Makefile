PROG = run.sh
COMP = lab1.o
SRC = lab1.c
CFLAGS = -g -std=c99 -fopenmp -fPIC -m64 -Wall
LFLAGS = -fPIC -m64 -Wall
CC = gcc

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LFLAGS) $(COMP) -o $(PROG)

run: ./$(PROG)
	./$(PROG) ${ARGS}
	
