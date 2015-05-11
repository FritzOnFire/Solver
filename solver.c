#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "solver.h"

#define MAXLINE 501

int main (int args, char* argv[])
{
	FILE *pFile = NULL;
	char boolean, i, j, x, y;
	short **loc_board, sum;
	bcl *head, *temp, *prune;
	clock_t begin, end;
	double time_spent;
	wl *top, *bot;
	long it = 0;

	begin = clock();

	if (args < 3) {
		printf("Not enough arguments.\n");
		printf("Format: ./solver <textfile> <0 | 1 | 2> [<output file>]\n");
		printf("0 = do not print states\n");
		printf("1 = print to terminal, 2 = print to file\n");
		return 1;
	}

	if (argv[2][0] == '2' && args < 4) {
		printf("Please specify output file.\n");
		return 1;
	}

	if (!(pFile = fopen(argv[1], "r"))) {
		printf("File not found\n");
		return 1;
	}

	size = get_board_size(pFile);

	fclose(pFile);

	pFile = fopen(argv[1], "r");

	head = malloc(sizeof(bcl));
	head->board = malloc(sizeof(short *) * size);
	head->moves = 0;
	head->move_list = malloc(sizeof(ml));
	head->move_list->move = -1;
	head->move_list->next = NULL;
	head->next = NULL;

	top = bot = malloc(sizeof(wl));

	for (i = 1; i < (size * size) - 1; i++) {
		bot->value = i;
		bot->wight = 0;
		bot->next = malloc(sizeof(wl));
		bot = bot->next;
	}

	bot->value = i;
	bot->wight = 0;
	bot->next = NULL;

	sum = 0;

	loc_board = head->board;

	for (i = 0; i < size; i++) {
		loc_board[i] = malloc(sizeof(short) * size);

		for (j = 0; j < size; j++) {
			fscanf(pFile, "%hd", &loc_board[i][j]);

			if (loc_board[i][j] == 0) {
				head->blank_y = y = i;
				head->blank_x = x = j;
			} else {
				sum += pop_wight_list(&top, loc_board[i][j]);
			}
		}
	}

	fclose(pFile);

	boolean = 0;

	if ((size * size) % 2 == 1 && sum % 2 == 0) {
		boolean = 1;
	}

	if ((size * size) % 2 == 0 && (sum + y) % 2 == 1) {
		boolean = 1;
	}

	if (boolean == 1) {

		loc_board = cpy_board(loc_board);

		switch (check_state(head->board)) {

		case 0:
			boolean = 0;
			break;

		case 1:
			boolean = 1;
			temp = head;
			break;

		case 2:
			printf("Something happend to the empty slot...\n");
			printf("Terminating...");
			boolean = 2;
			break;

		default:
			break;
		}

		MAXMOVES = malloc(sizeof(short));
		*MAXMOVES = (size + size) * size * size;

		while (boolean == 0) {
			solve(&head);

//			printf("Current Best!\n");
//			print_board(head->board);

			if (head == NULL) {
				printf("Ummmmm.....\n");
			}

			temp = head;
			beat = temp->weight;

//			printf("---------------------------\n");
			i = 0;

			if (temp->moves > 40) {
				printf("AWWWW YEA!!!\n");
			}

			while (boolean == 0 && temp != NULL && temp->weight == beat && i < 3) {
				//print_board(temp->board);

//				printf("%3d", temp->moves);

				switch (check_state(temp->board)) {

				case 0:
					temp = temp->next;
					break;

				case 1:
					boolean = 1;
					break;

				case 2:
					printf("Something happend to the empty slot...\n");
					printf("Terminating...");
					boolean = 2;
					break;

				default:
					break;
				}
				i++;
			}
//			printf("Iteration %ld\n", ++it);
//			printf("\n--------------------------\n");
			end = clock();
			/*if ((double) (end - begin) / CLOCKS_PER_SEC > 20.0) {
				printf("Took to long. This is how far I got:\n");
				print_board(head->board);
				break;
			}*/
		}

		if (boolean == 1) {
			if (argv[2][0] != '0') {
				boolean = argv[2][0] - '0';

				if (boolean == 1) {
					print_board (loc_board);
				} else {
					pFile = fopen(argv[3], "w");
						store_board (loc_board, pFile);
				}

				result(loc_board, temp->move_list, &y, &x, boolean,
							pFile);

				if (boolean == 2) {
					fclose(pFile);
				}
			}
			printf("Number of moves required to sort the puzzle: %hd\n",
					temp->moves);
		}

		free_board(loc_board);
	} else {
		printf("Unsolvable!\n");
		temp = head;
	}

	if (temp->move_list != NULL) {
		free_move_list(temp->move_list->next);
	}

	free_bcl(&head);
	free(MAXMOVES);

	end = clock();
	time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("Time taken to solve: %f seconds\n", time_spent);

	return 0;
}

