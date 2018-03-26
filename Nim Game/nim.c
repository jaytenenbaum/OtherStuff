#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void finish(int yourTurn, int* heapSizes);
void playerTurn(int numOfHeaps, int* heapSizes, int heapSizeTmp, int playerHeapTmp);
void removeObject(int* heapSizes, int playerHeapTmp, int heapSizeTmp);
void computerTurn(int numOfHeaps, int* heapSizes);
void computerFirstCase(int numOfHeaps, int* heapSizes);
void computerSecondCase(int numOfHeaps, int* heapSizes);
void printHeaps(int*  heapSizes, int numOfHeaps);
void printKthLine(int* heapSizes, int numOfHeaps, int k);
int maxHeapSize(int* heapSizes, int numOfHeaps);
int gameEnded(int* heapSizes, int numOfHeaps);
void printTurnStat(int* heapSizes, int numOfHeaps, int turnNum);
int calcNimSumOfHeapI(int NimSum, int currentHeapSize);
int calcNimSum(int* heapSizes, int numOfHeaps);
int WinningHeap(int currentNimSum, int currentSize);

int main(){
	// Is 1 when it is user's turn, and -1 when computer's turn
	int yourTurn = -1;
	// The number of heaps in the game
	int numOfHeaps;
	// The size user wants to remove from chosen heap
	int heapSizeTmp;
	// index of chosen heap
	int playerHeapTmp;
	// index which represents the amount of turns, and which is used to set heap sizes.
	int index;







	/**************************************************************************************************
	******************************************* Heaps Setup *******************************************
	***************************************************************************************************/

	printf("Enter the number of heaps:\n");
	scanf("%d", &numOfHeaps);
	if (numOfHeaps<1 || numOfHeaps>32){
		printf("Error: the number of heaps must be between 1 and 32.\n");
		return 1;
	}
	int* heapSizes = (int*)calloc(numOfHeaps, sizeof(int));
	printf("Enter the heap sizes:\n");
	for (index = 0; index < numOfHeaps; index++){
		scanf("%d", &heapSizeTmp);
		if (heapSizeTmp < 1){
			printf("Error: the size of heap %d should be positive.\n", index + 1);
			return 1;
		}
		heapSizes[index] = heapSizeTmp;
	}






	/**************************************************************************************************
	******************************************* Game loop   ***********************************
	***************************************************************************************************/

	index = 1;
	/*Iterate over game loop as long as the game hasn't ended yet*/
	while (gameEnded(heapSizes, numOfHeaps) == 0){
		/*Print Stats for current turn*/

		printTurnStat(heapSizes, numOfHeaps, index);

		if (yourTurn > 0){
			/*********************************************************
			**********************  User's Turn **********************
			**********************************************************/
			/*Get the wanted heap and number of objects to remove*/
			printHeaps(heapSizes, numOfHeaps);
			printf("Your turn: please enter the heap index and the number of removed objects.\n");
			scanf("%d %d", &playerHeapTmp, &heapSizeTmp);
			playerTurn(numOfHeaps, heapSizes, heapSizeTmp, playerHeapTmp);
		}
		else{
			/*********************************************************
			********************    Computer's Turn ******************
			**********************************************************/
			computerTurn(numOfHeaps, heapSizes);
		}


		
		/*change the sign for next turn.*/
		yourTurn *= -1;
		/*update the turn, to be ready for next iteration.*/
		index++;
	}

	/*GAME HAS ENDED*/
	finish(yourTurn, heapSizes);
	return 1;
}










void finish(int yourTurn, int* heapSizes){
	/*Free the used memory and print the winner*/
	free(heapSizes);
	yourTurn *= -1;
	if (yourTurn > 0){
		printf("You win!\n");
	}
	else{
		printf("Computer wins!\n");
	}
}

void playerTurn(int numOfHeaps, int* heapSizes, int heapSizeTmp, int playerHeapTmp){


	/*Checks whether or not the input is valid*/
	while (heapSizeTmp<0 || playerHeapTmp<0 || playerHeapTmp>numOfHeaps || heapSizeTmp>heapSizes[playerHeapTmp - 1]){
		printf("Error: Invalid input.\nPlease enter again the heap index and the number of removed objects.\n");
		scanf("%d %d", &playerHeapTmp, &heapSizeTmp);
	}
	/*deduct objects from relevant heap*/
	removeObject(heapSizes, playerHeapTmp, heapSizeTmp);
}
void removeObject(int* heapSizes, int playerHeapTmp, int heapSizeTmp){
	/*Remove the chosen objects*/
	heapSizes[playerHeapTmp - 1] -= heapSizeTmp;
	printf("You take %d objects from heap %d.\n", heapSizeTmp, playerHeapTmp);
}

