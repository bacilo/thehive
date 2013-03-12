#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TEAM_BLACK 0
#define TEAM_EMPTY 1
#define TEAM_WHITE 2
#define TEAM_POS 3
#define TEAM_AVAILABLE 4

#define BLANK_PIECE '-'

#define POS(board, x, y) (board[y][x])

#define ADJX1(i,j) (i%2?j:j-1)
#define ADJX6(i,j) (i%2?j+1:j)
#define ADJX2(i,j) (i%2?j:j-1)
#define ADJX5(i,j) (i%2?j+1:j)
#define ADJX3(i,j) j-1
#define ADJX4(i,j) j+1

#define ADJY1(i) i-1
#define ADJY6(i) i-1
#define ADJY2(i) i+1
#define ADJY5(i) i+1
#define ADJY3(i) i
#define ADJY4(i) i

#define FONT_BLACK printf("\033[30m")
#define FONT_RED   printf("\033[31m")
#define FONT_GREEN printf("\033[32m")
#define FONT_WHITE printf("\033[33m")

#define STARTING_PIECES "QBBSSGGGAAA"

#define BOARD_SIZE 10

typedef struct _piece PIECE;
typedef PIECE * PIECES;
typedef PIECES * BOARD;

struct _piece
{
	short team;
	char type;
};

typedef struct _position POSITION;
struct _position
{
	short x;
	short y;
};

void printRawBoard(BOARD board);

void initBoard(BOARD *aux){
	*aux = (BOARD) malloc(sizeof(PIECES) * BOARD_SIZE);
	for(int i=0; i < BOARD_SIZE; i++){
		(*aux)[i] = (PIECES) malloc(sizeof(PIECE) * BOARD_SIZE);
		for(int j=0; j < BOARD_SIZE; j++){
			(*aux)[i][j].type='.';
			(*aux)[i][j].team=TEAM_EMPTY;
		}
	}
}
void copyBoard(BOARD *dest, BOARD src){
	*dest = (BOARD) malloc(sizeof(PIECES) * BOARD_SIZE);
	for(int i=0; i < BOARD_SIZE; i++){
		(*dest)[i] = (PIECES) malloc(sizeof(PIECE) * BOARD_SIZE);
		for(int j=0; j < BOARD_SIZE; j++){
			(*dest)[i][j].type=src[i][j].type;
			(*dest)[i][j].team=src[i][j].team;
		}
	}
}

void setPosition(PIECE *pos, short team, char type){
	pos->type=type;
	pos->team=team;
}

bool breaksConnection(){
	return false;
}

bool isReachable(){
	return true;
}

bool hasOppositionNext(BOARD aux, short team, short i, short j){
	short oppTeam = (team==TEAM_WHITE?TEAM_BLACK:TEAM_WHITE);
	if((aux)[ADJY1(i)][ADJX1(i, j)].team==oppTeam)
		return true;
	if((aux)[ADJY2(i)][ADJX2(i, j)].team==oppTeam)
		return true;
	if((aux)[ADJY3(i)][ADJX3(i, j)].team==oppTeam)
		return true;
	if((aux)[ADJY4(i)][ADJX4(i, j)].team==oppTeam)
		return true;
	if((aux)[ADJY5(i)][ADJX5(i, j)].team==oppTeam)
		return true;
	if((aux)[ADJY6(i)][ADJX6(i, j)].team==oppTeam)
		return true;
	return false;
}

void isPossibleInitialPosition(BOARD *aux, short team, short i, short j, int *possNmbr, POSITION *temp){
	if(!hasOppositionNext(*aux, team, i, j) && (*aux)[i][j].team==TEAM_EMPTY){
		setPosition(&(*aux)[i][j], TEAM_POS, '0' + (*possNmbr));
		temp[(*possNmbr)].x=i;
		temp[(*possNmbr)].y=j;
		(*possNmbr)++;
	}
}

