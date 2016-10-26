PROG = run.sh
COMP = main.o printer.o segmentation.o
SRC = main.c printer.c segmentation.c
CFLAGS = -g -std=gnu99 -fPIC -m64 -Wall
LFLAGS = -std=gnu99 -fPIC -m64 -Wall
CC = mpicc
LIBS = -lm

all:
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LFLAGS) $(COMP) -o $(PROG) $(LIBS)  

run: ./$(PROG)
	mpirun -np ${NPROC} $(PROG) ${ARGS}
	
