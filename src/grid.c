#include "grid.h"

static void zero_memset(int *arr, int size){
  int i;
  for(i=0; i<size; i++)
    arr[i]=0;
  return;
}

void grid_update_value(Grid_T *g, int i, int j, int n){
  assert(g!=NULL);
  g->elts[i][j].val=n;
  return;
}

int grid_read_value(Grid_T g, int i, int j){
  return g.elts[i][j].val;
}

void grid_set_choice(Grid_T *g, int i, int j, int n){
  assert(g!=NULL);
  g->elts[i][j].choices.num[n]=1;
  return;
}

void grid_clear_choice(Grid_T *g, int i, int j, int n){
  assert(g!=NULL);
  g->elts[i][j].choices.num[n]=0;
  return;
}

int grid_choice_is_valid(Grid_T g, int i, int j, int n){
  return g.elts[i][j].choices.num[n];
}

void grid_remove_choice(Grid_T *g, int i, int j, int n){
  assert(g!=NULL);
  if(grid_choice_is_valid(*g, i, j, n))
    g->elts[i][j].choices.count=g->elts[i][j].choices.count-1;
  g->elts[i][j].choices.num[n]=0;
  return;
}

int grid_read_count(Grid_T g, int i, int j){
  return g.elts[i][j].choices.count;
}

void grid_set_count(Grid_T *g, int i, int j){
  assert(g!=NULL);
  g->elts[i][j].choices.count=9;
  return;
}

void grid_clear_count(Grid_T *g, int i, int j){
  assert(g!=NULL);
  g->elts[i][j].choices.count=0;
  return;
}

int grid_read_unique(Grid_T g){
  return g.unique;
}

void grid_set_unique(Grid_T *g){
  assert(g!=NULL);
  g->unique=1;
  return;
}

void grid_clear_unique(Grid_T *g){
  assert(g!=NULL);
  g->unique=0;
  return;
}

int grid_number_of_blanks(Grid_T g){
  int i, j, count=0;
  for(i=0; i<9; i++)
    for(j=0; j<9; j++)
      if(g.elts[i][j].val==0)
	count++;
  return count;
}

void grid_set_all_choices(Grid_T *g, int i ,int j){
  int k;
  assert(g!=NULL);
  for(k=0; k<10; k++)
    g->elts[i][j].choices.num[k]=1;
  return;
}

void grid_clear_all_choices(Grid_T *g, int i ,int j){
  int k;
  assert(g!=NULL);
  for(k=0; k<10; k++)
    g->elts[i][j].choices.num[k]=0;
  return;
}

int grid_can_place_value(Grid_T g, int i, int j, int n){
  int k, m;
  int subGrid_i;
  int subGrid_j;
  /*check the row*/
  for(k=0; k<9; k++){
    if(grid_read_value(g, i, k)==n)
      return 0;  /*you can not place this value in the cell i, j :(*/
  }
  /*check the column*/
  for(k=0; k<9; k++){
    if(grid_read_value(g, k, j)==n)
      return 0;   /*you can not place this value in the cell i, j :(*/
  }
  /*check the sub-grid*/
  subGrid_i=(i/3)*3;
  subGrid_j=(j/3)*3;
  for(k=subGrid_i; k<subGrid_i+3; k++){
    for(m=subGrid_j; m<subGrid_j+3; m++){
      if(grid_read_value(g, k, m)==n)
	return 0;   /*you can NOT place this value in the cell i, j :(*/
    }
  }
  return 1;      /*you CAN place this value in the cell i, j :)*/
}

int grid_check_rows(Grid_T g, char choice){
  int array[9];
  int i, j, number;
  /*check the rows*/
  for(i=0; i<9; i++){
    zero_memset(array, 9);
    for(j=0; j<9; j++){
      number=grid_read_value(g, i, j);
      if(number==0)
	continue;             /*skip blanks*/
      if(array[number-1]==1){  /*this number allready exists in the row or there is a blank cell*/
	switch(choice){
	case 'd':
	  return 0;                         /*if option is dont print, just stop and return 0*/
	  break;
	case 'p':
	  fprintf(stderr, "The number %d allready exists in row %d\n", number, i+1);
	  array[number-1]=2;                /*error for this number has been already printed*/
	  break;
	default:
	  assert(0 && "in grid_check_rows choise is not p or d\n");
	}
      }else if(array[number-1]==0)
	array[number-1]=1;              /*mark the number as found in the row*/
    }
  }
  return 1;
}

int grid_check_columns(Grid_T g, char choice){
  int array[9];
  int i, j, number;
  /*check the columns*/
  for(j=0; j<9; j++){
    zero_memset(array, 9);
    for(i=0; i<9; i++){
      number=grid_read_value(g, i, j);
      if(number==0)
	continue;                            /*skip the blanks*/
      if(array[number-1]==1){ /*this number allready exists in the column or there is a blank cell*/
	switch(choice){
	case 'd':
	  return 0;                     /*if option is dont print, just stop and return 0*/
	  break;
	case 'p':
	  fprintf(stderr, "The number %d allready exists in column %d\n", number, j+1);
	  array[number-1]=2;            /*error for this number has been already printed*/
	  break;
	default:
	  assert(0 && "in grid_check_columns choise is not p or d\n");
	}
      }else if(array[number-1]==0)
	array[number-1]=1;              /*mark the number as found in the column*/
    }
  }
  return 1;
}

