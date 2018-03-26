#include "header.h"

/*Return a linked list of all of the possible moves from a given vertex or NULL elsewhere*/
/*WE SOMEHOW NEED TO MANAGE THE CURRENT PLAYER EITHER BY CHANGING THE VERTEX 
	OR BY ADDING A GLOBAL VARIABLE THAT SAVES THE CURRNET PLAYER*/
linked_list getPossibleMoves(vertex v);

/*###############################################################################
################# When call at first it should be call like this : ##############
################# alphaBetaPruning(root,alphaINIT,betaINIT,player,depth) ########
###############################################################################*/
/*
alpha beta pruning function returning the score of the best next move based on the current player, given vertex of state of game, a,b, and remaining depth
*/
int alphaBetaPruning(vertex curr,int a,int b,int player,int depth){

	assert(depth>=0);
	assert(player==1||player==-1);
	/*get all the dext possible moves as vertex's edge*/
	curr->edges = getPossibleMoves(curr);
	if ( (depth == 0) || (curr->edges == NULL) ){
		curr->score = score(curr->game_state);
		return curr->score;
	}
	/*maximizing player*/
	if (player == 1){
		element iter = curr->edges->head;
		while(iter!=NULL){
			a = MAX(a,alphaBetaPruning(iter->node,a,b,-1,depth-1));
			/*the alpha beta cut*/
			if(b < a){
				break;
			}
			iter = iter->next;
		}
		curr->score=a;
		return a;
	}
	/*minimizing player*/
	else{
		element iter = curr->edges->head;
		while(iter != NULL){
			b = MIN(b,alphaBetaPruning(iter->node,a,b,1,depth-1));
			/*the alpha beta cut*/
			if(b < a){
				break;
			}
			iter = iter->next;
		}
		curr->score=b;
		return b;
	}

	
}
/*returns a linked list of all possible moves from the given game state by vertex*/
linked_list getPossibleMoves(vertex v){
	if(isGameOver(v->game_state)){
		return NULL;
	}
	switch(game){
		case 1:
			return getPossibleMovesTTT(v);
		case 2:
			return getPossibleMovesCON4(v);
		case 3:
			return getPossibleMovesREV(v);		
		default:
			printf("Error: Game has somehow not been decided yet.");
			return NULL;
	}
}


/*returns a linked list of all possible moves from the given game state by vertex, assuming the game is TTT*/
linked_list getPossibleMovesTTT(vertex v){
	linked_list ans = NULL;
	int j;
	int i;
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			if(isLegal(v,i,j)){
				/*if the linked list isn't initialized yet, init it*/
				if(ans == NULL){
					ans = getLinkedList();
					ans->head = getElementWithMove(v,i,j);
				}
				/*add another element with next move*/
				else{
					element e = getElementWithMove(v,i,j);
					e->next = ans->head;
					ans->head = e;
				}
			}
		}
	}
		return ans;

}

/*returns a linked list of all possible moves from the given game state by vertex, assuming the game is CON4*/
linked_list getPossibleMovesCON4(vertex v){
	linked_list ans = NULL;
	int i;
	for (i = 0; i < 7; ++i)
	{
		if(isLegal(v,0,i)){
				/*if the linked list isn't initialized yet, init it*/
				if(ans==NULL){
					ans = getLinkedList();
					ans->head = getElementWithMove(v,0,i);
				}
				/*add another element with next move*/
				else{
					element e = getElementWithMove(v,0,i);
					e->next = ans->head;
					ans->head = e;
				}
		}
	}
	return ans;
}

/*returns a linked list of all possible moves from the given game state by vertex, assuming the game is Reversi*/
linked_list getPossibleMovesREV(vertex v){
	linked_list ans = NULL;
	int j;
	int i;
	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{

			if(isLegal(v,i,j)){
				/*if the linked list isn't initialized yet, init it*/
				if(ans==NULL){
					ans = getLinkedList();
					ans->head = getElementWithMove(v,i,j);
				}
				/*add another element with next move*/
				else{
					element e = getElementWithMove(v,i,j);
					e->next=ans->head;
					ans->head = e;
				}
			}
		}
	}
	if((ans==NULL)&&(!isGameOver(v->game_state))){
		ans = getLinkedList();
		element node = (element)malloc(sizeof(struct element_s));
		node->node = v;
		node->next = NULL;
		node->prev = NULL;
		assert(node!=NULL);
		ans->head=node;
	}
	return ans;
}