POSITION* calcInitialPositions(BOARD *aux, short team){
	int possNmbr = 0;
	POSITION *temp = (POSITION*) malloc(sizeof(POSITION) * 20);
	for(int i=0; i < BOARD_SIZE; i++){
		for(int j=0; j < BOARD_SIZE; j++){
			if((*aux)[i][j].team==team){
				printf("found piece at: %d %d\n", i, j);
				isPossibleInitialPosition(aux, team, ADJY1(i), ADJX1(i, j), &possNmbr, temp);
				isPossibleInitialPosition(aux, team, ADJY2(i), ADJX2(i, j), &possNmbr, temp);
				isPossibleInitialPosition(aux, team, ADJY3(i), ADJX3(i, j), &possNmbr, temp);
				isPossibleInitialPosition(aux, team, ADJY4(i), ADJX4(i, j), &possNmbr, temp);
				isPossibleInitialPosition(aux, team, ADJY5(i), ADJX5(i, j), &possNmbr, temp);
				isPossibleInitialPosition(aux, team, ADJY6(i), ADJX6(i, j), &possNmbr, temp);
			}
		}
	}
	for(int i=0; i<possNmbr; i++)
		printf("adjacent %d: x = %d, y = %d\n",i , temp[i].x, temp[i].y);
	return temp;
}

void isMoveable(BOARD *aux, short team, short i, short j, int *possNmbr, POSITION *temp){
	if(!breaksConnection()){
		setPosition(&(*aux)[i][j], TEAM_AVAILABLE, '0' + (*possNmbr));
		temp[(*possNmbr)].x=i;
		temp[(*possNmbr)].y=j;
		(*possNmbr)++;
	}
}

POSITION* calcMoveablePieces(BOARD *aux, short team){
	int possNmbr = 0;
	POSITION *temp = (POSITION*) malloc(sizeof(POSITION) * 20);
	for(int i=0; i < BOARD_SIZE; i++){
		for(int j=0; j < BOARD_SIZE; j++){
			if((*aux)[i][j].team==team){
				printf("found piece at: %d %d\n", i, j);
				isMoveable(aux, team, i, j, &possNmbr, temp);
			}
		}
	}
	for(int i=0; i<possNmbr; i++)
		printf("adjacent %d: x = %d, y = %d\n",i , temp[i].x, temp[i].y);
	return temp;
}

PIECE * newPiece(PIECE * pce, short team, char type){
	// pce = (PIECE *) malloc(sizeof(PIECE));
	pce->team = team;
	pce->type = type;
	return pce;
}

void printRawBoard(BOARD board){
	printf("\t");
	for(int i=0; i < BOARD_SIZE; i++)
		printf("%d\t", i);
	printf("\n");
	for(int i=0; i < BOARD_SIZE; i++){
		printf("%d\t", i);
		for(int j=0; j < BOARD_SIZE; j++){
	 			printf("%c", POS(board, j, i).type);
	 		printf("\t");
		}
	 	printf("\n");
	}
}

bool rmPiece(char* plr, char type){
	char *occ = strchr(plr, type);
	if(occ){
		printf("removing piece %c, found in position %d\n", type, (occ-plr));
		*occ = plr[strlen(plr)-1];
		plr[strlen(plr)-1]='\0';
		return true;
	}
	printf("piece %c not available\n", type);
	return false;
}

void setColor(short team){
	switch(team){
		case TEAM_WHITE:
		FONT_GREEN;
		break;
		case TEAM_BLACK:
		FONT_RED;
		break;
		case TEAM_POS:
		FONT_WHITE;
		break;
		case TEAM_AVAILABLE:
		FONT_BLACK;
		break;
	}
}

void printTopPiece(short team){
	setColor(team);
	printf("/ \\");
	printf("\E[0m");
}

void printMiddlePiece(short team, char type){
	setColor(team);
	printf("|%c|", type);
	printf("\E[0m");
}

void printBottomPiece(short team){
	setColor(team);
	printf("\\ /");
	printf("\E[0m");
}

void printBlank(){
	printf("%c%c%c", BLANK_PIECE, BLANK_PIECE, BLANK_PIECE);
}