int grid_check_subgrids(Grid_T g, char choice){
  int array[9];
  int i, j, sg, subGrid_i, subGrid_j, number;
  /*check the sub-grids*/
  subGrid_i=0;
  subGrid_j=0;
  for(sg=0; sg<9; sg++){
    zero_memset(array, 9);
    for(i=subGrid_i; i<subGrid_i+3; i++){
      for(j=subGrid_j; j<subGrid_j+3; j++){
	number=grid_read_value(g, i, j);
	if(number==0)
	  continue;                       /*skip blanks. They have been allready checked in rows and columns*/
	if(array[number-1]==1){           /*this number allready exists in the sub-grid*/
	  switch(choice){
	  case 'd':
	    return 0;                     /*if option is dont print, just stop and return 0*/
	    break;
	  case 'p':
	    fprintf(stderr, "The number %d allready exists in sub-grid %d\n", number, sg+1);
	    array[number-1]=2;            /*error for this number has been allready printed*/
	    break;
	  default:
	    assert(0 && "in grid_check_subgrids choise is not p or d\n");
	  }
	}else if(array[number-1]==0)
	  array[number-1]=1;              /*mark the number as found in the sub-grid*/
      }
    }
    switch((sg+1)/3){
    case 0:    /*first row of sub grids*/
      subGrid_j=subGrid_j+3;
      break;
    case 1:    /*second row of sub grids*/
      subGrid_i=3;
      if((sg+1)%3==0)
	subGrid_j=0;
      else
	subGrid_j=subGrid_j+3;
      break;
    case 2:    /*third row of sub grids*/
      subGrid_i=6;
      if((sg+1)%3==0)
	subGrid_j=0;
      else
	subGrid_j=subGrid_j+3;
      break;
    default:
      break;
    }
  }
  return 1;
}

void grid_remove_choice_from_row(Grid_T *g, int row, int col, int n){
  int j;
  assert(g!=NULL);
  for(j=0; j<9; j++)    /*for every column of the row*/
    grid_remove_choice(g, row, j, n);
  return;
}

void grid_remove_choice_from_column(Grid_T *g, int row, int col, int n){
  int i;
  assert(g!=NULL);
  for(i=0; i<9; i++)    /*for every row of the column*/
    grid_remove_choice(g, i, col, n);
  return;
}

void grid_remove_choice_from_subgrid(Grid_T *g, int row, int col, int n){
  int subGrid_i, subGrid_j, i, j;
  assert(g!=NULL);
  subGrid_i=(row/3)*3;
  subGrid_j=(col/3)*3;
  for(i=subGrid_i; i<subGrid_i+3; i++){
    for(j=subGrid_j; j<subGrid_j+3; j++){
      grid_remove_choice(g, i, j, n);
    }
  }
  return;
}

int grid_find_unique_solution_cell(Grid_T g, int *i, int *j){
  int k, m, l, found;
  srand(getpid());
  found=0;
  for(k=0; k<9 && !found; k++)         /*first check if there is at last one cell with unique solution*/
    for(m=0; m<9 && !found; m++)
      if(grid_read_count(g, k, m)==1)  /*there is at least one unique cell*/
    found=1;
  if(!found)                           /*no cell with unique solution found so return 0*/
    return 0;
  
  do{                                  /*select one random cell with unique solution*/
    k=rand()%9;
    m=rand()%9;
  }while(grid_read_count(g, k, m)!=1);

  if(i!=NULL && j!=NULL){              /*a random cell with unique solution found*/
    *i=k;                              /*return the cell with the unique solution*/
    *j=m;
  }
  for(l=0; l<10; l++)
    if(grid_choice_is_valid(g, k, m, l))
      return l;                        /*return the unique solution*/
  assert(0);
}

void grid_return_cell_with_min_count(Grid_T g, int *row, int *col){
  int i, j, min=10;
  for(i=0; i<9; i++){
    for(j=0; j<9; j++){                   /*for every cell*/
      if(grid_read_value(g, i, j)!=0)
    continue;                         /*skip the filled cells. They always have count zero*/
      if(grid_read_count(g, i, j)<min){   /*find the cell with the minimum count*/
    min=grid_read_count(g, i, j);
    *row=i;
    *col=j;
      }
    }
  }
  return;
}

int grid_get_random_blank(Grid_T g, int *row, int *col){
  int i, j;
  srand(getpid());
  if(grid_number_of_blanks(g)==0)
    return 0;
  do{
    i=rand()%9;
    j=rand()%9;
  }while(grid_read_value(g, i, j)!=0);
  *row=i;
  *col=j;
  return 1;
}

void grid_zero_puzzle(Grid_T *g){
  int i, j;
  assert(g!=NULL);
  for(i=0; i<9; i++)
    for(j=0; j<9; j++)
      grid_update_value(g, i, j, 0);
  return;
}