/*copy the given board and allocate memory, with the given board, and number of columns and rows*/
board_t copy(board_t board,int numRows, int numCols) {
    /*allocate memory*/
    board_t copyBoard = getBoard(numRows, numCols);
    /*copy all board squares one by one*/
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            copyBoard[i][j] = board[i][j];
        }
    }
    return copyBoard;
}

/*get an empty allocated linked list*/
linked_list getLinkedList(){
	linked_list ans = (linked_list)malloc(sizeof(struct linked_list_s));
	assert(ans!=NULL);
	ans->head = NULL;

	return ans;

}

/*returns a new element after a move in row i and column j with player specified in v, and with board specified in v.
Note that the "next" of the element we create will be NULL*/
element getElementWithMove(vertex v,int i,int j){

	switch(game){
		case 1:
			return TTT_getElementWithMove(v,i,j);
		case 2:
			return connect4_getElementWithMove(v,i,j);
		case 3:
			return reversi_getElementWithMove(v,i,j);
		default:
			printf("ERROR: game hasn't been initialized!!!");
			return NULL;
	}
}

/*get the new element with move in i,j in TTT*/
element TTT_getElementWithMove(vertex v,int i,int j){
	/*allocate memory, and set relevant fields for the vertex, and then for an element containing it. pretty self explanatory*/

	vertex ans = (vertex)malloc(sizeof(struct vertex_s));
	assert(ans!=NULL);
	ans-> mov_col = j;
	ans-> mov_row = i;
	ans-> game_state = copy(v->game_state,3,3);
	ans-> game_state[i][j] = v->nextPlayer;
	ans-> nextPlayer = -(v->nextPlayer);
	ans->edges = NULL;

	element realAns = (element)malloc(sizeof(struct element_s));
	realAns->node = ans;
	realAns->next = NULL;
	realAns->prev = NULL;
	assert(realAns!=NULL);
	return realAns;


}
/*get the new element with move in i,j in CON4*/
element connect4_getElementWithMove(vertex v,int i,int j){
	/*allocate memory, and set relevant fields for the vertex, and then for an element containing it. pretty self explanatory*/
	vertex ans = (vertex)malloc(sizeof(struct vertex_s));
	assert(ans!=NULL);
	ans-> mov_col = j;
	ans-> mov_row = getInsertRow(v->game_state,j);
	ans-> game_state = copy(v->game_state,6,7);
	ans-> game_state[ans-> mov_row][j] = v->nextPlayer;
	ans-> nextPlayer = -(v->nextPlayer);
	ans->edges = NULL;


	element realAns = (element)malloc(sizeof(struct element_s));
	realAns->node = ans;
	realAns->next = NULL;
	realAns->prev = NULL;
	assert(realAns!=NULL);
	return realAns;

}

/*get the new element with move in i,j in reversi*/
element reversi_getElementWithMove(vertex v,int i,int j){
	/*allocate memory, and set relevant fields for the vertex, and then for an element containing it. pretty self explanatory*/
	vertex ans = (vertex)malloc(sizeof(struct vertex_s));
	assert(ans!=NULL);
	ans-> mov_col = j;
	ans-> mov_row = i;
	ans-> game_state = getReversiBoardAfterMove(v->game_state,i,j,v->nextPlayer);
	ans-> game_state[i][j] = v->nextPlayer;
	ans-> nextPlayer = -(v->nextPlayer);
	ans->edges = NULL;


	element realAns = (element)malloc(sizeof(struct element_s));
	realAns->node = ans;
	realAns->next = NULL;
	realAns->prev = NULL;
	assert(realAns!=NULL);
	return realAns;

}

/*given a column, in the game connect4, it returns the row that the disc should be inserted in given the column is col (essentially returns
the row index of the inserted piece)*/
int getInsertRow(board_t b, int col){
	int i;
	for(i=5; i>=0; i--){
		if(b[i][col] == 0){
			return i;
		}
	}
	/*we shouldn't be getting to this part of the code!!!*/
	assert(0);
	return -1;
}

