/*
 * configloader.h
 *
 *  Created on: Jan 7, 2018
 *      Author: heng
 */

#ifndef CONFIGLOADER_H_
#define CONFIGLOADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "globe.h"

#define CONF_FILE_PARSE_OK 0
#define CONF_FILE_NOTEXISTS 1
#define CONF_FILE_PARSE_ERROR 2

#define EACH_CONTINUE 0
#define EACH_FOUND 1


#define SENS_PORT_KEY "sens_port"
#define SENS_DEFAULT_PORT 5612

#define SENS_CONNECTION_SIZE_KEY "sens_connection_size"
#define SENS_DEFAULT_CONNECTION_SIZE 32


#define CTRL_PORT_KEY "ctrl_port"
#define CTRL_DEFAULT_PORT 5610

#define CTRL_CONNECTION_SIZE_KEY "ctrl_connection_size"
#define CTRL_DEFAULT_CONNECTION_SIZE 10

#define SYSTEM_LOG_KEY "system_log_file"

#ifdef MSWIN32
#define SYSTEM_DEFUALT_LOG "log/home-center.txt"
#else
#define SYSTEM_DEFUALT_LOG "/var/log/home-center/home-center.log"
#endif // MSWIN32

#define SENS_ADDR_KEY "sens_addr"
#define SENS_DEFAULT_ADDR "0.0.0.0"

#define SENS_RECV_TIMEOUT_KEY "sens_recv_timeout"
#define SENS_DEFAULT_RECV_TIMEOUT 6000

#define SENS_SEND_TIMEOUT_KEY "sens_send_timeout"
#define SENS_DEFAULT_SEND_TIMEOUT 3000

#define SENS_AUTH_TYPE_KEY "sens_auth_type"
#define SENS_DEFAULT_AUTH_TYPE 1

#define CTRL_ADDR_KEY "ctrl_addr"
#define CTRL_DEFAULT_ADDR "0.0.0.0"

#define CTRL_RECV_TIMEOUT_KEY "ctrl_recv_timeout"
#define CTRL_DEFAULT_RECV_TIMEOUT 6000

#define CTRL_SEND_TIMEOUT_KEY "ctrl_send_timeout"
#define CTRL_DEFAULT_SEND_TIMEOUT 3000

#define SYSTEM_SERVER_NAME_KEY "sys_server_name"
#define SYSTEM_DEFUALT_SERVER_NAME "smart home center"

#define CTRL_AUTH_MODE_KEY "ctrl_auth_type"
#define CTRL_DEFUALT_AUTH_MODE 0

#define SYSTEM_DB_FILE_KEY "db_file"
#define SYSTEM_DEFUALT_DB_FILE "home-center.db"


#define CONF_GET_INT(conf,key) \
    atoi(conf_get_value(conf,key))

#define CONF_GET_DOUBLE(conf,key) \
    atof(conf_get_value(conf,key))






struct hash_t{
    char *key;
    char *value;
    struct hash_t *next;

};

typedef struct conf_t{
    struct hash_t *head;
    struct hash_t *end;
    size_t size;

} conf,*pconf;

typedef struct sys_config_t{
    unsigned short sens_port;
    char sens_addr[IP_ADDR_STR_LENGTH];
    unsigned short ctrl_port;
    char ctrl_addr[IP_ADDR_STR_LENGTH];
    size_t sens_connection_size;
    size_t ctrl_connection_size;
    char sys_log_file[FILE_NAME_STR_LENGTH];
    char sys_server_name[NAME_STR_LENGTH];
    char mac_filter;
    unsigned int sens_recv_timeout;
    unsigned int sens_send_timeout;
    short sens_auth_type;
    unsigned int ctrl_recv_timeout;
    unsigned int ctrl_send_timeout;
    short ctrl_auth_type;

    char sys_db_file[FILE_NAME_STR_LENGTH];

} sys_config;

typedef int (*conf_each_func)(struct hash_t *,void *params);


int conf_parse(struct conf_t **conf,const char *filename);
void conf_each(conf_each_func func,struct conf_t *conf,void  *params);
const char *conf_get_value(struct conf_t *conf,const char *key);
int conf_destory(struct conf_t *conf);
int conf_load(struct sys_config_t *conf,const char *filename);

#endif /* CONFIGLOADER_H_ */
