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
#include <signal.h>
#include "globe.h"
#include "configloader.h"
#include "logger.h"

struct log_t *logger;

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

void main_exit (int retval) {

    log_destroy(logger);

    //sens_close();
    //sens_destroy();
    //sys_destroy();
	#ifdef MSWIN32
	destroy_net();
	#endif

    exit (retval);
}

/* osetreni signalu */
void signal_handler (int signum) {

  main_exit (EXIT_SUCCESS);
}

int main(int argc,char **args) {
    struct sys_config_t sys_cfg;
    char *config_file=NULL;
    if(argc>1){
    	config_file=args[1];
    }
    else{
    	config_file=DEFAULT_CONFIG_FILE;
    }

    if(conf_load(&sys_cfg,config_file)){
        printf("config file '%s' not found !\n",config_file);
        exit (EXIT_FAILURE);
        return -1;
    }


    if(log_create(&logger,LOG_LEVEL_ALL,LOG_LEVEL_SAVE_DEFAULT,sys_cfg.sys_log_file)!=0){
        printf("logfile:[%s] load error! \n",sys_cfg.sys_log_file);
        exit(EXIT_FAILURE);
        return -1;
    }

	#ifdef MSWIN32
	if(init_net()){
		LOG_SYS(logger,"network startup fail");
		exit(EXIT_FAILURE);
		return -1;

	}
	#endif

    /* registrace signal handleru */
    signal (SIGINT, signal_handler);
    signal (SIGTERM, signal_handler);
    #ifndef MSWIN32
    signal (SIGHUP, signal_handler);
    signal (SIGPIPE, SIG_IGN);
    #endif // MSWIN32

	LOG_SYS(logger,"application startup success!");



	main_exit(EXIT_SUCCESS);


	return EXIT_SUCCESS;
}
