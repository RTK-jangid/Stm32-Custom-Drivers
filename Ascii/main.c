/*
 * main.c
 *
 *  Created on: Jan 7, 2024
 *      Author: ritik
 */


#include<stdio.h>

char a =100;
char* address1 = (char*)&a;
int main(void){
	printf("%p \n",&address1);
	*address1 = 65;
	printf("%c \n",*address1);
	return 0;
}