void computerTurn(int numOfHeaps, int* heapSizes){
	/*Decides which case of the strategy cases to use*/
	if (calcNimSum(heapSizes, numOfHeaps) == 0){
		computerFirstCase(numOfHeaps, heapSizes);
	}
	else{
		computerSecondCase(numOfHeaps,heapSizes);
	}
}
void computerFirstCase(int numOfHeaps, int* heapSizes){
	/************** Case 1 of strategy *****************/
	/*Find minimum positive sized heap, and reduce on obect from it.*/
	// variable for iterations
	int i;
	for (i = 0; i < numOfHeaps; i++){
		if (heapSizes[i]>0){
			heapSizes[i] -= 1;
			printf("Computer takes 1 objects from heap %d.\n", i + 1);
			return;
		}
	}
}
void computerSecondCase(int numOfHeaps, int* heapSizes){
	/************** Case 2 of strategy *****************/
	/*find a winner heap, and remove needed objects from it*/
	// variable for iterations
	int i;
	// the NimSum of the current heap, when iterating over all heaps
	int currNimSum;
	for (i = 0; i < numOfHeaps; i++){
		currNimSum = calcNimSumOfHeapI(calcNimSum(heapSizes, numOfHeaps), heapSizes[i]);
		if (WinningHeap(currNimSum, heapSizes[i])){
			printf("Computer takes %d objects from heap %d.\n", (heapSizes[i] - currNimSum), i + 1);
			heapSizes[i] = currNimSum;
			break;
		}
	}
}
int WinningHeap(int currentNimSum, int currentSize){
	/*Returns whether the current heap is a winning heap( whether its nimSum is less than its size or not).*/
	return (currentNimSum < currentSize);
}
int calcNimSumOfHeapI(int NimSum, int currentHeapSize){
	/*Calculates the NimSum of current heap, by doing XOR of NimSum and current Heap's size.*/
	return (currentHeapSize) ^ (NimSum);

}

int calcNimSum(int* heapSizes, int numOfHeaps){
	/*Calculates NimSum of all heaps, by iterating over them, and doing Xor on the sizes.*/
	int j;
	int ans = heapSizes[0];
	for (j = 1; j < numOfHeaps; j++){
		ans = (ans)^(heapSizes[j]);
	}
	return ans;
}


void printTurnStat(int* heapSizes, int numOfHeaps, int turnNum){
	/*Prints the textual statistics of current turn.*/
	int i;
	printf("In turn %d heap sizes are:", turnNum);
	for (i = 0; i < numOfHeaps; i++){
		printf(" h%d=%d", i + 1, heapSizes[i]);
	}
	printf(".\n");
}
int gameEnded(int* heapSizes, int numOfHeaps){
	/*Returns if the game has ended. The game has ended iff each heap is of size 0.*/
	int i;
	for (i = 0; i < numOfHeaps; i++){
		if (heapSizes[i] != 0){
			return 0;
		}
	}
	return 1;
}
void printHeaps(int* heapSizes, int numOfHeaps){
	/*Print the graphic representation of the heaps.*/
	int k = maxHeapSize(heapSizes, numOfHeaps);

	while (k > 0){
		k -= 1;
		printKthLine(heapSizes, numOfHeaps, k);
	}
}

void printKthLine(int* heapSizes, int numOfHeaps, int k){
	/*Prints the K'th line in the picture of the heaps. the K'th line has blanks in heaps that have less than
	K elements, and Prints '*' in heaps that have >=K elements.*/
	int i;
	if (heapSizes[0] > k){
		printf("*");
	}
	else{
		printf(" ");
	}

	for (i = 1; i <numOfHeaps; i++){
		printf("\t");
		if (heapSizes[i] > k){
			printf("*");
		}
		else{
			printf(" ");
		}
	}
	printf("\n");
}

int maxHeapSize(int* heapSizes, int numOfHeaps){
	/*Returns the size of the maximum heap*/
	int i;
	int maxHeap = heapSizes[0];
	for (i = 1; i<numOfHeaps; i++){
		if (heapSizes[i]>maxHeap){
			maxHeap = heapSizes[i];
		}
	}
	return maxHeap;
}
