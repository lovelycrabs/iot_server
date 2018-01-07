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



#endif // TYPES_H_INCLUDED
