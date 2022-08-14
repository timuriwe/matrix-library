CC=gcc
FLAGS=-Wall -Werror -Wextra -std=c11

all: matrix.a

clean: 
	rm -f *.o *.a

matrix.a: matrix.o
	ar rc libmatrix.a matrix.o

matrix.o: matrix.c
	$(CC) $(FLAGS) -std=c11 -c matrix.c
