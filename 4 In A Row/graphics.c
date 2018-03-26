/*


*/
#include "header.h"

/*create a control with given surface, rectangle, and optionally a button above and a button below (above and below
may be given as null when we don't want them)*/
control createControl(SDL_Surface *sfc, SDL_Rect rect, control bAbove, control bBelow, char* cname){
	/*create the surface with the relevant fields*/
	control c= (control)malloc(sizeof(struct control_s));
	assert(c!=NULL);
	assert(sfc!=NULL);

	c->surface = sfc;
	c->rectangle = rect;
	c->buttonAbove = bAbove;
	c->buttonBelow = bBelow;
	c->children = NULL;
	if(cname == NULL){
		c->name = NULL;
		return c;
	}
	/*allocate memory for the name*/
	char * futureName = (char*)malloc(sizeof(char)* (strlen(cname)+1));
	strcpy(futureName,cname);
	c->name = futureName;
	return c;
}

/*add the control newChild as a child of father.*/
void addControlChild(control father, control newChild){
	/*if it's the first, we init it and set the new child*/
	if(father->children==NULL){
		controlLL c= getControlLL();
		assert(c!=NULL);
		c->cont= newChild;
		father->children=c;
	}
	/*simply adding a new son*/
	else{
		controlLL c= getControlLL();
		assert(c!=NULL);
		c->cont= newChild;
		c->next=father->children;
		father->children=c;

	}
}

/*return whether the given button was clicked in the event*/
int isClicked(control ctrl, SDL_Event e){
	return inRect( e.button.x, e.button.y ,ctrl->rectangle);
}

/*return whether the coorrdinates (x,y) are inside the given rectangle*/
int inRect( int x, int y ,SDL_Rect rect){
    return ( ( x >= rect.x && x <= rect.x + rect.w ) && ( y >= rect.y && y <= rect.y + rect.h ) );
}

/*return memory allocation of a control linked list*/
controlLL getControlLL(){
	controlLL c= (controlLL)malloc(sizeof(struct controlLL_s));
	c->next=NULL;
	assert(c!=NULL);
	return c;
}

/*draw the screen given the control root. the variable flip indicates whether we want to flip after all subtree
of given root is drawen. most of the time we will call the function with flip=1*/
void drawScreen(control root, int flip){
	/*don't draw nulls to screen. duh...*/
	if(root==NULL){
		return;
	}

	/*we do not draw the WINDOW, it's just there*/
	if(root->surface!=window){
		assert(root!=NULL);
		assert(root->surface!=NULL);
		/*draw current root*/
		drawControl(root);
	}
	/*iterate over children, and call recursively*/
	controlLL iter= root->children;
	while(iter!=NULL){
		/*call children in recursion, without flipping them at the end*/
		drawScreen(iter->cont,0);
		iter=iter->next;
	}

	/*managing the flip at the end*/
	if(flip==1){
		assert(window!=NULL);
		int succeeded= SDL_Flip(window);
		if (succeeded < 0) {
		printf("ERROR: unable to flip to new screen. SDL: %s\n", SDL_GetError());
		return;
		}
	}
}
/*draw a given control to the one and only window*/
void drawControl(control ctrl){
	if (SDL_BlitSurface(ctrl->surface, NULL, window, &(ctrl->rectangle)) != 0) {
			SDL_FreeSurface(ctrl->surface);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			return;
	}
}

/*get image from dir, and assert that it worked*/
SDL_Surface *getImageSurface(char* dir){
	SDL_Surface *imgsfc = SDL_LoadBMP(dir);
	assert(imgsfc);
	return imgsfc;
}

SDL_Rect getRect(int x, int y, int xOff, int yOff){
	SDL_Rect ans = {x,y,xOff,yOff};
	return ans;
}
/*free the UI tree from given root and down. We do take care of if the given control contains
 window as surface or not.*/
void freeUITree(control root){
	/*don't draw nulls to screen. duh...*/
	if(root==NULL){
		return;
	}
	/*iterate over children, and call recursively*/
	controlLL iter= root->children;
	controlLL nxt;
	while(iter!=NULL){
		/*call children in recursion, and meanwhile free the linked list of children*/
		freeUITree(iter->cont);
		nxt = iter->next;
		free(iter);
		iter=nxt;
	}

	if(root->surface!=window){
		 SDL_FreeSurface(root->surface);
	}
}

/*clear the screen to get a black screen.*/
void clearScreen(){
	SDL_FillRect(window, NULL, 0);
	SDL_Flip(window);
}

