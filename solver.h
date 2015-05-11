/****************************************************************************\
 *	solver.h																*
 *																			*
 *	Author: F. Engelbrecht													*
\****************************************************************************/
#ifndef SOLVER_H_
#define SOLVER_H_

/****************************************************************************\
 *	Structures																*
\****************************************************************************/
typedef struct wight_list_t
{
	short value, wight;
	struct wight_list_t *next;

} wl;

typedef struct move_list_t
{
	char move;
	struct move_list_t *next;

} ml;

typedef struct best_case_list_t
{
	short **board;
	short weight, moves;
	char blank_y, blank_x;
	struct best_case_list_t *next;
	ml *move_list;

} bcl;

/****************************************************************************\
 *	Prototypes																*
\****************************************************************************/
void solve(bcl **start);
bcl *get_bcl(bcl **start);
bcl *move_up(bcl **start);
bcl *move_down(bcl **start);
bcl *move_left(bcl **start);
bcl *move_right(bcl **start);
char add_bcl(bcl **head, bcl **poss);
char get_board_size(FILE *pFile);
void free_bcl(bcl **head);
void free_board(short **board);
void free_move_list(ml *move_list);
short get_board_weight(short **board);
short pop_wight_list(wl **top, short value);
char check_state(short **board);
short **cpy_board(short **board);
void result(short **board, ml *list, char *y, char *x, char boolean,
			FILE *pFile);
void store_board(short **board, FILE *pFile);
void print_board(short **board);

char size;
int name;
short beat;
short *MAXMOVES;

#endif /* SOLVER_H_ */
