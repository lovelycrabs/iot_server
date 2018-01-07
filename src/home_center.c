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
#include "configloader.h"

#ifdef MSWIN32
/*
 * Description:init windows network
 */
int init_net(){
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2,2), &wsaData);

}
int destroy_net(){
	WSACleanup();
}
#endif

int main(int argc,char **args) {
    struct conf_t *confhd=NULL;
    int c_code=0;
    if(argc>1){
        c_code = conf_parse(&confhd,args[1]);
    }
    else{
        c_code = conf_parse(&confhd,DEFAULT_CONFIG_FILE);
    }

    if(c_code!=0){
        printf("config file '%s' not found !\n",DEFAULT_CONFIG_FILE);
        exit (EXIT_FAILURE);
        return -1;
    }
	#ifdef MSWIN32
	if(init_net()){
		printf("WSAStartup fail!\r\n");
		exit(EXIT_FAILURE);
		return -1;

	}
	#endif

	printf("start success!\r\n");



	#ifdef MSWIN32
	destroy_net();
	#endif

	return EXIT_SUCCESS;
}
