

/*things to remember to do:
1. the load function replaces the red dots.
2. WE MOST!!!! CHECK THE MEMO LEAK!!!!!!!!!!!!
*/
/*
coloring looks something like this:
Uint32 colorkey = SDL_MapRGB(image->format,255,255,255);
SDL_SetColorKey(image, SDL_SRCCOLORKEY,colorkey);
*/
#include "header.h"
/*
create windows; main menu; calls a game;

*/

/*set all the global variables to initialized values*/
SDL_Surface* window;
int game=0;
int difficultyP1 = -1;
int difficultyP2 = -1;
int mode = -1;
int paused = 0;
int gameOver = 0;

/*the main loop, showing the pre- game windows, going to the main menu, and cleaning and showing finish screen at the end*/
int main(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}
	atexit(SDL_Quit);
	
	SDL_WM_SetCaption("SDL Test", "SDL Test");
	window = SDL_SetVideoMode(Width, Height,0,SDL_HWSURFACE|SDL_DOUBLEBUF);
	preGame();
	mainMenu();
	showClosingScreen();
	SDL_FreeSurface(window);

	return 1;
	
}


/*the main menu of the game. gets the user's selection and redirects to the next screen*/
void mainMenu(){
	clearScreen();
	/*create the root control for window*/
	control root = createControl(window,getRect(0,0,1,1), NULL, NULL,"window");
	/*create the three buttons and connect between them*/
	control newGame = createControl(SDL_LoadBMP("Textures/NewGame.bmp") , getRect(0,0,Width,Height/3),NULL, NULL,"new game");
	addControlChild(root,newGame);
	control quit = createControl(SDL_LoadBMP("Textures/Quit.bmp") , getRect(0,Height/3,Width,Height/3),newGame, NULL,"quit");
	addControlChild(root,quit);
	control loadGame = createControl(SDL_LoadBMP("Textures/LoadGame.bmp") , getRect(0,2*Height/3,Width,Height/3),quit, newGame,"load game");
	addControlChild(root,loadGame);
	/*finish connecting*/
	newGame->buttonAbove = loadGame;
	newGame->buttonBelow = quit;
	quit->buttonBelow = loadGame;

	/*draw them to screen.*/
	drawScreen(root, 1);
	
	/*we don't want to exit program yet*/
	int programQuit = 0;
	/*marks the current selected button with keyboard*/
	control chosenButton = newGame;
	printf("%s\n",chosenButton->name);
	/*while program hasn't quit*/
	while (!programQuit) {
		SDL_Event e;
		/*mange events*/
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				/*if clicked X in corner, we free the tree and exit*/
				case (SDL_QUIT):
					freeUITree(root);
					programQuit = 1;
					printf("Program has quit successfully\n");
					break;
				case (SDL_MOUSEBUTTONUP):
					/*pressed "New Game"*/
					if(isClicked(newGame,e)){
						printf("New Game has been clicked\n");
						freeUITree(root);
						chooseGame();
						/*we return here from called function, and we want to exit program*/
						programQuit = 1;
					}
					/*pressed "Quit"*/
					else if(isClicked(quit,e)){

						freeUITree(root);
						programQuit = 1;
						printf("Program has quit successfully\n");
					}
					/*pressed "Load Game"*/
					else if(isClicked(loadGame,e)){
						printf("Load Game has been clicked\n");
						freeUITree(root);
						loadGameMenu(0,NULL,0);
						/*we return here from called function, and we want to exit program*/
						programQuit = 1;
					}
					break;
				/*pressed keyboard button*/
				case (SDL_KEYDOWN):
					 switch(e.key.keysym.sym ){
					 	/*move up*/
						case SDLK_UP:
							printf("UP moving from %s",chosenButton->name);
							manageButtonColors(chosenButton,1);
							chosenButton = chosenButton->buttonAbove;
							printf("to: %s\n",chosenButton->name);
							break;
						/*move down*/
						case SDLK_DOWN:
							printf("DOWN moving from %s",chosenButton->name);
							manageButtonColors(chosenButton,-1);
							chosenButton = chosenButton->buttonBelow;
							printf("to: %s\n",chosenButton->name);

							break;
						/*press the selected button*/
						case SDLK_RETURN:
							printf("Enter executing function of button %s\n",chosenButton->name);
							/*start new game*/
							if (chosenButton == newGame){
								printf("Pressed enter on New Game\n");
								freeUITree(root);
								chooseGame();
								/*we return here from called function, and we want to exit program*/
								programQuit = 1;
							}
							/*quit the menu*/
							else if (chosenButton == quit){
								freeUITree(root);
								showClosingScreen();
								printf("Program quitted successfully\n");
								programQuit = 1;
							}
							/*load a game*/
							else if (chosenButton == loadGame){
								printf("Pressed enter on Load Game\n");
								freeUITree(root);
								loadGameMenu(0,NULL,0);
								/*we return here from called function, and we want to exit program*/
								programQuit = 1;
							}
							break;
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


/*open the new screen with the 4 new buttons of ttt, con4, reversi, cancel. we assume the buttons are divided into 4 stripes with equal height. it then starts the relevant game, according to the user's selection.*/
void chooseGame(){
	clearScreen();
	printf("Entered chooseGame function to choose the game type\n");
	/*create the root control for window*/
	control root = createControl(window, getRect(0,0,1,1) , NULL, NULL, "window");
	/*create the three buttons and connect between them*/
	control ttt = createControl(SDL_LoadBMP("Textures/TicTacToe.bmp") , getRect(0,0,Width,Height/4),NULL, NULL,"tic tac toe");
	addControlChild(root,ttt);
	control con4 = createControl(SDL_LoadBMP("Textures/Connect4.bmp") , getRect(0,Height/4,Width,Height/4),ttt, NULL,"connect 4");
	addControlChild(root,con4);
	control reversi = createControl(SDL_LoadBMP("Textures/Reversi.bmp") , getRect(0,2*Height/4,Width,Height/4),con4, NULL, "reversi");
	addControlChild(root,reversi);
	control cancel = createControl(SDL_LoadBMP("Textures/Cancel.bmp") , getRect(0,3*Height/4,Width,Height/4),reversi, ttt,"cancel");
	addControlChild(root,cancel);
	
	
	/*finish connecting*/
	ttt->buttonAbove = cancel;
	ttt->buttonBelow = con4;
	con4->buttonBelow = reversi;
	reversi->buttonBelow = cancel;
	
	
	/*draw them to screen.*/
	drawScreen(root, 1);
	control chosenButton = ttt;
	SDL_Event e;
	while(1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case (SDL_MOUSEBUTTONUP):
					/*pressed "ttt"*/
					if(isClicked(ttt,e)){
						printf("Clicked on Tic-Tac-Toe\n");
						freeUITree(root);
						game=1;
						chooseMode();
						setDifficultyByMode();
						initiateGame();
						return;
					}
					/*pressed "con4"*/
					else if(isClicked(con4,e)){
						printf("Clicked on con4\n");
						freeUITree(root);
						game=2;
						chooseMode();
						setDifficultyByMode();
						initiateGame();
						return;
					}
					/*pressed "reversi"*/
					else if(isClicked(reversi,e)){
						printf("Clicked on reversi\n");
						freeUITree(root);
						game=3;
						chooseMode();
						setDifficultyByMode();
						initiateGame();
						return;
					}
					/*pressed "cancel"*/
					else if(isClicked(cancel,e)){
						printf("Clicked on Cancel\n");
						freeUITree(root);
						mainMenu();
						return;
					}
				/*pressed keyboard*/
				case (SDL_KEYDOWN):
					switch(e.key.keysym.sym ){
						/*move up*/
						case SDLK_UP:
							
							printf("UP moving from %s",chosenButton->name);
							manageButtonColors(chosenButton,1);
							chosenButton = chosenButton->buttonAbove;
							printf("to: %s\n",chosenButton->name);
							break;
						/*move down*/
						case SDLK_DOWN:
							
							printf("DOWN moving from %s",chosenButton->name);
							manageButtonColors(chosenButton,-1);
							chosenButton = chosenButton->buttonBelow;
							printf("to: %s\n",chosenButton->name);
							break;
						/*select the marked button, and start the needed game processes before*/
						case SDLK_RETURN:
							if (chosenButton == ttt){
								printf("Pressed Enter on Tic-Tac-Toe\n");
								freeUITree(root);
								game=1;
								chooseMode();
								setDifficultyByMode();
								initiateGame();
								return;
							}
							else if (chosenButton == con4){
								printf("Pressed Enter on con4\n");
								freeUITree(root);
								game=2;
								chooseMode();
								setDifficultyByMode();
								initiateGame();
								return;
							}
							else if (chosenButton == reversi){
								printf("Pressed Enter on Reversi\n");
								freeUITree(root);
								game=3;
								chooseMode();
								setDifficultyByMode();
								initiateGame();
								return;
							}
							else if(chosenButton == cancel){
								printf("Pressed Enter on Cancel\n");
								freeUITree(root);
								mainMenu();
								return;
							}
							break;
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



/*show the load game menu or save game menu for user to choose the slot. note that when save is 0, we don't want to save the game,
we want' to load it, and if save is 1, we do actually want to load the game*/
void loadGameMenu(int save,board_t board, int nextPlayer){
	int numOfSlots = 5;
	clearScreen();
	if(save){
		printf("Entered save game menua to choose the slot\n");
	}
	else{
		printf("Entered loadGameMenu function to choose slot\n");
	}
	/*create the root control for window*/
	control root = createControl(window, getRect(0,0,1,1) , NULL, NULL, "window");
	/*open all the slots pictures*/
	char imageDir[100];
	control *controls= (control*)malloc(sizeof(control)*numOfSlots);
	int i;
	for(i=0;i<numOfSlots;i++){
		sprintf(imageDir, "Textures/Slots/%d.bmp", i+1);
		/*mark the existing slots*/
		if(slotExists(i+1)){
			printf("found!\n");
			addControlChild(root,createControl(SDL_LoadBMP("Textures/SaveExists.bmp") , getRect(0,i*(Height/numOfSlots),Width/10,Height/numOfSlots),NULL, NULL,NULL));
		}
		controls[i] = createControl(SDL_LoadBMP(imageDir) , getRect(0,i*(Height/numOfSlots),Width,Height/numOfSlots),NULL, NULL,NULL);
		addControlChild(root,controls[i]);

	}

	/*draw them to screen.*/
	drawScreen(root, 1);
	int chosenSlotIndex = 0;
	SDL_Event e;

	while(1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				/*if clicked on a slot*/
				case (SDL_MOUSEBUTTONUP):
					for(i=0;i<numOfSlots;i++){
						/*find clicked slot*/
						if(isClicked(controls[i],e)){
							printf("Clicked on on slot %d\n",i+1);
							freeUITree(root);
							/*manage whether to save the file or to load from file based on the value of save*/
							if(!save){
								gameOver = 0;
								loadGameStruct str = loadGameData(i+1);
								game = str->playedGame;
								chooseMode();
								setDifficultyByMode();
								Game(str->game_state,str->nextPlayer);
							}
							else{
								saveGameData(board, nextPlayer, i+1);
							}
							return;
						}
					}
						
				case (SDL_KEYDOWN):
					switch(e.key.keysym.sym ){
						/*move up*/
						case SDLK_UP:
							drawColor(controls[chosenSlotIndex], 1);
							drawControl(controls[chosenSlotIndex]);
							chosenSlotIndex = (chosenSlotIndex-1+numOfSlots) % numOfSlots;
							drawColor(controls[chosenSlotIndex], 0);
							drawControl(controls[chosenSlotIndex]);
							drawScreen(root, 1);
							break;
						/*move down*/
						case SDLK_DOWN:
							drawColor(controls[chosenSlotIndex], 1);
							drawControl(controls[chosenSlotIndex]);
							chosenSlotIndex = (chosenSlotIndex+1+numOfSlots)%numOfSlots;
							drawColor(controls[chosenSlotIndex], 0);
							drawControl(controls[chosenSlotIndex]);
							drawScreen(root, 1);
							break;
						/*choose selected button*/
						case SDLK_RETURN:
							printf("Pressed return on on slot %d\n",chosenSlotIndex+1);
							freeUITree(root);
							/*manage whether to save the file or to load from file based on the value of save*/
							if(!save){
								gameOver = 0;
								loadGameStruct str = loadGameData(chosenSlotIndex+1);
								game = str->playedGame;
								chooseMode();
								setDifficultyByMode();
								Game(str->game_state,str->nextPlayer);
							}
							else{
								saveGameData(board, nextPlayer,chosenSlotIndex+1);
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

/*a screen for user to choose the mode. the mode variable is global, so we just give it a value based on user's selection in this function*/
void chooseMode(){
	clearScreen();
	printf("Entered chooseMode function to choose the game mode\n");
	/*create the root control for window*/
	control root = createControl(window, getRect(0,0,1,1) , NULL, NULL, "window");
	/*create all game options*/
	control title = createControl(SDL_LoadBMP("Textures/Mode/Title.bmp") , getRect(0,0,Width,Height/5),NULL, NULL,"title");
	addControlChild(root,title);
	control pvp = createControl(SDL_LoadBMP("Textures/Mode/Pvp.bmp") , getRect(0,Height/5,Width,Height/5),NULL, NULL,"pvp");
	addControlChild(root,pvp);
	control pvc = createControl(SDL_LoadBMP("Textures/Mode/Pvc.bmp") , getRect(0,2*Height/5,Width,Height/5),pvp, NULL,"pvc");
	addControlChild(root,pvc);
	control cvp = createControl(SDL_LoadBMP("Textures/Mode/Cvp.bmp") , getRect(0,3*Height/5,Width,Height/5),pvc, NULL, "cvp");
	addControlChild(root,cvp);
	control cvc = createControl(SDL_LoadBMP("Textures/Mode/Cvc.bmp") , getRect(0,4*Height/5,Width,Height/5),cvp, pvp,"cvc");
	addControlChild(root,cvc);
	
	
	/*finish connecting*/
	pvp->buttonAbove = cvc;
	pvp->buttonBelow = pvc;
	pvc->buttonBelow = cvp;
	cvp->buttonBelow = cvc;
	
	
	/*draw them to screen.*/
	drawScreen(root, 1);
	control chosenButton = pvp;
	SDL_Event e;
	while(1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case (SDL_MOUSEBUTTONUP):
					/*pressed "pvp"*/
					if(isClicked(pvp,e)){
						freeUITree(root);
						mode = PVP;
						return;
					}
					/*pressed "pvc"*/
					else if(isClicked(pvc,e)){
						freeUITree(root);
						mode = PVC;
						return;
					}
					/*pressed "cvp"*/
					else if(isClicked(cvp,e)){
						freeUITree(root);
						mode = CVP;
						return;
					}
					/*pressed "cvc"*/
					else if(isClicked(cvc,e)){
						freeUITree(root);
						mode = CVC;
						return;
					}
					break;

				case (SDL_KEYDOWN):
					switch(e.key.keysym.sym ){
						/*moving up*/
						case SDLK_UP:
							
							printf("UP moving from %s",chosenButton->name);
							manageButtonColors(chosenButton,1);
							chosenButton = chosenButton->buttonAbove;
							printf("to: %s\n",chosenButton->name);
							break;
						/*moving down*/
						case SDLK_DOWN:
							
							printf("DOWN moving from %s",chosenButton->name);
							manageButtonColors(chosenButton,-1);
							chosenButton = chosenButton->buttonBelow;
							printf("to: %s\n",chosenButton->name);
							break;
						/*set mode to be the marked button*/
						case SDLK_RETURN:
							if (chosenButton == pvp){
								freeUITree(root);
								mode = PVP;
								return;
							}
							else if (chosenButton == pvc){
								freeUITree(root);
								mode = PVC;
								return;
							}
							else if (chosenButton == cvp){
								freeUITree(root);
								mode = CVP;
								return;
							}
							else if(chosenButton == cvc){
								freeUITree(root);
								mode = CVC;
								return;
							}
							break;
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
	


/*show the screens before the main menu.*/
void preGame(){
	/*FIRST SCREEN*/
	control root = createControl(window,getRect(0,0,1,1), NULL, NULL,"window");
	/*create the three buttons and connect between them*/
	control firstScreen = createControl(SDL_LoadBMP("Textures/FirstScreen.bmp") , getRect(0,0,Width,Height/3),NULL, NULL,"first screen");
	addControlChild(root,firstScreen);
	drawScreen(root, 1);
	SDL_Delay(2000);
	freeUITree(root);





	/*SECOND SCREEN*/
	control root2 = createControl(window,getRect(0,0,1,1), NULL, NULL,"window");
	/*create the three buttons and connect between them*/
	control secondScreen = createControl(SDL_LoadBMP("Textures/SecondScreen.bmp") , getRect(0,0,Width,Height/3),NULL, NULL,"second screen");
	addControlChild(root2,secondScreen);
	drawScreen(root2, 1);
	/*wait for user click and then move to screen showing instructions*/
	SDL_Event e;
	while(1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case (SDL_MOUSEBUTTONUP):
					freeUITree(root2);
					showInstructions();
					return;
				default:
					continue;
			}
		}
	}



}

/*shows the screen after exiting program*/
void showClosingScreen(){
	control root = createControl(window,getRect(0,0,1,1), NULL, NULL,"window");
	/*create the three buttons and connect between them*/
	control firstScreen = createControl(SDL_LoadBMP("Textures/Thanks.bmp") , getRect(0,0,Width,Height/3),NULL, NULL,"first screen");
	addControlChild(root,firstScreen);
	drawScreen(root, 1);
	SDL_Delay(1500);

	freeUITree(root);
	return;
}

/*show the screen contining user instructions of how to use the program*/
void showInstructions(){

	control root = createControl(window,getRect(0,0,1,1), NULL, NULL,"window");
	/*create the three buttons and connect between them*/
	control instructions = createControl(SDL_LoadBMP("Textures/Instructions.bmp") , getRect(0,0,Width,Height/3),NULL, NULL,"instructions");
	addControlChild(root,instructions);
	drawScreen(root, 1);
	SDL_Event e;
	/*wait for user to click*/
	while(1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case (SDL_MOUSEBUTTONUP):
					freeUITree(root);
					return;
				default:
					continue;
			}
		}
	}

}