/*void solve(bcl **start)
{
	bcl *head, *step, *temp;

	head = NULL;

	while (*start != NULL) {
		step = get_bcl(start);

		while (step != NULL) {
			temp = step;
			step = step->next;
			if (add_bcl(&head, &temp) == 0) {
				break;
			}
		}

		temp = (*start)->next;
		free_board((*start)->board);
		free(*start);
		*start = temp;
	}

	*start = head;
}*/

void solve(bcl **start)
{
	bcl *head, *temp;

	/* Send job */
	head = get_bcl(start);

	if (head == NULL) {
		printf("Super CUT!!\n");
	}
	/* Remove job from queue */
	temp = *start;
	*start = (*start)->next;
	free(temp);

	while (head != NULL) {
		temp = head;
		head = head->next;
		temp->next = NULL;
		add_bcl(start, &temp);
	}
}

bcl *get_bcl(bcl **start)
{
	bcl *head, **temp;

	head = NULL;
	temp = malloc(sizeof(bcl *));

	*temp = move_up(start);
	add_bcl(&head, temp);

	*temp = move_down(start);
	add_bcl(&head, temp);

	*temp = move_left(start);
	add_bcl(&head, temp);

	*temp = move_right(start);
	add_bcl(&head, temp);

	free(temp);

	return head;
}

bcl *move_up(bcl **start)
{
	bcl *temp;

	if ((*start)->blank_y == 0 || (*start)->move_list->move == 1) {
		return NULL;
	}

	temp = malloc(sizeof(bcl));

	temp->board = cpy_board((*start)->board);
	temp->moves = (*start)->moves + 1;
	temp->blank_y = (*start)->blank_y - 1;
	temp->blank_x = (*start)->blank_x;
	temp->next = NULL;
	temp->move_list = malloc(sizeof(ml));
	temp->move_list->move = 0;
	temp->move_list->next = (*start)->move_list;

	temp->board[temp->blank_y + 1][temp->blank_x] =
		temp->board[temp->blank_y][temp->blank_x];
	temp->board[temp->blank_y][temp->blank_x] = 0;

	temp->weight = get_board_weight(temp->board) + temp->moves;

	return temp;
}

bcl *move_down(bcl **start)
{
	bcl *temp;

	if ((*start)->blank_y == size - 1 || (*start)->move_list->move == 0) {
		return NULL;
	}

	temp = malloc(sizeof(bcl));

	temp->board = cpy_board((*start)->board);
	temp->moves = (*start)->moves + 1;
	temp->blank_y = (*start)->blank_y + 1;
	temp->blank_x = (*start)->blank_x;
	temp->next = NULL;
	temp->move_list = malloc(sizeof(ml));
	temp->move_list->move = 1;
	temp->move_list->next = (*start)->move_list;

	temp->board[temp->blank_y - 1][temp->blank_x] =
		temp->board[temp->blank_y][temp->blank_x];
	temp->board[temp->blank_y][temp->blank_x] = 0;

	temp->weight = get_board_weight(temp->board) + temp->moves;

	return temp;
}

bcl *move_left(bcl **start)
{
	bcl *temp;

	if ((*start)->blank_x == 0 || (*start)->move_list->move == 3) {
		return NULL;
	}

	temp = malloc(sizeof(bcl));

	temp->board = cpy_board((*start)->board);
	temp->moves = (*start)->moves + 1;
	temp->blank_y = (*start)->blank_y;
	temp->blank_x = (*start)->blank_x - 1;
	temp->next = NULL;
	temp->move_list = malloc(sizeof(ml));
	temp->move_list->move = 2;
	temp->move_list->next = (*start)->move_list;

	temp->board[temp->blank_y][temp->blank_x + 1] =
		temp->board[temp->blank_y][temp->blank_x];
	temp->board[temp->blank_y][temp->blank_x] = 0;

	temp->weight = get_board_weight(temp->board) + temp->moves;

	return temp;
}

bcl *move_right(bcl **start)
{
	bcl *temp;

	if ((*start)->blank_x == size - 1 || (*start)->move_list->move == 2) {
		return NULL;
	}

	temp = malloc(sizeof(bcl));

	temp->board = cpy_board((*start)->board);
	temp->moves = (*start)->moves + 1;
	temp->blank_y = (*start)->blank_y;
	temp->blank_x = (*start)->blank_x + 1;
	temp->next = NULL;
	temp->move_list = malloc(sizeof(ml));
	temp->move_list->move = 3;
	temp->move_list->next = (*start)->move_list;


	temp->board[temp->blank_y][temp->blank_x - 1] =
		temp->board[temp->blank_y][temp->blank_x];
	temp->board[temp->blank_y][temp->blank_x] = 0;

	temp->weight = get_board_weight(temp->board) + temp->moves;

	return temp;
}

