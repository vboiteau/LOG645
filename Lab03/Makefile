PROG = run.sh
COMP = main.o
SRC = main.c
CFLAGS = -g -fPIC -m64 -Wall
LFLAGS = -fPIC -m64 -Wall
CC = mpicc
LIBS = -lm

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LFLAGS) $(COMP) -o $(PROG) $(LIBS)

run: ./$(PROG)
	mpirun -np ${NPROC} $(PROG) ${ARGS}
	
