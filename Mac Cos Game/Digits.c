/*
 * Digits.c
 *
 *  Created on: Oct 29, 2013
 *      Author: ofirabramov
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(void){
    
    long x;
    unsigned int i;
    /*The amount of digits of x.*/
    int NumOfDig;
    /*The digit at index i of x. */
    int IIndex;
    
    printf("Enter a long number and a non-negative integer:\n");
    scanf("%ld %u",&x,&i);
    if(x==0){
	NumOfDig=1;
    }
    else{
    	/*The amount of digits in a number, is 1+ floor of its log base 10. */
    	NumOfDig = floor(log10(labs(x)))+1;
    }
    printf("The number of digits in x=%ld is %d\n",x,NumOfDig);
    
    /*The valid indexes are numbers between 1 and the amount of digits.*/
    if(i<1 || i>NumOfDig){
        printf("ERROR: i=%u is an invalid digit number for x=%ld\n",i,x);
    }
    else{
	/*The digit at index i is the remainder of division by 10 of floor(absolute value of the number, divided by 10^ (i-1)). 
	For example, the digit of 12345 at index 3 is:
	12345/100= 123.45->floor(123.45)=123-> 123%10=3*/
        IIndex = ((int)(floor(labs(x)/pow(10,i-1))))%10;
        printf("The digit of x=%ld at index i=%u is %d\n",x,i,IIndex);
    }
    return 0;
}
