#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<memory.h>
#include<ctype.h>

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
int movepiece(Coord_t old,Coord_t new );
int playermove(char player);
Coord_t enter_piece(Coord_t *playmem);
Coord_t enter_target(void);
int actcol(int usery);
int actrow(int userx);
int color_rule(Coord_t sample,char player);
int piece_rule(Coord_t st,Coord_t end,piece_t stp,piece_t endp);
int check_piece(bool mrule,Coord_t stp,Coord_t endp);
int check_target(Coord_t check,char pcolor);

piece_t board[ROW][COLUMN];//board array


void init_board(void){
    
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COLUMN;j++){
            board[i][j].info = BLANK;
            board[i][j].color = 'N';
        }
    }
    //numbers on the board
    board[0][0].info = "１";
    board[0][1].info = "２";
    board[0][2].info = "３";
    board[0][3].info = "４";
    board[0][4].info = "５";
    board[0][5].info = "６";
    board[0][6].info = "７";
    board[0][7].info = "８";
    board[0][8].info = "９";
    for(int i = 0; i<COLUMN; i++){
        board[0][i].color = 'L';
    }

    board[9][9].info = "１";
    board[8][9].info = "２";
    board[7][9].info = "３";
    board[6][9].info = "４";
    board[5][9].info = "５";
    board[4][9].info = "６";
    board[3][9].info = "７";
    board[2][9].info = "８";
    board[1][9].info = "９";

     for(int i = 0; i<ROW; i++){
        board[i][9].color = 'L';
    }

    // red words on the board
    for(int i = 0;i<9;i++){
        board[3][i].info = R(步);
        board[3][i].color = 'R';   
    }
    board[2][1].info = R(飛);
    board[2][1].color = 'R';  
    board[2][7].info = R(角);
    board[2][7].color = 'R';
    board[1][0].info = board[1][8].info = R(香);
    board[1][0].color = board[1][8].color = 'R';
    board[1][1].info = board[1][7].info = R(桂);
    board[1][1].color = board[1][7].color = 'R';
    board[1][2].info = board[1][6].info = R(銀);
    board[1][2].color = board[1][6].color = 'R';
    board[1][3].info = board[1][5].info = R(金);
    board[1][3].color = board[1][5].color = 'R';
    board[1][4].info = R(王);
    board[1][4].color = 'R';

   

    // blue words on the board
    for(int i = 0;i<9;i++){
        board[7][i].info = B(步);  
        board[7][i].color = 'B';
    }
    board[8][7].info = B(飛);
    board[8][7].color = 'B';
    board[8][1].info = B(角);
    board[8][1].color = 'B';
    board[9][0].info = board[9][8].info = B(香);
    board[9][0].color = board[9][8].color = 'B';
    board[9][1].info = board[9][7].info = B(桂);
    board[9][1].color = board[9][7].color = 'B';
    board[9][2].info = board[9][6].info = B(銀);
    board[9][2].color = board[9][6].color = 'B';
    board[9][3].info = board[9][5].info = B(金);
    board[9][3].color = board[9][5].color = 'B';
    board[9][4].info = B(王);
    board[9][4].color = 'B';
}

Stack_t* init_stack(){
    Stack_t* stk = (Stack_t*)malloc(sizeof(Stack_t));
    stk -> top = NULL;
    return stk;
}


void push(Stack_t *const stk, piece_t src[ROW][COLUMN]){

    Boardlist_t *node = (Boardlist_t*)malloc(sizeof(Boardlist_t));
     for(int i = 0;i<ROW;i++)
    {
        for(int j = 0;j<COLUMN;j++){
            node -> boardmem[i][j] = src[i][j];
        }
    }
    node -> next = NULL;

    if(stk -> top){
        stk -> top -> next = node;
        node -> previous  = stk ->top;
    }
    stk -> top = node;

    return;
}

void pop(Stack_t *const stk, piece_t des[ROW][COLUMN], long size){
    if(is_empty(stk)){
        printf("Stack is empty!\n");
        return;
    }

    Boardlist_t *tmp = stk -> top;
    stk -> top = tmp -> previous;
    for(int i = 0;i<ROW;i++)
    {
        for(int j = 0;j<COLUMN;j++){
            memcpy(&des[i][j], &(tmp -> boardmem[i][j]), size);
        }
    }
    free(tmp);
    return;

}

bool is_empty(const Stack_t *const stk){
    if(stk -> top ==NULL){
        return true;
    }
    else{
        return false;
    }
}


