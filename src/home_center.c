/*
 ============================================================================
 Name        : home_center.c
 Author      : heng
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "globe.h"

#ifdef MSWIN32
int init_net(){
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2,2), &wsaData);

}
int destroy_net(){
	WSACleanup();
}
#endif

int main(int argc,char **args) {
	#ifdef MSWIN32
	if(init_net()){
		printf("WSAStartup fail!\r\n");
		exit(EXIT_FAILURE);

	}
	#endif

	printf("start success!\r\n");



	#ifdef MSWIN32
	destroy_net();
	#endif

	return EXIT_SUCCESS;
}