/*Takes the control C, and if i=0, it changes the white to clear, and if i=1 it changes the clear to white.*/
void drawColor(control c, int i){
	SDL_FillRect(window,&(c->rectangle),0);
	/*input i=0 is given when we want to change white to black (button is selected)*/
	if(i==0){

		if (SDL_SetColorKey((c->surface), SDL_SRCCOLORKEY, SDL_MapRGB((c->surface)->format, 255, 255, 255)) != 0) {
			printf("ERROR: failed to set color key: %s\n", SDL_GetError());
			SDL_FreeSurface(c->surface);
			return ;
		}
	}
	/*input i=1 is given when we want to change black to white (button is deselected)*/
	else{

		if (SDL_SetColorKey((c->surface), SDL_MapRGB((c->surface)->format, 255, 255, 255) ,SDL_SRCCOLORKEY) != 0) {
			printf("ERROR: failed to set color key: %s\n", SDL_GetError());
			SDL_FreeSurface(c->surface);
			return ;
		}
	}
}

/*manages the coloring after pressing up or down. it unmarks the old one, and marks the new one. 1 is moving up, and -1 is moving down.*/
void manageButtonColors(control oldButton, int i){
	control newButton = (i==1)?(oldButton->buttonAbove):(oldButton->buttonBelow);
	drawColor(oldButton, 1);
	drawColor(newButton, 0);
	drawControl(oldButton);
	drawControl(newButton);
	SDL_Flip(window);
}

/*get the button panel for the game which will be located on the right hand side.*/
control getButtonPanel(){
	/*create the root transparent window*/
	control buttonPanel = createControl(window, getRect(600,0,200,600) , NULL, NULL, "button panel");
	/*create the buttons, and add them as children of the buttonPanel*/
	control restartGame = createControl(SDL_LoadBMP("Textures/RestartGame.bmp") , getRect(3*Width/4,0,Width/4,Height/7),NULL, NULL,"restart game");
	addControlChild(buttonPanel,restartGame);
	control saveGame = createControl(SDL_LoadBMP("Textures/SaveGame.bmp") , getRect(3*Width/4,Height/7,Width/4,Height/7),NULL, NULL,"save game");
	addControlChild(buttonPanel,saveGame);
	control pauseUnpause;
	/*switch loading of pause button or unpause button*/
	if(paused == 1){
		pauseUnpause = createControl(SDL_LoadBMP("Textures/Unpause.bmp") , getRect(3*Width/4,2*Height/7,Width/4,Height/7),NULL, NULL, "pause");
		addControlChild(buttonPanel,pauseUnpause);
	}
	else{
		pauseUnpause = createControl(SDL_LoadBMP("Textures/Pause.bmp") , getRect(3*Width/4,2*Height/7,Width/4,Height/7),NULL, NULL, "pause");
		addControlChild(buttonPanel,pauseUnpause);
	}
	control mainMenu = createControl(SDL_LoadBMP("Textures/MainMenu.bmp") , getRect(3*Width/4,3*Height/7,Width/4,Height/7),NULL, NULL,"main menu");
	addControlChild(buttonPanel,mainMenu);
	control difficultyp1 = createControl(SDL_LoadBMP("Textures/DifficultyP1.bmp") , getRect(3*Width/4,4*Height/7,Width/4,Height/7),NULL, NULL,"difficultyp1");
	addControlChild(buttonPanel,difficultyp1);
	control difficultyp2 = createControl(SDL_LoadBMP("Textures/DifficultyP2.bmp") , getRect(3*Width/4,5*Height/7,Width/4,Height/7),NULL, NULL,"difficultyp2");
	addControlChild(buttonPanel,difficultyp2);
	control quit = createControl(SDL_LoadBMP("Textures/quitGame.bmp") , getRect(3*Width/4,6*Height/7,Width/4,Height/7),NULL, NULL,"quit game");
	addControlChild(buttonPanel,quit);
	
	/*connect between all the buttons, who is above whom and who is below whom*/
	buttonPanel->buttonBelow = restartGame;
	restartGame->buttonAbove = buttonPanel;
	restartGame->buttonBelow = saveGame;
	saveGame->buttonAbove = restartGame;
	saveGame->buttonBelow = pauseUnpause;
	pauseUnpause->buttonAbove = saveGame;
	pauseUnpause->buttonBelow = mainMenu;
	mainMenu->buttonAbove = pauseUnpause;
	mainMenu->buttonBelow = difficultyp1;
	difficultyp1->buttonAbove = mainMenu;
	difficultyp1->buttonBelow = difficultyp2;
	difficultyp2->buttonAbove = difficultyp1;
	difficultyp2->buttonBelow = quit;
	quit->buttonAbove = difficultyp2;
	quit->buttonBelow = buttonPanel;

	return buttonPanel;

	
}
/*return a control representing the game area*/
control getGameArea(board_t board){
	int winner;
	/*if paused, we show pause screen instead of board*/
	if(paused==1){
		control gameArea = createControl(window, getRect(0,0,600,600) , NULL, NULL, "game area");
		control dummySon = createControl(SDL_LoadBMP("Textures/GamePause.bmp"), getRect(0,0,600,600) , NULL, NULL, "game pause");
		addControlChild(gameArea,dummySon);
		return gameArea;
	}


	printf("Getting game area control\n");
	control gameArea = createControl(window, getRect(0,0,600,600) , NULL, NULL, "game area");
	/*if game is over, we also print the stripe showing either the winning combination, or if someone won or tied*/
	if(gameOver){
		if(isFull(board)){
			/*not reversi => no ties*/
			if(game!=3){
				/*if someone has won*/
				if(score(board) == 100000 || score(board) == -100000){
					control winningStripe = getWinningStripe(board);
					addControlChild(gameArea,winningStripe);
				}
				/*if tied*/
				else{
					control gameTie = createControl(SDL_LoadBMP("Textures/GameTie.bmp"), getRect(0,250,600,100) , NULL, NULL, "game tie");
					addControlChild(gameArea,gameTie);
				}	
			}
			else{
				/*get the winner of the game, and show the picture accordingly*/
				winner = getWinnerReversi(board);
				if(winner == -1){
					control p1Won = createControl(SDL_LoadBMP("Textures/P1Won.bmp"), getRect(0,250,600,100) , NULL, NULL, "p1 won");
					addControlChild(gameArea,p1Won);
				}
				else if(winner == 1){
					control p2Won = createControl(SDL_LoadBMP("Textures/P2Won.bmp"), getRect(0,250,600,100) , NULL, NULL, "p2 won");
					addControlChild(gameArea,p2Won);
				}
				else{
					control gameTie = createControl(SDL_LoadBMP("Textures/GameTie.bmp"), getRect(0,250,600,100) , NULL, NULL, "game tie");
					addControlChild(gameArea,gameTie);
				}
			}
		}
		else{
			control winningStripe = getWinningStripe(board);
			addControlChild(gameArea,winningStripe);
		}
	}
	/*get the columns and rows sizes*/
	int colNum=-1,rowNum=-1;
	switch(game){
		case 1:
			colNum=3;
			rowNum=3;
			break;
		case 2:
			colNum=7;
			rowNum=6;			
			break;
		case 3:
			colNum=8;
			rowNum=8;
			break;
		default:
			printf("Error: Game type has not been initialized yet!!!");
	}

	printf("Setting all board pieces as children of gameArea\n");

	char pieceName[100];
	int i,j;
	control curr;
	SDL_Surface *currPieceImage;
	for (i = 0; i < rowNum; i++)
	{
		for (j = 0; j < colNum; j++)
		{
			/*for each piece we load the relevant picture, based on its coordinates given in the piece name.*/
			sprintf(pieceName, "%d %d", i, j);
			currPieceImage = getPieceImg(board[i][j]);
			/*create the square itself*/
			curr = createControl(currPieceImage , getRect(j*(((3*Width)/4)/colNum),i*(Height/rowNum),((3*Width)/4)/colNum,Height/rowNum),NULL, NULL,pieceName);
			addControlChild(gameArea,curr);
		}
	}
	printf("finished setting all board pieces as children of gameArea\n");
	return gameArea;
}