void regret_or_save(Stack_t *playing_stack,int *hand,char player){
    char det;
    int error = 0;
    printf("\nDid you want to regret this hand? Or do you want to save the board right here?\n");
    printf("To regret type 0,to continue playing type p, to save type s\n");
    while(error == 0){
        scanf("%c",&det);
        getchar();
        if(det=='0'|| det=='p'||det=='s'){
            break;
        }
        else{
            printf("Wrong! try again\n");
        }
    }
    
    if(det =='0'){
        printf("\nEnter continuous regreting mode\n");
        printf("If yes type Y, if no type N\n");
        char loop = 'Y';
        while(loop == 'Y'){
            pop(playing_stack,board,sizeof(piece_t));
            *hand-=1;
            printf("\nThis is after hand no. %d\n",*hand);
            printboard();
            if(player == 'B'){
                printf("\nPlayer 'R' do you want to regret this hand of yours?\n");
                scanf("%c",&loop);
                getchar();
                player = 'R';
                continue;
            }
            if(player == 'R'){
                printf("\nPlayer 'B' do you want to regret this hand of yours?\n");
                scanf("%c",&loop);
                getchar();
                player = 'B';
            }    
        }
        push(playing_stack,board);
        printf("\nExit continuous regretting mode");
        printf("\nDo you like to continue playing or save the board?(Type c for continue or s for save)\n" );
        scanf("%c",&det);
        getchar();
        if(det == 'c'){
            printboard();
            return;
        }
    }
    if(det=='s'){
        push(playing_stack,board);
        *hand = -1; // hand = -1 indicates that board should be loaded
        return;
    }
    if(det =='p'){
        return;
    }
}

void read_old_file(char *filename){
    char *data;
    data = (char*) malloc(COLUMN);
    FILE *cfptr;
    int handub;
    int handnum = 0;
    char move;
    if((cfptr = fopen(filename,"r"))==NULL){
        printf("File can not be open.\n");
    }
    else{
        fscanf(cfptr,"%d",&handub);
        //print for hand No.0
        printf("\nThis is hand No.%d\n",handnum);
        for(int count =0;count<ROW;count++){
            fscanf(cfptr,"%s",data);
            printf("%s\n",data);
        }
        //move
        while(handnum<= handub){
            printf("Enter f for next hand b for previous hand, e to exit\n");
            scanf("%c",&move);
            getchar();
            if(move == 'f'){    
                handnum++;
                if(handnum>handub){
                    handnum--;
                    printf("There's no more hands that you had played,try exit or go previous hand\n");
                    continue;
                }
                printf("\nThis is hand No.%d\n",handnum);
                for(int count =0;count<ROW;count++){
                    fscanf(cfptr,"%s",data);
                    
                    printf("%s\n",data);
                }
            }
            else if(move == 'b'){
                rewind(cfptr);
                fscanf(cfptr,"%s",data);

                for(int count = 0;count<handnum-1;count++){
                    for(int count =0;count<10;count++){
                        fscanf(cfptr,"%s",data);
                    }
                }
                handnum--;
                if(handnum<0){
                    handnum++;
                    printf("There's no more previous hands you played,try exit or go next hand\n");
                    continue;
                }
                printf("\nThis is hand No.%d\n",handnum);
                for(int count =0;count<10;count++){
                    fscanf(cfptr,"%s",data);
                    printf("%s\n",data);
                }
                
            }
            else if(move == 'e'){
                free(data);
                fclose(cfptr);   
                printf("file closing...\n");
                break;
            }
            else{
                printf("Enter wrong, try again\n");
                continue;
                
            }
            
        }
        
    }
   
}



void load_to_file(Stack_t* srcstk,Stack_t* desstk,int hand,char *filename){
    piece_t tmpboard[ROW][COLUMN];
    //correct order
    while(!is_empty(srcstk)){
        pop(srcstk,tmpboard,sizeof(piece_t));
        push(desstk,tmpboard);
    }
    FILE * cfptr;
    if((cfptr = fopen(filename,"w"))==NULL){
        printf("File can not be open.\n");
    }
    else{
        int count = 0;
        fprintf(cfptr,"%d\n",hand);
        while(count<= hand){
            pop(desstk,tmpboard,sizeof(piece_t));
            for(int i = 0;i<10;i++){    
                for(int j = 0;j<10;j++){
                    fprintf(cfptr,"%s",tmpboard[i][j].info);
                }
                fprintf(cfptr,"\n");
            }
            count++;
        }
    }
        fclose(cfptr);
        printf("File load complete\n");
}