void printNiceBoard(BOARD board){
	for(int i=0; i<BOARD_SIZE; i++){
		for(int line=0; line<3; line++){
			if(i%2)
				printf("%c%c", BLANK_PIECE, BLANK_PIECE);
			for(int j=0; j<BOARD_SIZE; j++){
				printf("%c", BLANK_PIECE);
				if(POS(board, j, i).team == TEAM_EMPTY)
					printBlank();
				else{
					switch(line){
						case 0:
					// printf("found piece and going for line %d\n", line);
						printTopPiece(POS(board, j, i).team);
						break;
						case 1:
						printMiddlePiece(POS(board, j, i).team, POS(board, j, i).type);
						break;
						case 2:
						printBottomPiece(POS(board, j, i).team);
						break;
					}
				}

			}
			if(!(i%2))
				printf("%c%c", BLANK_PIECE, BLANK_PIECE);
			printf("\n");
		}
	}
}

void printHelp(){
	printf("Welcome to The Fucking Hive!\n");
	printf("h - help\n");
	printf("s - start a new game\n");
	printf("q - quit\n");
	printf("    -----    \n");
	printf("i X Y - insert piece\n");
	printf("c - clear board\n");
}

bool isGame = false;
bool isPlayer1 = false;

int main(){
	BOARD board, possBoard;

	char *p1 = (char*) malloc(sizeof(char) * strlen(STARTING_PIECES));
	char *p2 = (char*) malloc(sizeof(char) * strlen(STARTING_PIECES));

	p1 = strcpy(p1, STARTING_PIECES);
	p2 = strcpy(p2, STARTING_PIECES);

	initBoard(&board);

	char command[10];
	do{
		if(isGame){
			printNiceBoard(board);
			printf("It's your turn %s. What you want to do?\n", (isPlayer1?"player 1":"player 2"));
			printf("Pieces you can play: %s\n", (isPlayer1?p1:p2));
		}

		printf("What you want to do?\n");
		scanf("%s", command);
		switch(command[0]){
			case 's':
			isGame=true;
			isPlayer1=true;
			break;
			case 'h':
			printHelp();
			break;
			case 'm':
			POSITION *temp;
			int choice;
			copyBoard(&possBoard, board);
			temp = calcMoveablePieces(&possBoard, TEAM_WHITE);
			printNiceBoard(possBoard);
			printf("Which piece would you like to move?\n");
			scanf("%d", &choice);
			printf("%d\n", choice);
			break;
			default:
			if(rmPiece((isPlayer1?p1:p2), command[0])){
				if(strlen(p1) == strlen(STARTING_PIECES)-1){
					if(isPlayer1)
						newPiece(&POS(board, BOARD_SIZE/2, BOARD_SIZE/2), (isPlayer1?TEAM_WHITE:TEAM_BLACK), command[0]);
					else{
						POSITION *temp;
						int choice;
						copyBoard(&possBoard, board);
						temp = calcInitialPositions(&possBoard, TEAM_WHITE);
						printNiceBoard(possBoard);
						printf("Where would you like to place it?\n");
						scanf("%d", &choice);
						printf("%d\n", choice);
						newPiece(&POS(board, temp[choice].y, temp[choice].x), TEAM_BLACK, command[0]);
					}
				}
				else{
					POSITION *temp;
					int choice;
					copyBoard(&possBoard, board);
					temp = calcInitialPositions(&possBoard, (isPlayer1?TEAM_WHITE:TEAM_BLACK));
					printRawBoard(possBoard);
					printNiceBoard(possBoard);
					printf("Where would you like to place it?\n");
					scanf("%d", &choice);
					printf("%d\n", choice);
					newPiece(&POS(board, temp[choice].y, temp[choice].x), (isPlayer1?TEAM_WHITE:TEAM_BLACK), command[0]);
				}
				isPlayer1=!isPlayer1;
			}
			break;
		}
	} while(command[0]!='q');

	// newPiece(&POS(board, 5, 6), TEAM_WHITE, 'Q');
	// newPiece(&POS(board, 5, 7), TEAM_POS, 'G');
	// newPiece(&POS(board, 5, 8), TEAM_BLACK, 'G');
	// newPiece(&POS(board, 6, 8), TEAM_BLACK, 'G');
	// // newPiece(&POS(board, 4, 8), true, 'R');
	// newPiece(&POS(board, 4, 7), TEAM_WHITE, 'A');
	// printRawBoard(board);
	// printNiceBoard(board);
	return 0;
}