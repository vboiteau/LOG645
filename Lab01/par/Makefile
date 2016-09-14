PROG = run.sh
COMP = lab1.o
SRC = lab1.c
CFLAGS = -g -fPIC -m64 -Wall
LFLAGS = -fPIC -m64 -Wall
CC = mpicc

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LFLAGS) $(COMP) -o $(PROG)

run: ./$(PROG)
	mpirun -np ${NPROC} $(PROG) ${ARGS}
	