/*Given a piece and knowing which game we are playing returns a new surface with the correct image*/
SDL_Surface *getPieceImg(int piece){
	assert(piece==0 ||piece ==1 ||piece==-1);
	char imgAdd[100];
	/*the image address*/
	sprintf(imgAdd, "Textures/%d/%d.bmp", game, piece);
	/*load the image*/
	SDL_Surface *img = SDL_LoadBMP(imgAdd);
	/*assert if the load have failed*/
	assert(img != NULL);
	return img;
}

/*Given the control that represents a piece of the board and a piece updates the control to have the right image for the given piece*/
void updateBoardImage(control c, int piece){
	printf("freeing the prevous surface\n");
	/*we free the prev' image*/
	assert(c->surface!=NULL);
	SDL_FreeSurface(c->surface);
	printf("loads the new image\n");
	/*and load the new one*/
	c->surface = getPieceImg(piece);
	assert(c->surface!=NULL);
}

/*Updates the board in a given game area according to a given board, we assume here that the name of each control of the board is "row col".*/
void updateBoardGame(control gameArea, board_t board){
	controlLL iter = gameArea->children;
	int row, col;
	/*all of the board parts a children of the gameArea*/
	while ((iter != NULL) && (iter->cont!=NULL)){
		assert(iter->cont!=NULL);
		sscanf(iter->cont->name, "%d %d", &row, &col);
		printf("Enering updateBoardImage\n");
		updateBoardImage(iter->cont, board[row][col]);
		printf("done updateBoardImage\n");
		iter = iter->next;
	}
}
/*Updates the game area and then draw it.*/
void drawBoard(control gameArea, board_t board){
	updateBoardGame(gameArea, board);
	drawScreen(gameArea, 1);
}

