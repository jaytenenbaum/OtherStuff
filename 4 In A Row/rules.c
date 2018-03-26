/*
	isLegal(element elem);
*/
#include "header.h"


/*returns whether the board move on b in row,col with the next player is legal*/
int isLegalBoardMove(board_t b, int row, int col, int nextPlayer){
	vertex v = (vertex)malloc(sizeof(struct vertex_s));
	v-> game_state = b;
	v-> nextPlayer = nextPlayer;
	int res = isLegal(v,row,col);
	/*free the used memory*/
	free(v);
	return res;
}

/*returns the new board after the move on board b in row,col with next player*/
board_t getNewBoard(board_t b,int row,int col, int nextPlayer){
	/*create vertex then the element containing it*/
	vertex v = (vertex)malloc(sizeof(struct vertex_s));
	v-> game_state = b;
	v-> nextPlayer = nextPlayer;
	element e = getElementWithMove(v,row,col);
	board_t ans = e->node->game_state;
	free(v);
	free(e);
	return ans;


}

/*returns if puting the next move in [row,col] is legal, with information from the current vertex. (next player is given in vertex)*/
int isLegal(vertex  v, int row, int col){
	switch(game){
		case 1:
			return TTT_isLegal(v, row, col);
		case 2:
			return connect4_isLegal(v, row, col);
		case 3:
			return reversi_isLegal(v,row, col);
		default:
			printf("ERROR: no game has been chosen!!!");
			return -1;
	}	
}

/*returns whether the move is legal for TTT. This is simply if the given coordinates are free*/
int TTT_isLegal(vertex  v, int row, int col){
	return (v->game_state)[row][col]==0;
	
}
/*returns whether the move is legal for CON4.  This is simply if the top piece in the given row is free*/
int connect4_isLegal(vertex  v, int row, int col){
	return (v->game_state)[0][col]==0;
}
/*returns whether the move is legal for reversi.  This is simply if the potential board after making the move is different from given board
in coordinates that aren't the inserted coordinates*/
int reversi_isLegal(vertex  v, int row, int col){

	/*check if the square is already full*/
	if((v->game_state)[row][col]!=0){
		return 0;
	}
	/*if it isn't, check if adding it would change the board*/
	board_t b = getReversiBoardAfterMove(v->game_state,row,col, v->nextPlayer);
	int i,j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			/*the new piece added is always changed, so no "chochma"*/
			if((i==row)&&(j==col)){
				continue;
			}
			/*if we changed at least one piece, dayeynu*/
			if((v->game_state)[i][j] != b[i][j]){
				freeBoard(b,8);
				return 1;
			}
		}
	}
	freeBoard(b,8);
	return 0;

}

/*free the board using the game global variable, to have less switches in other functions.*/
void freeBoardWithGlobalGame(board_t board){
	switch(game){
		case 1:
			freeBoard(board,3);
			break;
		case 2:
			freeBoard(board,6);
			break;		
		case 3:
			freeBoard(board,8);
			break;	
		default:
			printf("Error: no selection has been made about the game.\n");
	}
}
/* free memory allocated for board. it first frees each row, and finally frees the board.*/
void freeBoard(board_t board, int rows) {

	for (int i = 0; i < rows; i++) {
		free(board[i]);
	}
	free(board);
}

/*returns whether the game is over or not. This is iff board is full, or someone has won*/
int isGameOver(board_t board){
	int sc = score(board);
	if(isFull(board)){
		return 1;
	}
	return ((sc==WIN) ||(sc==LOSE)); 
}

/*returns whether the given board is full or not*/
int isFull(board_t board){
	int rows,cols,i,j;
	/*get board sizes*/
	switch(game){
		case 1:
			rows = 3;
			cols = 3;
			break;
		case 2:
			rows = 6;
			cols = 7;
			break;	
		case 3:
			rows = 8;
			cols = 8;
	}
	/*check if at least one coordinate is free (zero)*/
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			if(board[i][j]==0){
			 return 0;
			}
		}
	}
	return 1;
}

/*returns the winning stripe based on the givin board, using the game*/
control getWinningStripe(board_t board){
	assert(game>0 && game<3);
	printf("entering getWinningStripe function with game %d\n",game);
	/*this is the resulted control we later return*/
	control res= createControl(window, getRect(0,0,600,600) , NULL, NULL, "winner stripe");
	assert(res!=NULL);
	/*the pointer to the winning line*/
	int* winningLine = getWinningLine(board);
	printf("got the winning line\n");
	/*set size of the line, based on game. Nore game is either 1 or 2, because reversi has no winning line*/
	int size = (game == 1) ? 3 : 4;
	int mod= (game == 1)? 3:7;
	int rectHeigth = (game == 1)? Height/3:Height/6;
	int rectWidth = (game == 1)? ((3*Width)/4)/3:((3*Width)/4)/7;
	printf("getting direction\n");
	/*represents the direction of the line, 0 is horizontal, 1 is vertical, 2 is diagonal "\" and 3 is diagonal "/"*/
	int dir;
	if (winningLine[0]/mod == winningLine[1]/mod){
		dir = 0;
	}
	else if (winningLine[0]%mod == winningLine[1]%mod){
		dir = 1;
	}
	else if (winningLine[0]%mod> winningLine[1]%mod){
		dir = 2;
	}
	else{
		dir = 3;
	}
	printf("direction is %d\n",dir );
	/*The address of the line that we draw*/
	control c;
	char add[100];
	sprintf(add, "Textures/Lines/%d/%d.bmp", game, dir);
	printf("add\n");
	int i;
	int row,col;
	/*go over the different coordinates of the drawing line, and add them as children to the returned control*/
	for (i = 0; i < size; i++){
		row=winningLine[i]/mod;
		col=winningLine[i]%mod;
		printf("row %d and col %d\n",row,col );
		c=createControl(SDL_LoadBMP(add), getRect(col*rectWidth,row*rectHeigth,rectWidth,rectHeigth), NULL, NULL, "winner stripe");
		if (SDL_SetColorKey((c->surface), SDL_SRCCOLORKEY, SDL_MapRGB((c->surface)->format, 255, 255, 255)) != 0) {
			printf("ERROR: failed to set color key: %s\n", SDL_GetError());
			SDL_FreeSurface(c->surface);
			return NULL ;
		}
		addControlChild(res,c);
		
	}
	return res;
}


/*returns the winner of reversi*/
int getWinnerReversi(board_t board){
	int ones,minusOnes,i,j;

	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			assert(board[i][j]!=0);
			if(board[i][j]==-1){
				minusOnes++;
			}
			else{
				ones++;
			}
		}
	}
	/*more ones than minus ones means that winner is 1*/
	if(ones>minusOnes){
		return 1;
	}
	/*more minus ones than ones means that winner is -1*/
	else if(ones<minusOnes){
		return -1;
	}
	/*equal amount means tie*/
	else{
		return 0;
	}

}


/*Returns 1 if there is a move that the next player can do and 0 otherwise.*/
int canMakeAMoveReversi(board_t board,int nextPlayer){
	vertex v = (vertex)malloc(sizeof(struct vertex_s));
	v->nextPlayer=nextPlayer;
	v->game_state=copy(board,8,8);
	/*get possible moves from given state (not that it returns NULL if no such moves exist)*/
	linked_list moves=getPossibleMovesREV(v);
	freeBoard(v->game_state,8);
	free(v);
	/*return whether there are possible moves from the given state.*/
	return moves!=NULL;
}