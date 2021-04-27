#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>

typedef struct grid_s{
  int unique;      /* if 1 after solving, puzzle has unique solution */
  struct{ 
    int val;       /* value of element i,j */
    struct{         
      int count;   /* number of possible choices for element i,j */ 
      int num[10]; /* map of choices: num[k]==1 if k is a valid choice
                      for element i,j, else num[k]=0 */
    }choices;     /* struct choices is used while solving the
		     puzzle and is otherwise undefined */
  }elts[9][9];    /* sudoku grid elements; 0<=i,j<=9 */
}Grid_T;


/* update value of i,j to n */
void grid_update_value(Grid_T *g, int i, int j, int n);

/* return value of i,j */
int grid_read_value(Grid_T g, int i, int j);

/* set (to 1) or clear (to 0) choice n for elt i, j */
void grid_set_choice(Grid_T *g, int i, int j, int n);
void grid_clear_choice(Grid_T *g, int i, int j, int n);

/* true if choice n for elt i,j is valid */
int grid_choice_is_valid(Grid_T g, int i, int j, int n);

/* remove n from choices of elt i,j and adjust count only if n is a
   valid choice for elt i,j */
void grid_remove_choice(Grid_T *g, int i, int j, int n);

/* return count, set (to 9), or clear (to 0) count for elt i, j */
int grid_read_count(Grid_T g, int i, int j);
void grid_set_count(Grid_T *g, int i, int j);
void grid_clear_count(Grid_T *g, int i, int j);

/* return val, set (to 1), or clear (to 0) unique flag for g */
int grid_read_unique(Grid_T g);
void grid_set_unique(Grid_T *g);
void grid_clear_unique(Grid_T *g);

/* set (to 1) or clear (to 0) all elements for elt i,j */
void grid_set_all_choices(Grid_T *g, int i ,int j);
void grid_clear_all_choices(Grid_T *g, int i ,int j);

/*clear the choice n from all elts i,j of the same row, column or subgrid*/
void grid_remove_choice_from_row(Grid_T *g, int row, int col, int n);
void grid_remove_choice_from_column(Grid_T *g, int row, int col, int n);
void grid_remove_choice_from_subgrid(Grid_T *g, int row, int col, int n);

/* returns the number of the blank cells */
int grid_number_of_blanks(Grid_T g);

/* true if the value n can be placed in the cell i, j */
int grid_can_place_value(Grid_T g, int i, int j, int n);

/* it returns a random qunique solution cell with its the solution
   or it returns 0 if there are no cells with unique solution */
int grid_find_unique_solution_cell(Grid_T g, int *i, int *j);

/* searches the grid for the cell with the less available choices and returns it */
void grid_return_cell_with_min_count(Grid_T g, int *row, int *col);

/* if choise==d (dont print errors) return 1 if all the rows, columns, subgrids are valid, 0 if not 
   if choise==p (print errors)      prints all errors and returns 1
   They dont care about empty cells*/
int grid_check_rows(Grid_T g, char choice);
int grid_check_columns(Grid_T g, char choice);
int grid_check_subgrids(Grid_T g, char choise);

/*returns a random blank cell and 1, or zero if there are no blank cells in the grid*/
int grid_get_random_blank(Grid_T g, int *row, int *col);

/*takes a puzzle g and make all the cells blank*/
void grid_zero_puzzle(Grid_T *g);
