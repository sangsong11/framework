#ifndef __QQQASKYDEF_H__
#define __QQQASKYDEF_H__

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define BITSET(p,n)  (*(p) |= (1u <<(n)))
#define BITCLR(p,n)  (*(p) &= ~(1u <<(n)))
#define BITGET(i,n)  ((i) & (1u << (n)))

#define SAFE_FREE(p) do{if(p) { free(p);(p) = NULL} }while(0)

#define Fn_apply(type,fn,...){\
                     void *stopper_for_apply = (int[]){0};\
                     type **list_for_apply = (type*[]){__VA_ARGS__,stopper_for_apply};\
                     for(int i = 0;list_for_apply[i] != stopper_for_apply;i++)\
                     fn(list_for_apply[i]);\
                     }
#define Free_all(...)   Fn_apply(void,free,__VA_ARGS__)

#define Sasprintf(write_to,...) {\
                char *tmp_string_for_extend =(write_to);\
                asprintf(&(wrrite_to),__VA_ARGS__);\
                free(tmp_string_for_extend);\
              }
			  
			  
#define LOG_DATA(d, l)\
    do\
{\
    int i;\
    for(i=0;i<l;i++)\
{\
    if((i+1) % 16) \
    printf("%02X ", d[i]);\
        else\
        printf("%02X\n", d[i]);\
}\
    if(i % 16) printf("\n");\
}\
    while(0)
		
#endif