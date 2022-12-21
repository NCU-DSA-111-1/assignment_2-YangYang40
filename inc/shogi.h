#include <stdio.h>
#include<stdlib.h>

#define ROW 10
#define COLUMN 10
#define BLANK "　"
#define R(piece) "\033[31m"#piece"\033[m"
#define B(piece) "\033[34m"#piece"\033[m"

typedef struct piece{
    char *info;
    char color;
}piece_t;

//Linked list to save board
typedef struct boardlist{
    piece_t boardmem[ROW][COLUMN];
    struct boardlist *previous;
    struct boardlist *next;
}Boardlist_t;

typedef struct coord{
    int x;
    int y;
}Coord_t;

typedef struct Stack{
    Boardlist_t* top;
}Stack_t;

void init_board(void);
Stack_t* init_stack(void);
void push(Stack_t *const stk, piece_t src[ROW][COLUMN]);
void pop(Stack_t *const stk, piece_t des[ROW][COLUMN], long size);
bool is_empty(const Stack_t *stk);
void printboard(void);
void play(char *filename);
void read_old_file(char *filename);
void load_to_file(Stack_t* srcstk, Stack_t* desstk,int hand,char *filename);
void regret_or_save(Stack_t *playing_stack,int *hand,char player);
int movepiece(Coord_t prev,Coord_t post );
int playermove(char player);
Coord_t enter_piece(Coord_t *playmem);
Coord_t enter_target(void);
int actcol(int usery);
int actrow(int userx);
int color_rule(Coord_t sample,char player);
int piece_rule(Coord_t st,Coord_t end,piece_t stp,piece_t endp);
int check_piece(bool mrule,Coord_t stp,Coord_t endp);
int check_target(Coord_t check,char pcolor);