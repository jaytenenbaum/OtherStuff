/*
	

*/
#include "header.h"
/*initiate a blank new game, and at the end call the main game function to run it.*/
void initiateGame(){
	/*game isn't over*/
	gameOver = 0;
	printf("Started game with difficulties: %d ,%d\n",difficultyP1,difficultyP2);
	printf("Entering Initiate Game Function\n");
	board_t board;
	/*get relevant board size*/
	switch(game){
		case 1:
			board= getBoard(3,3);
			break;
		case 2:
			board= getBoard(6,7);
			break;		
		case 3:
			board= getBoard(8,8);
			/*init 4 middle squares*/
			board[3][3]=1;
			board[4][4]=1;
			board[3][4]=-1;
			board[4][3]=-1;
			break;	
		default:
			printf("Error: no selection has been made about the game1.\n");
	}
	printf("Starting new game from Initiate Game\n");
	/*start the game*/
	Game(board , -1);
	
}

/*returns a board_t with rows rows and cols cols*/
board_t getBoard(int rows, int cols){

	board_t board= (board_t)calloc(rows,sizeof(int*));
	int i;
	for(i=0;i<rows;i++){
		board[i]=(int*)calloc(cols,sizeof(int));
		assert(board[i]!=NULL);
	}
	assert(board!=NULL);

	return board;

}

/*the main game loop. This runs a game, and manages al the different user inputs, and the main user/computer loop.*/
void Game(board_t board, int nextPlayer){
	/*if game's over, move to relevant panel*/
	if(gameOver){
		printf("g.o. detected\n");
		gameOverPanel(nextPlayer,board);
		return;
	}

	clearScreen();
	printf("Entered function \"Game\" \n");
	/*get the dummy transparent control, with the buttons of the right panel as its children*/
	control root = createControl(window,getRect(0,0,1,1), NULL, NULL,"window");
	/*get the panel containing the menu on the right*/
	control rightButtonPanel = getButtonPanel();
	addControlChild(root, rightButtonPanel);

	assert(board!=NULL);
	/*getGameArea returns a control that will be a dummy transparent one, and all its children will be the different pieces of the board.*/
	control gameArea = getGameArea(board);
	addControlChild(root, gameArea);
	/*finally draw the screen itself to show to user*/
	drawScreen(root,1);
	assert(mode!=-1);

	/* mainPlayer is the variabel with of the main player. in modes PVP and CVC it changes,
	and the main player is always the current one. in mode PVC and CVP the main player is always
	the user. */
	int mainPlayer=100;
	if(mode!=CVP){
		mainPlayer = nextPlayer;
	}
	else{
		mainPlayer = 1;
		if(nextPlayer==-1){
			board = pcMove(-1,board);
			drawBoard(gameArea, board);
		}
	}

	assert(mainPlayer!=100);
	int row,col;
	/*variable to represent the clicked button of the board in coordinates "i j" */
	char* receivedCoordinates = NULL;


	SDL_Event e;
	/*while the game isstill in the normal loop*/
	while(1){
			assert(board!=NULL);
			/*check if game is over. If it is, move to the game over panel*/
			gameOver = isGameOver(board);
			if(gameOver){
				printf("Game Over Detected!!! \n");
				freeUITree(root);
				gameOverPanel(mainPlayer,board);
				return;
			}
		/*while there are still events (because of the while(1) outside, we still get later events)*/	
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				/*user has clicked*/
				case (SDL_MOUSEBUTTONUP):
					/*THIS WILL ONLY BE RELEVANT WHEN IT'S USER'S TURN*/
					if(isClicked(gameArea,e)){
						if(mode == CVC|| (paused==1)){
							continue;
						}
						/*user's turn has been done. get the chosen button, check if move's OK, 
						add the move to the board, refresh board, and turn to other player's turn
						(it will depend on mode of course...)*/

						receivedCoordinates = getButtonClickedName(gameArea, e);
						assert(receivedCoordinates!=NULL);
						printf("%s\n",receivedCoordinates);
						sscanf(receivedCoordinates, "%d %d",&row,&col);
						printf("You've clicked on the button in coordinates %d,%d\n",row,col);
						/*if the move is legal, do it and respond in the wanted way (if it's against computer for instance, make the pc's move)*/
						if(isLegalBoardMove(board, row, col, mainPlayer)){
							/*set new board and update screen*/
							board = userMove(board,row,col,mainPlayer);
							drawBoard(gameArea, board);
							/*manage again if game is over*/
							gameOver = isGameOver(board);
							if(gameOver){
								printf("Game Over Detected!!! \n");
								freeUITree(root);
								gameOverPanel(mainPlayer,board);
								return;
							}
							printf("game not over \n");
							/*if mode is PVP, we just change our main player, and continue with loop normally*/
							if(mode==PVP){
							/* next move is of the second player, therefore we don't make pc move*/
								mainPlayer=-1*mainPlayer;
							}
							else{
							/* make pc move (with the not main (or other) player)*/
								do{
									/*if the game isn't reversi, or it is, and we pc can make a move*/
									if((game!=3) ||  canMakeAMoveReversi(board,-1*mainPlayer)){
										/*set new board after the pc's move*/
										board = pcMove(-1*mainPlayer, board);
										assert(board!=NULL);
										printf("going to drawBoard!\n");
										drawBoard(gameArea, board);
									}
									/*check again if game is over*/
									gameOver = isGameOver(board);
									if(gameOver){
										printf("Game Over Detected!!! \n");
										freeUITree(root);
										gameOverPanel(-1*mainPlayer,board);
										return;
									}

								/*continue with the pc's move as long as there isn't a move for the player*/
								}while((game==3) && (!canMakeAMoveReversi(board,mainPlayer)) );
							}
						}
						else{
							continue;
						}
						break;			
					}
					/*if we click right button panel, manage the click*/
					if(isClicked(rightButtonPanel,e)){
						int ret = manageRightPanelEvent(root, rightButtonPanel,e,board, mainPlayer);
						if(ret == 1){
							/*here we return from the function wanting to finish game and exit program.
							note that we freed all necessary stuff (board and uitree) in the managerightpanelevent
							function, so no need here.*/
							return ;
						}
						if(ret == 0){
							/*here we come back from another panel to game*/
							drawScreen(root,1);
						}
						if(ret ==2){
							/*here we come back in the remainder of the last game (when we pause a game, we throw away the last tree, and start a game from scratch
							using the board and next player. We return here after the new game created ends, and we should just exit.)*/
							return ;
						}
					}
				default:
					continue;
			}
		}
		if((mode==CVC) && (paused==0)){
			/*when the mode is CVC, we make the mainPlayer's move, and update the main player.*/
			if(game==3){
				printf("\nchecking if another move can be done\n");
				if(!canMakeAMoveReversi(board,mainPlayer)){
					mainPlayer=-1*mainPlayer;
					continue;
				}
			}
			pcMove(mainPlayer,board);
			assert(board!=NULL);
			drawBoard(gameArea, board);
			mainPlayer=-1*mainPlayer;
			SDL_Delay(100);
				//printf("Done With PC for now\n");
			}
		SDL_Delay(1);
	}
		
}

