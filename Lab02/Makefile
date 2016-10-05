PROG = run.sh
COMP = lab2.o
SRC = lab2.c
CFLAGS = -g -std=c99 -fopenmp -fPIC -m64 -Wall
LFLAGS = -std=c99 -fopenmp -fPIC -m64 -Wall
CC = gcc

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LFLAGS) $(COMP) -o $(PROG)

run: ./$(PROG)
	./$(PROG) ${ARGS}
	
