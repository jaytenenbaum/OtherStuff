/*
	getScore(board_t board)
	
	#############################################################################################################
	#######	IMPORTANT: ALL FUNCTIONS HERE RETURN THE SCORE OF THE BOARD (-1 is player, 1 is pc)	#################
	#############################################################################################################
*/
#include "header.h"

/*return the score of the given board splitting into subfunctions by game*/
int score(board_t board){
	switch(game){
		case 1:
			return TTT_score(board);
		case 2:
			return connect4_score(board);
		case 3:
			return reversi_score(board);
		default:
			printf("ERROR: no game has been chosen!!!");
			return -1;
	}
		
}

/*calculate score of board in TTT. 0 if nobody wins, and +-100000 if someone wins*/
int TTT_score(board_t board){
	int i;
	/*vertical and horizontal*/
	for (i = 0; i < 3; i++){
		if((board[i][0]== board[i][1]) && (board[i][1]== board[i][2])&& (board[i][1]!=0)){
			return board[i][0]*100000;
		}
		if((board[0][i]== board[1][i]) && (board[1][i]== board[2][i])&& (board[0][i]!=0)){
			return board[0][i]*100000;
		}
	}
	/*diagonals*/
	if((board[0][0]== board[1][1]) && (board[1][1]== board[2][2])&& (board[0][0]!=0)){
		return board[0][0]*100000;
	}
	if((board[2][0]== board[1][1]) && (board[1][1]== board[0][2])&& (board[2][0]!=0)){
		return board[2][0]*100000;
	}
	
	/*if no winner*/
	return 0;
}

/*calculate score of board in CON4. Same function from last H.W.*/
int connect4_score(board_t board){
	/*REMINDER: 1 is for X, 0 is for blank, -1 is for O*/

	int score;
	/*the array with the frequencies of each possible combination in the form [-4,-3,-2,-1,0,1,2,3,4]*/
	int freq[9] = { 0 };
	int i, j;


	/*horizontal combinations*/
	for (i = 0; i < 6; i++){
		for (j = 0; j < 4; j++){
			freq[board[i][j] + board[i][j + 1] + board[i][j + 2] + board[i][j + 3] + 4]++;
		}
	}

	/*vertical combinations*/
	for (i = 0; i < 3; i++){
		for (j = 0; j < 7; j++){
			freq[board[i][j] + board[i + 1][j] + board[i + 2][j] + board[i + 3][j] + 4]++;
		}
	}

	/*diagonal "\" combinations*/
	for (i = 0; i < 3; i++){
		for (j = 0; j < 4; j++){
			freq[board[i][j] + board[i + 1][j + 1] + board[i + 2][j + 2] + board[i + 3][j + 3] + 4]++;
		}
	}

	/*diagonal "/" combinations*/
	for (i = 3; i < 6; i++){
		for (j = 0; j<4; j++){
			freq[board[i][j] + board[i - 1][j + 1] + board[i - 2][j + 2] + board[i - 3][j + 3] + 4]++;
		}
	}

	/*There is a winning combination for X's*/
	if (freq[8]>0){
		
		return 100000;
	}
	/*There is a winning combination for O's*/
	if (freq[0] > 0){
		return -100000;
	}
	/*multiply coefficients, to find score.*/
	score = (-5)*freq[1] + (-2)*freq[2] + (-1)*freq[3] + (1)*freq[5] + (2)*freq[6] + (5)*freq[7];
	return score;
}

/*calculate score of board in reversi, Using the function suggested*/
int reversi_score(board_t board){
	/*returned score*/
	int score;
	int freq[5] = {0,0,0,0,0};
	int i, j;
	int gameOver=1;
	int ones;
	int minusOnes;
	/*check if game is over or not*/
	for (i = 0; i < 8; i++){
		for (j = 0; j < 8; j++){
			if(board[i][j]==1){
				ones++;
				gameOver=0;
			}
			if(board[i][j]==-1){
				minusOnes++;
				gameOver=0;
			}
		}
	}
	/*check the winner if game is over*/
	if(gameOver==1){
		if(ones>minusOnes){
			return 100000;
		}
		else if(ones>minusOnes){
			return -100000;
		}
		else{
		return 0;
		
		}
	}
	

	/*region 1*/
	for (i = 2; i < 6; i++){
		for (j = 2; j < 6; j++){
			freq[0]+=board[i][j];
		}
	}
	/*region 2*/
	for (j = 2; j < 6; j++){
		freq[1]+=board[1][j];
		freq[1]+=board[j][1];
		freq[1]+=board[6][j];
		freq[1]+=board[j][6];
	}

	/*region 3*/
	for (j = 2; j < 6; j++){
		freq[2]+=board[0][j];
		freq[2]+=board[j][0];
		freq[2]+=board[7][j];
		freq[2]+=board[j][7];
	}

	/*region 4*/
	freq[3]+=board[0][1];
	freq[3]+=board[1][1];
	freq[3]+=board[1][0];
	
	freq[3]+=board[6][0];
	freq[3]+=board[6][1];
	freq[3]+=board[7][1];
	
	freq[3]+=board[0][6];
	freq[3]+=board[1][6];
	freq[3]+=board[1][7];
	
	freq[3]+=board[7][6];
	freq[3]+=board[6][6];
	freq[3]+=board[6][7];
	
	/*region 5*/
	freq[4]+=board[0][0];
	freq[4]+=board[0][7];
	freq[4]+=board[7][0];
	freq[4]+=board[7][7];
	
	score = (1)*freq[0] + (-1)*freq[1] + (5)*freq[2] + (-5)*freq[3] + (10)*freq[4];
	return score;
	


}



