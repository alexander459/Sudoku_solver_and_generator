# Sudoku_solver_and_generator
Language: C

This is a sudoku solver and generator project. Uses a grid ADT to representate the grid of the sudoku in grid.c/grid.h 
and implements solve fuctions and main in grid.c/grid.h.
Usage: ./sudoku
       Reads a puzzle from stdin and solves it 
       ./sudoku -c
       Reads a puzzle from stdin and checks if it is correct. If it is not prints errors in stderr
       ./sudoku -g nelts -u
       Generates a random puzzle with nelts blank cells. Parameter -u will generate a puzzle with a unique solution
