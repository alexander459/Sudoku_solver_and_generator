#include"sudoku.h"

/*initializes choices for every cell of the grid*/
static void sudoku_init_choices(Grid_T *g){
  int i, j, n;
  for(i=0; i<9; i++){                             /*access every cell*/
    for(j=0; j<9; j++){
      n=grid_read_value(*g, i, j);
      if(n!=0){                                   /*Cell is filled*/
	grid_clear_count(g, i, j);                /*so set count to zero and all choices as invalid*/
	grid_clear_all_choices(g, i, j);
      }else{                                      /*Else the cell is blank*/
	grid_set_count(g, i, j);                  /*so set count to 9 and all choices as valid*/
	grid_set_all_choices(g, i, j);
	grid_clear_choice(g, i, j, 0);            /*zero could never be a choice*/
	for(n=1; n<=9; n++){                      /*But some of the 9 choices may be invalid. Eliminate them*/
	  if(!grid_can_place_value(*g, i, j, n))  /*first check if the choice can be applied in cell i, j*/
	    grid_remove_choice(g, i, j, n);       /*if not, then remove it; its invalid*/
	}
      }
    }
  }
  return;
}

/*searches for a blank cell with unique solution. Returns that cell, 1, and the unique choice choice.
  If there is no such a cell it returns a random blank cell, 0, and  a random available choice for this cell
  If there is no cell with available choice it returns 0.*/
static int sudoku_try_next(Grid_T g, int *row, int *col){
  int i, j, n;
  assert(row!=NULL && "in sudoku_try_next row is null\n");
  assert(col!=NULL && "in sudoku_try_next col is null\n");
  if(grid_number_of_blanks(g)==0)
    return 0;                                      /*no blank cells*/
  n=grid_find_unique_solution_cell(g, &i, &j);     /*find a random cell with unique solution and store the solution in n*/
  if(n!=0){                                        /*if n is a unique solution for the cell*/
    *row=i;
    *col=j;
    return n;                                      /*return the unique solution and the cell*/
  }else{                                           /*else cell has no unique solution so return the cell with the less choices*/
    grid_return_cell_with_min_count(g, &i, &j);    /*take the cell with the min count*/
    *row=i;
    *col=j;
    if(grid_read_count(g, i, j)==0)                /*this blank cell has no available choices*/
      return 0;                                    /*dead end*/
  }
  srand(getpid());
  while(1){
    i=rand()%9+1;                                  /*take a random number from 1 to 9*/
    if(grid_choice_is_valid(g, *row, *col, i))
      return i;                                    /*return it if its valid to apply on the cell.
						     If the cell has unique solution then the l will take its value*/
  }
  assert(0 && "problem in sudoku_try_next. I am not supposed to be here...");
}

/*Takes a grid and a cell. It applies the value n at this cell and removes it from the choices.
  returns the number of the avilable choices for the cell*/
static int sudoku_update_choice(Grid_T *g, int i, int j, int n){
  assert(g!=NULL);
  grid_update_value(g, i, j, n);
  grid_remove_choice(g, i, j, n);
  return grid_read_count(*g, i, j);
}

/*Using grid.h functions, removes the choice n from its row, column and subgrid*/
static void sudoku_eliminate_choice(Grid_T *g, int r, int c, int n){
  assert(g!=NULL);
  grid_remove_choice_from_row(g, r, c, n);
  grid_remove_choice_from_column(g, r, c, n);
  grid_remove_choice_from_subgrid(g, r, c, n);
  return;
}

static Grid_T sudoku_generate_complete(void){
  Grid_T new_grid;
  grid_zero_puzzle(&new_grid);     /*create a blank puzzle (dont need to initialize it, it will be inited in sudoku_solve)*/
  new_grid=sudoku_solve(new_grid); /*solve the blank puzzle. There are many solutions os the puzzle will be random*/
  return new_grid;                 /*return the completed puzzle*/
}
  
Grid_T sudoku_read(void){
  int i, j;
  Grid_T new_grid;
  for(i=0; i<9; i++)
    for(j=0; j<9; j++)
      scanf("%d", &new_grid.elts[i][j].val);
  grid_set_unique(&new_grid);                     /*lets suppose it is unique. If it is not, that will change in solve*/
  return new_grid;
}

void sudoku_print(FILE *s, Grid_T g){
  int i, j;
  for(i=0; i<9; i++){
    for(j=0; j<9; j++)
      fprintf(s, "%d ", g.elts[i][j].val);
    fprintf(s, "\n");
  }
  return;
}

void sudoku_print_errors(Grid_T g){
  char op='p';                            /*p for "print errors"*/
  int i, j;
  grid_check_rows(g, op);
  grid_check_columns(g, op);
  grid_check_subgrids(g, op);
  for(i=0; i<9; i++)                      /*grid functions dont care about blanks so check for blanks here*/
    for(j=0; j<9; j++)
      if(!grid_read_value(g, i, j))
	fprintf(stderr, "%d,%d is blank\n", i+1, j+1);
  return;
}

