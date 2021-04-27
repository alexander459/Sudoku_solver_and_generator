CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

all: sudoku

sudoku: src/sudoku.o src/grid.o
	$(CC) $(CFLAGS) $^ -o $@

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-rm -f sudoku ./src/*.o
