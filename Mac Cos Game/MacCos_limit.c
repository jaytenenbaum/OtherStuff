/*
 * MacCos_limit.c
 *
 *  Created on: Oct 29, 2013
 *      Author: ofirabramov
 */

#include <stdio.h>

int main(void){
	double x;
	int n=0;
	/*Absolute value of Current Addition in current iteration to the total sum.
	Note that we don't really need to mind Cn's sign, because -0==0. */
	double AbstCurrAdd = 1;

	printf("Enter a double number:\n");
	scanf("%lf", &x);

	while(1){
		/*Every two iterations, the absolute value of the Current Addition multiplies by x^2/((n)*(n+1)), therefore
		every iteration multiplies by x/n. */
		AbstCurrAdd *= x/(n+1);
		/*Check if we are adding 0 in the current iteration. */
		if (AbstCurrAdd == 0){
			break;
		}
		/*To maintain the amount of iterations before adding 0.*/
		n++;
	}

	printf("The optimal order of the Maclaurin polynomial function at x=%f is %d\n", x, n);
return 0;
}
