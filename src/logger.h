/*
 * logger.h
 *
 *  Created on: Jan 7, 2018
 *      Author: heng
 *
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <stdio.h>
#include <stdlib.h>
#ifdef MULT_THREAD
#include <pthread.h>
#endif

#define LOG_LEVEL_INFO 0x0001
#define LOG_LEVEL_WARN 0x0002
#define LOG_LEVEL_ERR  0x0004
#define LOG_LEVEL_SYS  0x0008
#define LOG_LEVEL_ALL  0xFFFF
#define LOG_LEVEL_SAVE_DEFAULT 0x000C

#define LOG_WRITE(log,lev,format,...) \
    log_printf(log,lev,format,##__VA_ARGS__)

#define LOG_INFO(log,format,...) \
    log_printf(log,0x0001,format,##__VA_ARGS__)

#define LOG_WARN(log,format,...) \
    log_printf(log,0x0002,format,##__VA_ARGS__)

#define LOG_ERROR(log,format,...) \
    log_printf(log,0x0004,format,##__VA_ARGS__)

#define LOG_SYS(log,format,...) \
    log_printf(log,0x0008,format,##__VA_ARGS__)


typedef struct log_t{
    FILE *fs;
    #ifdef MULT_THREAD
    pthread_mutex_t fs_mutex;
    #endif // MULT_THREAD
    char *filename;
    unsigned short print_lev;
    unsigned short save_lev;
} log_printer;

//extern struct log_t *log_writer;

int log_create(struct log_t **log,unsigned short print_lev,unsigned short save_lev,const char *filename);
int log_destroy(struct log_t *log);
int log_printf(struct log_t *log,unsigned short lev,const char *format, ...);

#endif /* LOGGER_H_ */