/*return the new board after pc's move, and free given board*/
board_t pcMove(int currentPlayer,board_t board){
	printf("Executing pc move\n");
	vertex currentStateVertex = (vertex)malloc(sizeof(struct vertex_s));
	/*set new state board sizes*/
	switch(game){
		case 1:
			currentStateVertex -> game_state = copy(board,3,3);
			break;
		case 2:
			currentStateVertex -> game_state = copy(board,6,7);
			break;		
		case 3:
			currentStateVertex -> game_state = copy(board,8,8);
			break;	
		default:
			printf("Error: no selection has been made about the game2.\n");
	}

	/*set the new fields*/
	currentStateVertex -> nextPlayer = currentPlayer;
	currentStateVertex -> edges = NULL;
	/*set the depth according to the player*/
	int depth = (currentPlayer==-1)?difficultyP1:difficultyP2;
	printf("starting minimax with %d\n",currentPlayer);
	/*get the best move score*/
	int bestScore = alphaBetaPruning(currentStateVertex,alphaINIT,betaINIT,currentPlayer,depth);
	/*search for the best move with that score in children*/
	element iter = currentStateVertex->edges->head;
	printf("Searching for next best move with abpruning score\n");
	while(iter!=NULL){
		assert(iter->node!=NULL);
		if(iter->node->score == bestScore){
			break;	
		}
		iter = iter->next;
	}
	assert(iter!=NULL);
	printf("PC move's next board: \n");
	printAsciiBoard(iter->node->game_state);
	/*manage deletion of old board, and return new board.*/
	switch(game){
		case 1:
			freeBoard(board,3);
			board = copy(iter->node->game_state, 3,  3);
			break;
		case 2:
			freeBoard(board,6);
			board = copy(iter->node->game_state, 6,  7);
			break;		
		case 3:
			freeBoard(board,8);
			board = copy(iter->node->game_state, 8,  8);
			break;	
		default:
			printf("Error: no selection has been made about the game3.\n");
	}
	assert(board!=NULL);
	assert(currentStateVertex!=NULL);
	freeABPruningTree(currentStateVertex);
	printf("done with pcMove\n");
	assert(board!=NULL);
	return board;
}