/*returns a reversi board after making a move with given player in given coordinates, to a given board. (flipping al the needed pieces, and adding current piece)*/
board_t getReversiBoardAfterMove(board_t b,int row, int col, int player){
	board_t board= copy(b,8,8);

	int ind = 0;
	int j;


	/*right ray shooted from inserted piece, checking for flips to make.*/
	/*ind indicates whether we need to change the ray shooten from the inputted piece to other pieces.
	if we indeed hit a piece from our color while travelling the ray, after a sequence of other color pieces,
	we return with j=col+1 to beginning, and start coloring. if there is a space between added piece and the piece
	of same color on ray, we stop and don't color. if we hit the edge before finding same piece color, we stop
	*/
	ind = 0;
	for (j = col+1; j < 8; j++)
	{
		if(ind){
			if(board[row][j]==player||board[row][j]==0){
				break;
			}
			board[row][j]=player;
		}
		else{
			if(board[row][j]==player){
				ind =1;
				j=col;
			}
			else if(board[row][j] == 0){
				break;
			}
		}
	}

	/*left ray shooted from inserted piece, checking for flips to make.*/
	ind = 0;
	for (j = col-1; j >=0; j--)
	{
		if(ind){
			if(board[row][j]==player||board[row][j]==0){
				break;
			}
			board[row][j]=player;
		}
		else{
			if(board[row][j]==player){
				ind =1;
				j=col;
			}
			else if(board[row][j] == 0){
				break;
			}

		}
	}

	/*down ray shooted from inserted piece, checking for flips to make*/
	ind = 0;
	for (j = row+1; j < 8; j++)
	{
		if(ind){
			if(board[j][col]==player||board[j][col]==0){
				break;
			}
			board[j][col]=player;
		}
		else{
			if(board[j][col]==player){
				ind =1;
				j=row;
			}
			else if(board[j][col] == 0){
				break;
			}
		}
	}

	/*up ray shooted from inserted piece, checking for flips to make*/
	ind = 0;
	for (j = row-1; j >=0; j--)
	{
		if(ind){
			if(board[j][col]==player||board[j][col]==0){
				break;
			}
			board[j][col]=player;
		}
		else{
			if(board[j][col]==player){
				ind =1;
				j=row;
			}
			else if(board[j][col] == 0){
				break;
			}
		}
	}

	/*diagonal right up  ray shooted from inserted piece, checking for flips to make*/
	ind = 0;
	for (j = 1; ((col+j)< 8) && ((row-j)>=0); j++)
	{
		if(ind){
			if(board[row-j][col+j]==player||board[row-j][col+j]==0){
				break;
			}
			board[row-j][col+j]=player;
		}
		else{
			if(board[row-j][col+j]==player){
				ind =1;
				j=0;
			}
			else if(board[row-j][col+j] == 0){
				break;
			}
		}
	}

	
	/*diagonal right down  ray shooted from inserted piece, checking for flips to make*/
	ind = 0;
	for (j = 1; ((col+j)< 8) && ((row+j)<8); j++)
	{
		if(ind){
			if(board[row+j][col+j]==player||board[row+j][col+j]==0){
				break;
			}
			board[row+j][col+j]=player;
		}
		else{
			if(board[row+j][col+j]==player){
				ind =1;
				j=0;
			}
			else if(board[row+j][col+j] == 0){
				break;
			}
		}
	}


	/*diagonal left up  ray shooted from inserted piece, checking for flips to make*/
	ind = 0;
	for (j = 1; ((col-j)>=0) && ((row-j)>=0); j++)
	{
		if(ind){
			if(board[row-j][col-j]==player||board[row-j][col-j]==0){
				break;
			}
			board[row-j][col-j]=player;
		}
		else{
			if(board[row-j][col-j]==player){
				ind =1;
				j=0;
			}
			else if(board[row-j][col-j] == 0){
				break;
			}
		}
	}

	/*diagonal left down  ray shooted from inserted piece, checking for flips to make*/
	ind = 0;
	for (j = 1; ((col-j)>= 0) && ((row+j)<8); j++)
	{
		if(ind){
			if(board[row+j][col-j]==player||board[row+j][col-j]==0){
				break;
			}
			board[row+j][col-j]=player;
		}
		else{
			if(board[row+j][col-j]==player){
				ind =1;
				j=0;
			}
			else if(board[row+j][col-j] == 0){
				break;
			}
		}
	}
	
	/*color the actual piece added*/
	board[row][col]=player;


	return board;

}

/*free the whole abpruning tree from given root vertex*/
void freeABPruningTree(vertex root){
	/*free the board*/
	switch(game){
		case 1:
			freeBoard(root->game_state,3);
			break;
		case 2:
			freeBoard(root->game_state,6);
			break;		
		case 3:
			freeBoard(root->game_state,8);
			break;	
		default:
			printf("Error: no selection has been made about the game.\n");
	}
	/*we are in a leaf*/
	if(root->edges == NULL){
		free(root);
		return ;
	}

	element cur,prev;
	cur = root->edges->head;
	/*we have children*/
	while(cur!=NULL){
		prev = cur;
		cur=cur->next;
		freeABPruningTree(prev->node);
		free(prev);
	}
	
	free(root->edges);
	free(root);
}
