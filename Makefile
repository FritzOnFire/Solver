# Makefile
# Author Fritz-Herman Engelbrecht

COMPILER = gcc -O3
MPI = mpicc -O3
FLAGS = -g -Wall
LIBS = -lm

all: solver mpi_solver

solver:
	$(COMPILER) -o solver solver.c $(LIBS)

mpi_solver:
	$(MPI) -o mpi_solver mpi_solver.c $(LIBS)

clean:
	rm -f solver
	rm -f mpi_solver
	reset
