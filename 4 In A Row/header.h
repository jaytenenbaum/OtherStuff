#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdarg.h>
#include <SDL.h>
#include <SDL_video.h>
#include <assert.h>

#define alphaINIT -100001
#define betaINIT 100001
#define WIN 100000
#define LOSE -100000

#define Height 600
#define Width 800
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define PVP 0
#define PVC 1
#define CVP 2
#define CVC 3
extern int paused;
extern int game;// 1= tic tac toe, 2= connect4, 3= reversi
/*NOTE THAT IN EVERY NEW SCREEN WE CREATE A NEW CONTROL OUT OF NODE, AND THEREFORE*/
extern SDL_Surface* window;
/*difficulty from probably 1 to 7*/
extern int difficultyP1;
extern int difficultyP2;
extern int mode;
extern int gameOver;

struct element_s {
	struct vertex_s *node;

	struct element_s *next;
	struct element_s *prev;
};

typedef struct element_s * element;

typedef int** board_t;

struct linked_list_s {
	element head;
	element tail;
};

typedef struct linked_list_s * linked_list;

struct vertex_s {
	int mov_col;
	int mov_row;
	board_t game_state;
	int score;
	int nextPlayer;

	linked_list edges;
};

typedef struct vertex_s * vertex;

struct loadGame_s{
	board_t game_state;
	int nextPlayer;
	int playedGame;
};
typedef struct loadGame_s* loadGameStruct;



struct control_s{
	 struct controlLL_s* children;
	 SDL_Surface *surface;
	 SDL_Rect rectangle;
	 /*the buttons above and below the current button. used for keyboard movement in menus*/
	 struct control_s* buttonAbove;
	 struct control_s* buttonBelow;
	 char* name;

};
typedef struct control_s* control;

struct controlLL_s{
	control cont;
	struct controlLL_s* next;
};
typedef struct controlLL_s * controlLL;


void showInstructions();
void showClosingScreen();
void preGame();
int isFull(board_t board);
void setDifficultyByMode();
void setDifficulty(int player);
control createControl(SDL_Surface *sfc, SDL_Rect rect, control bAbove, control bBelow, char* cname);
void addControlChild(control father, control newChild);
void createAndLinkChild(control father, SDL_Surface *sfc, SDL_Rect rect, control bAbove, control bBelow);
int isClicked(control ctrl, SDL_Event event);
int inRect( int x, int y ,SDL_Rect rect);
controlLL getControlLL();
void drawScreen(control root, int flip);
void drawControl(control ctrl);
SDL_Surface *getImageSurface(char* dir);
SDL_Rect getRect(int x, int y, int xOff, int yOff);
void freeUITree(control root);
void loadGameMenu(int save,board_t board, int nextPlayer);
void chooseGame();
void mainMenu();
void chooseMode();
void clearScreen();
void drawColor(control c ,int i);
void manageButtonColors(control oldButton, int i);

control getButtonPanel();
control getGameArea(board_t board);
void printAsciiBoard(board_t board);

/*########################## NEW FUNCTIONS ##########################*/
SDL_Surface *getPieceImg(int piece);
void updateBoardImage(control c, int piece);
void updateBoardGame(control gameArea, board_t board);
void drawBoard(control gameArea, board_t board);



/*GAMES FILE FUNCTIONS*/
void initiateGame();
board_t getBoard(int rows, int cols);
void Game(board_t board, int nextPlayer);
board_t pcMove(int currentPlayer,board_t board);
board_t userMove(board_t board, int row,int col, int mainPlayer);
char *getButtonClickedName(control gameArea, SDL_Event e);
int manageRightPanelEvent( control root, control rightButtonPanel,SDL_Event e, board_t board, int nextPlayer);
void gameOverPanel(int nextPlayer,board_t board);

/*DATA FILE FUNCTIONS*/
void saveGameData(board_t board, int nextPlayer,int slot);
loadGameStruct loadGameData(int slot);
void printBoard(FILE *f,board_t board);
board_t get_blank_board(int rows, int cols);
int file_exists(char *filename);
int slotExists(int slot);

/*abPruning FILE FUNCTIONS*/
linked_list getPossibleMovesTTT(vertex);
linked_list getPossibleMovesCON4(vertex);
linked_list getPossibleMovesREV(vertex);	
linked_list getLinkedList();
element getElementWithMove(vertex v,int i,int j);
int alphaBetaPruning(vertex curr,int a,int b,int player,int depth);
board_t copy(board_t board,int numRows, int numCols);
element TTT_getElementWithMove(vertex,int,int);
element connect4_getElementWithMove(vertex,int,int);
element reversi_getElementWithMove(vertex,int,int);
int getInsertRow(board_t b, int col);
board_t getReversiBoardAfterMove(board_t b,int row, int col, int player);
void freeABPruningTree(vertex root);

/*rules FILE FUNCTIONS*/
void freeBoard(board_t board, int rows);
int isLegal(vertex  v, int row, int col);
int TTT_isLegal(vertex  v, int row, int col);
int connect4_isLegal(vertex  v, int row, int col);
int reversi_isLegal(vertex  v, int row, int col);
int isLegalBoardMove(board_t b, int row, int col, int nextPlayer);
int isGameOver(board_t board);
board_t getNewBoard(board_t b,int row,int col, int nextPlayer);
void freeBoardWithGlobalGame(board_t board);
control getWinningStripe(board_t board);
int getWinnerReversi(board_t board);
int canMakeAMoveReversi(board_t board,int nextPlayer);

/*score FILE FUNCTIONS*/
int score(board_t board);
int TTT_score(board_t board);
int connect4_score(board_t board);
int reversi_score(board_t board);
int *connect4WinningLine(board_t board);
int *TTTWinningLine(board_t board);
int *getWinningLine(board_t board);