/*draws a new screen to set the difficulty, and returns the difficuty chosen.*/
void setDifficulty(int player){
	assert(player==1 ||player==2);
	/*if we are in tic-tac-toe, we just set the difficulty to 9...*/
	if(game == 1){
		if(player == 1){
			difficultyP1 = 9;
		}
		else{
			difficultyP2 = 9;
		}
		return ;
	}

	/*set the number of possible difficulties*/
	int numOfDifficulties = -1;
	switch(game){
		case 1:
			printf("We shouldn't be setting difficulty for tic-tac-toe!!!");
			assert(0);
			break;
		case 2:
			numOfDifficulties =	7;
			break;
		case 3:
			numOfDifficulties = 4;
			break;
		default:
			printf("Error: Game type has not been initialized yet!!!");
	}


	clearScreen();
	printf("Entered setDifficulty function to choose the game difficulty\n");
	/*create the root control for window*/
	control root = createControl(window, getRect(0,0,1,1) , NULL, NULL, "window");

	/*load the title containing the user instructions*/
	char imageDir[100];
	control *controls= (control*)malloc(sizeof(control)*numOfDifficulties);
	int i ;
	if(player==1){
		control title = createControl(SDL_LoadBMP("Textures/Difficulties/Title1.bmp") , getRect(0,0,Width,75),NULL, NULL,"Title");
			addControlChild(root,title);
	}
	else{
		control title = createControl(SDL_LoadBMP("Textures/Difficulties/Title2.bmp") , getRect(0,0,Width,75),NULL, NULL,"Title");
		addControlChild(root,title);
	}

	/*load the different difficulty levels*/
	for(i=0;i<numOfDifficulties;i++){
		sprintf(imageDir, "Textures/Difficulties/%d/%d.bmp", game, i+1);
		controls[i] = createControl(SDL_LoadBMP(imageDir) , getRect(0,75+i*((Height-75)/numOfDifficulties),Width,(Height-75)/numOfDifficulties),NULL, NULL,NULL);
		addControlChild(root,controls[i]);
	}

	/*draw them to screen.*/
	drawScreen(root, 1);
	int chosenDifficultyIndex = 0;
	SDL_Event e;

	/*get user feedback about difficulty*/
	while(1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				/*clicked button up or down on keyboard*/
				case (SDL_MOUSEBUTTONUP):
					/*find clicked button, and set the difficulty accordingly*/
					for(i=0;i<numOfDifficulties;i++){
						if(isClicked(controls[i],e)){
							printf("Clicked on difficulty %d\n",i+1);
							freeUITree(root);
							if(player == 1){
								difficultyP1 = (i+1);
							}
							else{
								difficultyP2 = (i+1);
							}
							return;
						}
					}
				/*keyboard pressed*/
				case (SDL_KEYDOWN):
					switch(e.key.keysym.sym ){
						/*pressed up. set the new chosen difficulty index for analyzing when user presses enter. set the new selected button*/
						case SDLK_UP:
							drawColor(controls[chosenDifficultyIndex], 1);
							drawControl(controls[chosenDifficultyIndex]);
							chosenDifficultyIndex = (chosenDifficultyIndex-1+numOfDifficulties) % numOfDifficulties;
							drawColor(controls[chosenDifficultyIndex], 0);
							drawControl(controls[chosenDifficultyIndex]);
							SDL_Flip(window);
							break;
						/*pressed down. set the new chosen difficulty index for analyzing when user presses enter. set the new selected button*/
						case SDLK_DOWN:
							drawColor(controls[chosenDifficultyIndex], 1);
							drawControl(controls[chosenDifficultyIndex]);
							chosenDifficultyIndex = (chosenDifficultyIndex+1+numOfDifficulties)%numOfDifficulties;
							drawColor(controls[chosenDifficultyIndex], 0);
							drawControl(controls[chosenDifficultyIndex]);
							SDL_Flip(window);
							break;
						/*confirm the selection, and set the difficulty*/
						case SDLK_RETURN:
							printf("Pressed return on difficulty %d\n",chosenDifficultyIndex+1);
							freeUITree(root);
							if(player == 1){
								difficultyP1 = (i+1);
							}
							else{
								difficultyP2 = (i+1);
							}
							return;
						default:
							break;
					}

				default:
					continue;
			}
		}
		SDL_Delay(1);
	}
}

/*show the setting difficulty menu on the screen, taking into account the different players of the game.*/
void setDifficultyByMode(){
	switch(mode){
		case PVP:
			return;
		case PVC:
			setDifficulty(2);
			break;
		case CVP:
			setDifficulty(1);
			break;
		case CVC:
			setDifficulty(1);
			setDifficulty(2);
			break;
		default:
			printf("ERROR: No mode has been chosen!!!");
			assert(0);

	}
}