/*Returns an array of 7*row+col representing the winning line, for the game CON4*/
int *connect4WinningLine(board_t board){
	/*REMINDER: 1 is for X, 0 is for blank, -1 is for O*/
	
	/*The wining line is represnted as an array of 4*row+col*/
	int* winningLine=(int *)calloc(4,sizeof(int));
	int i, j;
	int n;

	/*horizontal combinations*/
	for (i = 0; i < 6; i++){
		for (j = 0; j < 4; j++){
			if ((board[i][j] == board[i][j + 1]) && (board[i][j + 2] == board[i][j + 3]) && (board[i][j + 2] == board[i][j + 1])&& (board[i][j] != 0)){
				for(n=0;n<4;n++){
					winningLine[n] = 7*i+(j+n);
				}
				return winningLine;
			}
		}
	}

	/*vertical combinations*/
	for (i = 0; i < 3; i++){
		for (j = 0; j < 7; j++){
			if ((board[i][j] == board[i + 1][j]) && (board[i + 2][j] == board[i + 3][j]) && (board[i][j] == board[i + 2][j]) && (board[i][j] != 0)){
				for(n=0;n<4;n++){
					winningLine[n] = 7*(i+n)+j;
				}
				return winningLine;
			}
		}
	}

	/*diagonal "\" combinations*/
	for (i = 0; i < 3; i++){
		for (j = 0; j < 4; j++){
			if ((board[i][j] == board[i + 1][j + 1]) && (board[i + 2][j + 2] == board[i + 3][j + 3] ) && (board[i][j] == board[i + 2][j + 2])&& (board[i][j] != 0)){
				for(n=0;n<4;n++){
					winningLine[n] = 7*(i+n)+(j+n);
				}
				return winningLine;
				}
		}
	}

	/*diagonal "/" combinations*/
	for (i = 3; i < 6; i++){
		for (j = 0; j<4; j++){
			if ((board[i][j] == board[i - 1][j + 1]) && (board[i - 2][j + 2] == board[i - 3][j + 3]) && (board[i][j] == board[i - 2][j + 2])&& (board[i][j] != 0)){
				for(n=0;n<4;n++){
					winningLine[n] = 7*(i-n)+(j+n);
				}
				return winningLine;
			}
		}
	}

	return winningLine;
}


/*Returns an array of 3*row+col representing the winning line, for the game TTT*/
int *TTTWinningLine(board_t board){
	printf("enter TTTWinningLine\n");
	/*The wining line is represnted as an array of 3*row+col*/
	int* winningLine=(int *)calloc(3,sizeof(int));
	int i;
	printf("checking vertical and horizontal\n");
	/*vertical and horizontal*/
	for (i = 0; i < 3; i++){
		if ((board[i][0] == board[i][1]) && (board[i][1] == board[i][2]) && (board[i][1] != 0)){
			printf("found horizontal!!\n");
			winningLine[0] = 3*i;
			winningLine[1] =  i*3+ 1;
			winningLine[2] =  i*3+ 2;

		}
		if ((board[0][i] == board[1][i]) && (board[1][i] == board[2][i]) && (board[0][i] != 0)){
			printf("found vertical!!\n");
			winningLine[0] = i;
			winningLine[1] = i+3;
			winningLine[2] = i+6;
		}
	}
	/*diagonals*/
	if ((board[0][0] == board[1][1]) && (board[1][1] == board[2][2]) && (board[0][0] != 0)){
		printf("found diagonal!!\n");
		winningLine[0] = 0;
		winningLine[1] = 4;
		winningLine[2] = 8;

	}
	if ((board[2][0] == board[1][1]) && (board[1][1] == board[0][2]) && (board[2][0] != 0)){
		printf("found diagonal!!\n");
		winningLine[0] = 2;
		winningLine[1] = 4;
		winningLine[2] = 6;
	}

	return winningLine;
}


/*Returns an array representing the winning line*/
int *getWinningLine(board_t board){
	printf("enter getWinningLine with game %d\n",game);
	/*switch functions according to game*/
	if (game == 1){
		return TTTWinningLine(board);
	}
	if (game == 2){
		return connect4WinningLine(board);
	}
	printf("ERROR: Wrong game!!!");
	return NULL;
}