/*return the new board after pc's move, and free given board*/
board_t userMove(board_t board, int row,int col, int mainPlayer){
	printf("Executing user move, to row %d, and col %d\n",row,col);
	board_t res = getNewBoard(board,row,col,mainPlayer);
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
			printf("Error: no selection has been made about the game4.\n");
	}
	return res;
	
}

/*We call this function with fatherControl, and it goes over the children (the individual children buttons)
and returns the name of the child button we clicked in.*/
char *getButtonClickedName(control fatherControl, SDL_Event e){
	controlLL iter= fatherControl->children;
	while(iter!=NULL){
		/*call children in recursion, without flipping them at the end*/
		if(isClicked(iter->cont,e)){
			return iter->cont->name;
		}
		iter=iter->next;
	}
	printf("ERROR: No square of board has been clicked!!!\n");
	return NULL;
}

/*returns 1 if when we return from this function we should exit game after returning, returns -1 if no
button was chosen, and returns 0 when we come back to the same game after switching a panel for
user selection (difficulty, pause, etc...)*/
int manageRightPanelEvent( control root, control rightButtonPanel,SDL_Event e, board_t board, int nextPlayer){
	char* clickedName = getButtonClickedName(rightButtonPanel,e);
	/*to restart game, just free everything and set game as not over*/
	if(!strcmp(clickedName,"restart game")){
		gameOver = 0;
		freeUITree(root);
		freeBoardWithGlobalGame(board);
		initiateGame();
		return 1;
	}
	/*use the load game menu to save the game, not that we just give it 1 as input for the menu to change from loading to saving*/
	else if(!strcmp(clickedName,"save game")){
		loadGameMenu(1, board, nextPlayer);
		return 0;
	}
	/*to pause, just update the paused variable, and continue with the game*/
	else if(!strcmp(clickedName,"pause")){
		if(paused==0){
			freeUITree(root);
			paused = 1;
			Game(board, nextPlayer);
		}
		else{
			freeUITree(root);
			paused = 0;
			Game(board, nextPlayer);
		}
		return 2;
	}
	/*to go to main menu, free the tree, and just call main menu*/
	else if(!strcmp(clickedName,"main menu")){
		freeUITree(root);
		freeBoardWithGlobalGame(board);
		mainMenu();
		return 1;
	}
	else if(!strcmp(clickedName,"difficultyp1")){
		setDifficulty(1);
		return 0;
		
	}
	else if(!strcmp(clickedName,"difficultyp2")){
		setDifficulty(2);
		return 0;
	}
	/*to quit just free memory*/
	else if(!strcmp(clickedName,"quit game")){
		freeUITree(root);
		freeBoardWithGlobalGame(board);
		return 1;
	}
	else{
		printf("ERROR: no button from right panel has been chosen!!!");
		return -1;
	}
}

/*print the board using ascii. primarily for debuggin uses*/
void printAsciiBoard(board_t board){
	int rows,cols;
	int i,j;
	/*set the size of board*/
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
			break;	
		default:
			printf("Error: no selection has been made about the game.\n");
	}
	/*print square by square*/
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < cols; ++j)
		{
			printf("%d\t",board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

/*the panel which we get to after game is over. to possibility to click on the board itself, only to press the right button panel*/
void gameOverPanel(int nextPlayer,board_t board){
	gameOver = 1;
	clearScreen();
	printf("Entered function \"GameOverPanel\" \n");
	/*get the dummy transparent control, with the buttons of the right panel as its children*/
	control root = createControl(window,getRect(0,0,1,1), NULL, NULL,"window");
	control rightButtonPanel = getButtonPanel();
	addControlChild(root, rightButtonPanel);

	assert(board!=NULL);
	/*getGameArea returns a control that will be a dummy transparent one, and all its children will be the different pieces of the board.*/
	control gameArea = getGameArea(board);
	addControlChild(root, gameArea);

	drawScreen(root,1);
	assert(mode!=-1);
	SDL_Event e;
	while(1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case (SDL_MOUSEBUTTONUP):
					if(isClicked(rightButtonPanel,e)){
						int ret = manageRightPanelEvent(root, rightButtonPanel,e,board, nextPlayer);
						if(ret == 1){
							/*here we return from the function wanting to finish game and exit program.
							note that we freed all necessary stuff (board and uitree) in the managerightpanelevent
							function, so no need here.*/
							return ;
						}
						if(ret == 0){
							/*here we come back from another panel to game*/
							drawScreen(root,1);
						}
						if(ret ==2){
							/*here we come back in the remainder of the last game (when we pause a game, we throw away the last tree, and start a game from scratch
							using the board and next player. We return here after the new game created ends, and we should just exit.)*/
							return ;
						}
					}
				default:
					continue;
			}
		}
		SDL_Delay(1);
	}
	freeUITree(root);
	return;
}