void printboard(){
    for(int i = 0;i<10;i++)
    {
        for(int j = 0;j<10;j++){
            printf("%s",board[i][j].info);
        }
        printf("\n");
    }
}
 
void play(char *filename){
    Stack_t *playing_stack = init_stack();
    Stack_t *loading_stack = init_stack();
    int hands = 1;
    int memhand = 0;
    int win = 0;
    push(playing_stack,board);
    while(win==0){
       
        if(hands %2 == 1){
            printf("\nThis is hand no. %d\n",hands);
            win = playermove('B');
            memhand = hands;
            regret_or_save(playing_stack,&hands,'B');
            if(hands== -1){
                //load here;
                load_to_file(playing_stack,loading_stack,memhand,filename);
                break;
            }
            hands +=1;
            push(playing_stack,board);
            if(win == 2){
                printf("Game Over! Player\033[34m X \033[mWin!\n");
                break;
            }
            
        }
        else if(hands%2 == 0){
            printf("\nThis is hand no. %d\n",hands);
            win = playermove('R');
            memhand = hands;
            regret_or_save(playing_stack,&hands,'R');
            if(hands== -1){
                //load here;
                load_to_file(playing_stack,loading_stack,memhand,filename);
                break;
            }
            hands+=1;
            push(playing_stack,board);
        }
        if(win == 1){
            printf("Game Over! Player\033[31m Y \033[mWin!\n");
        }

    }
   
}

int playermove(char player){
    Coord_t acstp;
    Coord_t acfnp;
    Coord_t plstp;
    int status = 0;
    int finish = 0;
    int presult = 0;
    
    if(player == 'B'){
        printf("Player\033[34m %c \033[mplay!\n",player);
    }
    else if (player == 'R'){
        printf("Player\033[31m %c \033[mplay!\n",player);
    }
    // enter piece coordinate
    acstp = enter_piece(&plstp);
    status = color_rule(acstp,player);
    while(status!=0){
        if(status==2){
            printf("The piece you chose is not your piece, try again\n");
        }
        else if(status==1){
            printf("There's no piece on this coordinate try again\n");
        }
        acstp = enter_piece(&plstp) ;
        status = color_rule(acstp,player) ;
    }
    // enter target coordinate
    acfnp = enter_target();
    presult = piece_rule(acstp,acfnp,board[acstp.x][acstp.y],board[acfnp.x][acfnp.y]);
    while(presult != 0){
        if(presult == 2){
            //system("clear");
            printboard();
            printf("This piece cannot move like this, try again\n");
            printf("The piece you chose is at (%d,%d) and it's a %s \n\n",plstp.x,plstp.y,board[acstp.x][acstp.y].info);
        }
        else if(presult == 1){
            //system("clear");
            printboard();
            printf("There's another piece of yours ont this coordinate, try again\n");
            printf("The piece you chose is at (%d,%d) and it's a %s \n\n",plstp.x,plstp.y,board[acstp.x][acstp.y].info);
        }
        acfnp = enter_target();
        presult = piece_rule(acstp,acfnp,board[acstp.x][acstp.y],board[acfnp.x][acfnp.y]);
    }
    finish = movepiece(acstp,acfnp);
    //system("clear");
    printboard();
    return finish;
}

Coord_t enter_piece(Coord_t *playmem){
    Coord_t piece;
    int tmp;
    printf("Please enter the coordinate of the piece that you want to move:\n");
    scanf("%d %d",&piece.x,&piece.y);
    getchar();
    playmem -> x = piece.x;
    playmem -> y = piece.y;
    tmp = piece.x;
    piece.x = actrow(piece.y); 
    piece.y = actcol(tmp);
    return piece;
}

Coord_t enter_target(void){
    Coord_t target;
    int tmp;
    printf("Please enter which coordinate you want this piece to move to \n");
    scanf("%d %d",&target.x,&target.y);
    getchar();
    tmp = target.x;
    target.x = actrow(target.y);
    target.y = actcol(tmp);
    return target;
}

int actcol(int userx){
    return userx-1;//cange user coordinate to actual 
}

int actrow(int usery){
    return 10-usery;
}

int color_rule(Coord_t sample,char player){
    if(board[sample.x][sample.y].color==player){
        return 0;
    }
    else if(board[sample.x][sample.y].color=='N'){
        return 1;
    }
    else{
        return 2;
    } 
}

