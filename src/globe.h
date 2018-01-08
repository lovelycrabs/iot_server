#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#define MSWIN32
#else
    #ifdef __WINDOWS_
        #ifndef MSWIN32
        #define MSWIN32
        #endif // OS_WINDOWS
    #endif // __WINDOWS_
#endif


#ifdef __GNUC__
    #ifndef PACKED
        #define PACKED __attribute__ ((packed))
    #endif // PACKED
#endif // __GNUC__



#ifdef MSWIN32
#define CLOSE_SOCK(so) closesocket(so)
#else

#define CLOSE_SOCK(so) close(so)
typedef int SOCKET;

#endif // MSWIN32

#define DEFAULT_CONFIG_FILE "home_center.conf"

#define IP_ADDR_STR_LENGTH 32
#define FILE_NAME_STR_LENGTH 128
#define NAME_STR_LENGTH 32
#define ID_STR_LENGTH 32
#define PASS_KEY_STR_LENGTH 64



#endif // TYPES_H_INCLUDED