char add_bcl(bcl **head, bcl **poss)
{
	bcl *temp;

	if (*poss == NULL) {
		return 0;
	}

	if ((*poss)->moves > *MAXMOVES) {
		free(*poss);
		return 0;
	}

	if (*head == NULL) {
		*head = *poss;
		return 0;
	}

	if ((*head)->weight >= (*poss)->weight) {
		(*poss)->next = *head;
		*head = *poss;
		return 1;
	}

	temp = *head;

	while (temp->next != NULL) {
		if (temp->next->weight > (*poss)->weight) {
			(*poss)->next = temp->next;
			temp->next = *poss;
			break;
		}
		temp = temp->next;
	}
	temp->next = *poss;

	return 1;
}

char get_board_size (FILE *pFile)
{
	char *startReadLine, *readLine;
	char read, size;
	short temp;
	int len;

	startReadLine = readLine = malloc(sizeof(char) * MAXLINE);

	temp = 0;
	read = fgetc(pFile);

	while (read != '\n') {
		readLine[temp++] = read;
		read = fgetc(pFile);
	}
	readLine[temp++] = '-';
	readLine[temp] = '1';

	size = 0;
	len = 0;

	while (sscanf(readLine, "%hd%n", &temp, &len) == 1) {
		readLine += len;

		if (temp == -1) {
			break;
		}
		size++;
	}

	free(startReadLine);

	return size;
}

void free_bcl(bcl **head)
{
	bcl *temp;

	while ((*head)->next != NULL) {
		temp = *head;
		*head = (*head)->next;
		free_board(temp->board);
		free(temp->move_list);
		free(temp);
	}

	free_board((*head)->board);
	free((*head)->move_list);
	free(*head);
}

void free_board(short **board)
{
	char i;

	for (i = 0; i < size; i++) {
		free(board[i]);
	}
	free(board);
}

void free_move_list(ml *move_list)
{
	ml *temp;

	if (move_list == NULL) {
		return;
	}

	while (move_list->next != NULL) {
		temp = move_list;
		move_list = move_list->next;
		free(temp);
	}
	free(move_list);
}

short get_board_weight (short **board)
{
	char i, j, y, x;
	short weight = 0;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				continue;
			}

			y = ((board[i][j] - 1) / size - i);
			if (y < 0) {
				y *= -1;
			}
			x = ((board[i][j] - 1) % size - j);
			if (x < 0) {
				x *= -1;
			}
			weight += y + x;
		}
	}

	return weight;
}

short pop_wight_list(wl **top, short value)
{
	wl *temp, *pop;
	short wight;

	if ((*top)->value == value) {
		wight = (*top)->wight;
		temp = *top;
		*top = (*top)->next;
		return wight;
	} else {
		(*top)->wight++;
	}

	temp = *top;

	while (temp->next->value != value) {
		temp->next->wight++;
		temp = temp->next;
	}

	wight = temp->next->wight;
	pop = temp->next;
	temp->next = temp->next->next;
	free(pop);

	return wight;
}

char check_state (short **board)
{
	char i, j;
	short current = 1;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (board[i][j] != current++) {
				if (i == size - 1 && j == size - 1
					&& board[i][j] == 0) {
					return 1;
				}

				return 0;
			}
		}
	}

	return 2;
}

short **cpy_board (short **board)
{
	short **temp;
	char i, j;

	temp = malloc(sizeof(short *) * size);
	for (i = 0; i < size; i++) {
		temp[i] = malloc(sizeof(short) * size);
		for (j = 0; j < size; j++) {
			temp[i][j] = board[i][j];
		}
	}

	return temp;
}

void result (short **board, ml *list, char *y, char *x, char boolean,
				FILE *pFile)
{
	if (list->move == -1) {
		return;
	}

	result (board, list->next, y, x, boolean, pFile);

	switch (list->move) {

	case 0:
		board[*y][*x] = board[*y - 1][*x];
		board[*y - 1][*x] = 0;
		*y = *y - 1;
		break;

	case 1:
		board[*y][*x] = board[*y + 1][*x];
		board[*y + 1][*x] = 0;
		*y = *y + 1;
		break;

	case 2:
		board[*y][*x] = board[*y][*x - 1];
		board[*y][*x - 1] = 0;
		*x = *x - 1;
		break;

	case 3:
		board[*y][*x] = board[*y][*x + 1];
		board[*y][*x + 1] = 0;
		*x = *x + 1;
		break;

	default:
		break;
	}

	if (boolean == 1) {
		print_board (board);
	} else {
		store_board (board, pFile);
	}
}

void store_board (short **board, FILE *pFile)
{
	char i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size - 1; j++) {
			fprintf(pFile, "%hd ", board[i][j]);
		}
		fprintf(pFile, "%hd\n", board[i][j]);
	}
	fprintf(pFile, "\n");
}

void print_board (short **board)
{
	char i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			printf("%hd ", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