int piece_rule(Coord_t st,Coord_t end,piece_t stp,piece_t endp){
    int get = 0;
    bool moverule;
    //Rule for B(步)
    if(strstr(board[st.x][st.y].info,B(步))!= NULL){
        moverule = (st.x-end.x == 1 && st.y-end.y == 0);
        get = check_piece(moverule,st,end);
    }
    //Rule for R(步)
    if(strstr(board[st.x][st.y].info,R(步))!= NULL){
        moverule = (st.x-end.x == -1 && st.y-end.y == 0);
        get = check_piece(moverule,st,end);
    }
    //Rule for B(香)
    if(strstr(board[st.x][st.y].info,B(香))!= NULL){
        int limit = 0;
        for(int i = st.x-1; board[i][st.y].color =='N'; i--){
            limit = i;
        }
        printf("\nlimit = %d\n",limit);//tmp
        moverule = (end.x<st.x && end.x>=limit-1 && st.y-end.y == 0);
        get = check_piece(moverule,st,end);
    }
    //Rule for R(香)
    if(strstr(board[st.x][st.y].info,R(香))!= NULL){
        int limit;
        for(int i = st.x+1; board[i][st.y].color =='N'; i++){
            limit = i;
        }
        moverule = (end.x>st.x && end.x<=limit+1 && st.y-end.y == 0);
        get = check_piece(moverule,st,end);
    }
    //Rule for R(香)
    if(strstr(board[st.x][st.y].info,R(香))!= NULL){
        int limit;
        for(int i = st.x+1; board[i][st.y].color =='N'; i++){
            limit = i;
        }
        moverule = (end.x>st.x && end.x<=limit+1 && st.y-end.y == 0);
        get = check_piece(moverule,st,end);
    }
    //Rule for B(桂)
    if(strstr(board[st.x][st.y].info,B(桂))!= NULL){
        moverule = ((st.x-end.x == 2 && st.y-end.y == 1) || (st.x-end.x == 2 && st.y-end.y == -1));
        get = check_piece(moverule,st,end);
    }
    //Rule for R(桂)
    if(strstr(board[st.x][st.y].info,R(桂))!= NULL){
        moverule = ((st.x-end.x == -2 && st.y-end.y == 1) || (st.x-end.x == -2 && st.y-end.y == -1));
        get = check_piece(moverule,st,end);
    }
    //Rule for B(銀)
    if(strstr(board[st.x][st.y].info,B(銀))!= NULL){
        moverule = ((st.x-end.x == 1 && (st.y-end.y == 0 || st.y-end.y == 1 || st.y-end.y == -1)) || (st.x-end.x == -1 && (st.y-end.y == 1 || st.y-end.y == -1)));
        get = check_piece(moverule,st,end);
    }
    //Rule for R(銀)
    if(strstr(board[st.x][st.y].info,R(銀))!= NULL){
        moverule = ((st.x-end.x == -1 && (st.y-end.y == 0 || st.y-end.y == 1 || st.y-end.y == -1)) || (st.x-end.x == 1 && (st.y-end.y == 1 || st.y-end.y == -1)));
        get = check_piece(moverule,st,end);
    }
    //Rule for B(金)
    if(strstr(board[st.x][st.y].info,B(金))!= NULL){
        moverule = ((st.x-end.x == 1 && (st.y-end.y == 0 || st.y-end.y == 1 || st.y-end.y == -1)) || (st.x-end.x == -1 && st.y-end.y == 0) || (st.x-end.x == 0 && (st.y-end.y == 1 || st.y-end.y == -1)));
        get = check_piece(moverule,st,end);
    }
    //Rule for R(金)
    if(strstr(board[st.x][st.y].info,R(金))!= NULL){
        moverule = ((st.x-end.x == -1 && (st.y-end.y == 0 || st.y-end.y == 1 || st.y-end.y == -1)) || (st.x-end.x == 1 && st.y-end.y == 0) || (st.x-end.x == 0 && (st.y-end.y == 1 || st.y-end.y == -1)));
        get = check_piece(moverule,st,end);
    }
    //Rule for B(王)
    if(strstr(board[st.x][st.y].info,B(王))!= NULL){
        moverule = ((st.x-end.x == 1 && (st.y-end.y == 0 || st.y-end.y == 1 || st.y-end.y == -1)) || (st.x-end.x == -1 && (st.y-end.y == 0 || st.y-end.y == 1 || st.y-end.y == -1)) || (st.x-end.x == 0 && (st.y-end.y == 1 || st.y-end.y == -1)));
        get = check_piece(moverule,st,end);
    }
    //Rule for R(王)
    if(strstr(board[st.x][st.y].info,R(金))!= NULL){
        moverule = ((st.x-end.x == -1 && (st.y-end.y == 0 || st.y-end.y == 1 || st.y-end.y == -1)) || (st.x-end.x == 1 && (st.y-end.y == 0 || st.y-end.y == 1 || st.y-end.y == -1)) || (st.x-end.x == 0 && (st.y-end.y == 1 || st.y-end.y == -1)));
        get = check_piece(moverule,st,end);
    }
    //Rule for B(角) and R(角)
    if((strstr(board[st.x][st.y].info,B(角))!= NULL) || (strstr(board[st.x][st.y].info,R(角))!= NULL)){
        int ul=st.x-1 ,ur=st.x-1 ,dl=st.x+1 ,dr=st.x+1;
        // upper and lower limit to right
        if(end.y-st.y > 0){
            for(int i = st.y+1; board[ur][i].color =='N'; i++){
                ur-=1;
            }
            for(int i = st.y+1; board[dr][i].color =='N'; i++){
                dr+=1;
            }
            moverule = ((abs(st.x-end.x)==abs(st.y-end.y)) && (end.x>=ur && end.x<=dr) && (st.x-end.x!=0));
        }
        //upper and lower limit to left
        if(end.y-st.y < 0){
            for(int i = st.y-1; board[ul][i].color =='N'; i--){
                ul-=1;
            }
            for(int i = st.y-1; board[dr][i].color =='N'; i--){
                dl+=1;
            }
            moverule = ((abs(st.x-end.x)==abs(st.y-end.y)) && (end.x>=ul && end.x<=dl) && (st.x-end.x!=0));
        }
        get = check_piece(moverule,st,end);
    }    
    //Rule for B(飛) and R(飛)
    if((strstr(board[st.x][st.y].info,B(飛))!= NULL) || (strstr(board[st.x][st.y].info,R(飛))!= NULL)){
        int up = st.x-1, dw = st.x+1, lf = st.y-1, rg = st.y+1;
        //up and down limits 
        if(end.y-st.y == 0){
            for(int i = st.x-1; board[i][st.y].color =='N'; i--){
                up-=1;
            }
            for(int i = st.x+1; board[i][st.y].color =='N'; i++){
                dw+=1;
            }
            moverule = (end.x>=up && end.x<=dw);
        }else if(end.x-st.x == 0){
            for(int i = st.y-1; board[st.x][i].color =='N'; i--){
                lf-=1;
            }
            for(int i = st.y+1; board[st.x][i].color =='N'; i++){
                rg+=1;
            }
            moverule = (end.y>=lf && end.y<=rg);
        }
        else{
            moverule = ((end.y-st.y == 0) || (end.x-st.x == 0)); 
        }

        get = check_piece(moverule,st,end);
    }
    return get;
}

