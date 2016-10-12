PROG = run.sh
COMP = lab2.o
SRC = lab2.c
CFLAGS = -g -std=gnu99 -fopenmp -fPIC -m64 -Wall
LFLAGS = -std=gnu99 -fopenmp -fPIC -m64 -Wall
CC = gcc

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LFLAGS) $(COMP) -o $(PROG)

run: ./$(PROG)
	./$(PROG) ${ARGS}
	
