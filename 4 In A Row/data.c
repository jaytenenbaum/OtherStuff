/*


*/
#include "header.h"


/* saves the game data in the right slot, assuming the next player and the given board*/
void saveGameData(board_t board, int nextPlayer, int slot){
	char fileLocation[100];
	sprintf(fileLocation,"Files/%d",slot);
	/*open the relevant file*/
	FILE *f= fopen(fileLocation,"wr");
	/*print the game to file*/
	switch(game){
		case 1:
			fprintf(f,"Tic-tac-toe\n");
			break;
		case 2:
			fprintf(f,"Connect4\n");
			break;
		case 3:
			fprintf(f,"Reversi\n");
			break;
	}
	/*print the next player to file*/
	fprintf(f,"%d\n",nextPlayer);
	/*print the board itself to file*/
	printBoard(f,board);
	fclose(f);
	
	
}
/*save the loaded data, from the file in files with the name "slot", into the returned value of type loadGameStruct
which is a pointer to a struct.*/
loadGameStruct loadGameData(int slot){
	/*buffer for game string to contain*/
	char game_buf[20];
	/*amount of rows and cols*/
	int rows,cols;
	int cur_game;
	int i,j;
	/*this is the returned structure of data with all relevant parameters*/
	loadGameStruct ret = (loadGameStruct) malloc(sizeof(struct loadGame_s));
	char fileLocation[100];
	/*find the relevant file representing slot*/
	sprintf(fileLocation,"Files/%d",slot);
	if(!file_exists(fileLocation)){
		printf("ERROR: The file doesn't exist");
		return NULL;
	}
	FILE *f= fopen(fileLocation,"r");
	
	/*scan game to game_buf*/
	fscanf(f,"%s ",game_buf);
	if(strcmp(game_buf,"Tic-tac-toe")==0){
		cur_game=1;
	}
	else if(strcmp(game_buf,"Connect4")==0){
		cur_game=2;
	}
	else if(strcmp(game_buf,"Reversi")==0){
		cur_game=3;
	}
	(ret->playedGame)= cur_game;
	assert(cur_game==1||cur_game==2||cur_game==3);
	/*set number of rows and cols*/
	switch(cur_game){
		case 1:
			rows=3;
			cols=3;
			break;
		case 2:
			rows=6;
			cols=7;
			break;
		case 3:
			rows=8;
			cols=8;
			break;
	}
	
	/*scan the given file to find the relevant parameters*/
	fscanf(f,"%d ",&(ret->nextPlayer));
	(ret->game_state)=get_blank_board(rows,cols);
	
	/*fill in the new board*/
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fscanf(f,"%d ",&((ret->game_state)[i][j]));
		}
	}
	fclose(f);
	assert(ret->nextPlayer==-1||ret->nextPlayer==1);
	
	
	return ret;
}
/*print board to given file, right after the game name, and next player, using given board.*/
void printBoard(FILE *f,board_t board){

	int rows=0,cols=0;

	switch(game){
		case 1:
			rows=3;cols=3;
			break;
		case 2:
			rows=6;cols=7;
			break;
		case 3:
			rows=8;cols=8;
			break;
	}
	/*iterate to print board to file*/
	int i,j;
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fprintf(f,"%d ",board[i][j]);
		}
		
		fprintf(f,"\n");
	}

}

/* returns a blank board with allocated memory for each row*/
board_t get_blank_board(int rows, int cols) {
	int j;
	/*allocate memory for rows*/
	board_t board = (board_t) malloc(rows*sizeof(int*));
	if (board == NULL ){
		perror("Error: standard function malloc has failed");
		exit(EXIT_FAILURE);
	}
	/*allocate memory for each row*/
	for (j = 0; j < rows; j++){
		board[j] = calloc(cols, sizeof(int));
		if (board[j] == NULL) {
			perror("Error: standard function calloc has failed");
			exit(EXIT_FAILURE);
		}
	}
	return board;
}

/*returns whether the file with name given in filename does indeed exist*/
int file_exists(char *filename){

	FILE *file;
	file = fopen(filename, "r");
    if (file !=NULL)
    {
        fclose(file);
        return 1;
    }
    return 0;
}
/*returns whether the given slot already exists in memory*/
int slotExists(int slot){
	char fileLocation[100];
	sprintf(fileLocation,"Files/%d",slot);
	return file_exists(fileLocation);
}