int check_piece(bool mrule,Coord_t stp,Coord_t endp){
    int check = 0;
    int result = 0;
    if(mrule == 1){
        check = check_target(endp,board[stp.x][stp.y].color);
        if(check == 1){
            result = 1;
        }
        else{
            result = 0; 
        }
    }
    else{
        result = 2;    
    }    
    return result;
}

int check_target(Coord_t check,char pcolor){
    if(board[check.x][check.y].color == pcolor){
        return 1;
    }
    else{
        return 0;
    }
}
int movepiece(Coord_t old,Coord_t new ){
    int det = 0;
    if((strstr(board[new.x][new.y].info,R(王))!=NULL)){
        det = 1;   
    }
    else if(strstr(board[new.x][new.y].info,B(王))!=NULL){
        det = 2;
    }
    // the cage of piece info and color
    board[new.x][new.y].info = board[old.x][old.y].info;
    board[old.x][old.y].info = BLANK;
    board[new.x][new.y].color = board[old.x][old.y].color;
    board[old.x][old.y].color = 'N';

    return det;
}




int main(int argc, char *argv[]){
    
    
    char *filename;
    for(int i = 0;i<argc;i++){
        
        if(i==1){
            char *arg = argv[i];
            char *arg2 = argv[i+1];
            switch(arg[1]){
                case'1':
                    filename = argv[i+1];
                    read_old_file(filename);
                    printf("finish reading...");
                    break;
                case'n':
                    switch(arg2[1]){
                        case's':
                            filename = argv[i+2];
                            init_board();
                            printf("\n\nStart!\n\n");
                            printboard();
                            play(filename);
                            printboard();
                            break;
                    }
                break;
        
            }
        }

    }
    

    return 0;
}