int sudoku_is_correct(Grid_T g){
  char op='d';                            /*d for "dont print errors"*/
  /*if rows columns and subgrid are okay and if there are no blanks then sudoku is correct*/
  if(grid_check_rows(g, op) && grid_check_columns(g, op) && grid_check_subgrids(g, op) && !grid_number_of_blanks(g))
    return 1;
  return 0;
}

#define FOREVER for(;;)
Grid_T sudoku_solve(Grid_T g){
  int i, j, n, unique;                          /*unique will store 0 if n is unique solution and 1 if not*/
  Grid_T temp;
  sudoku_init_choices(&g);                      /*initialize it*/
  FOREVER{                                  
    n=sudoku_try_next(g, &i, &j);               /*try to find a cell with at last one available choice*/
    if(!n)                                      /*if there are no cells with available choice there is no solution or maybe it is solved*/
      return g;                                 /*return g to solve it again of to terminate if it is solved*/
    
    unique=sudoku_update_choice(&g, i, j, n);   /*if the n was a unique solution, sudoku_update will return 0 
						  because there will be no more choices left*/
    if(unique==0){                              /*n was a unique solution*/
      sudoku_eliminate_choice(&g, i, j, n);     /*we are sure for the solution. Eliminate n from the other elements' choices*/
    }else{
      grid_clear_unique(&g);                    /*if sudoku has no cell with unique choices
						then sudoku has not unique solution*/      
      temp=sudoku_solve(g);                     /*else n was not unique solution so try to solve the new puzzle again*/
      if(sudoku_is_correct(temp)){
	return temp;                            /*if it has been solved return it*/
      }else{
	grid_update_value(&g, i, j, 0);
      }
    }
  }
  assert(0 && "problem in solve. I am not supposed to be here...\n");
}

int sudoku_solution_unique(Grid_T g){
  return grid_read_unique(g);
}

Grid_T sudoku_generate(int nelts){
  Grid_T g;
  int row, col;
  int blanked;                              /*blanked stores the number of the blank cells.
					      (its time consuming to call grid_number_of_blanks_ every time)*/
  srand(getpid());
  g=sudoku_generate_complete();
  blanked=0;
  if(nelts>81)
    nelts=81;
  do{
    row=rand()%9;                           /*select a random cell*/
    col=rand()%9;
    if(grid_read_value(g, row, col)!=0){    /*if it is filled*/
      grid_update_value(&g, row, col, 0);   /*make it blank it!*/
      blanked++;
    }
  }while(blanked!=nelts);                   /*stop when nelts cells are blank*/
  return g;
}

#include<string.h>
int main(int argc, char *argv[]){
  Grid_T g;
  int nelts, i;
  switch(argc){
  case 1:     /* ./sudoku */
    g=sudoku_read();          /*read a puzzle from stdin*/
    sudoku_print(stderr, g);  /*print the puzzle in stderr*/
    g=sudoku_solve(g);        /*solve the puzzle*/
    if(!sudoku_is_correct(g))
      fprintf(stderr, "sudoku has no solution\n");
    else if(!grid_read_unique(g))
      fprintf(stderr, "puzzle has not a unique solution\n");
    else
      fprintf(stderr, "puzzle has a unique solution\n");
    sudoku_print(stderr, g);  /*print the solved puzzle*/
    sudoku_print_errors(g);
    break;
  case 2:
    if(!strcmp("-c", argv[1])){  /* ./sudoku -c */
      g=sudoku_read();           /*read a puzzle from stdin*/
      sudoku_print(stderr, g);   /*print the puzzle in stderr*/
      if(sudoku_is_correct(g))   /*if correct print "correct"*/
	fprintf(stderr, "sudoku is correct\n");
      else{                      /*if not correct print the errors*/
	fprintf(stderr, "sudoku is not correct\n");
	sudoku_print_errors(g);
      }
    }
    break;
  case 3:
    /*empty case*/
  case 4:
    if(!strcmp("-g", argv[1])){               /* ./sudoku -g nelts */
      for(i=0; argv[2][i]!=0; i++){           /*check if the third argument is a number*/
	if(argv[2][i]<48 || argv[2][i]>57){
	  fprintf(stderr, "invalid input\n");
	  return 0;
	}
      }
      nelts=atoi(argv[2]);
      if(argc==4 && !strcmp("-u", argv[3])){             /*unique solution*/
	do{
	  g=sudoku_generate(nelts);
	}while(grid_read_unique(g));
      }else
	g=sudoku_generate(nelts);
      sudoku_print(stderr, g);
    }else
      fprintf(stderr, "invalid input\n");
    break;
  default:
    fprintf(stderr, "invalid input\n");
  }
  return 1;
}
