/*
 * MacCos.c
 *
 *  Created on: Oct 29, 2013
 *      Author: ofirabramov
 */
#include <stdio.h>
#include <math.h>

int main(void){
    	int N;
	double x;
	double sum=1;
	int i;
	/* The absolute value of the number we are adding or subtracting from sum in each iteration. */
	float CurrentAdd=1;
	int sign;

	printf("Enter a double number and a non-negative integer:\n");
	scanf("%lf %d",&x,&N);


	for(i=1;i<=N;i++){
		/* defining sign adding or subtracting for each iteration*/
		if(i%4==2)
			sign=-1;
		if(i%4==0)
			sign=1;
		if(i%2==1)
			sign=0;
		/* Each two iterations, the CurrentAdd's absolute value multiplies by x^2/(n(n+1)), therefore
		in every iteration we can multiply by x and divide by i. */
		CurrentAdd*=x/i;

		/* adding the i'th number for the sum */
		sum+=sign*CurrentAdd;
	}

	printf("The %d-th order Maclaurin polynomial function at x=%f is %f\n",N,x,sum);

return 0;
}
