#ifndef _OSAL_DEFS_H_
#define _OSAL_DEFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <locale.h>

#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <dirent.h>
#include <sys/time.h>
#include <strings.h>


#ifdef __cplusplus
#define OSAL_BEGIN_DECLS EXTERN "C" {
#define OSAL_END_DECLS }
#else
#define OSAL_BEGIN_DECLS
#define OSAL_END_DECLS	
#endif

#define CALL_NEXT(last,op,current) \
    do{ \
	 if(0 == (last)) {(current) = (op);} \
	}while(0)
		
#define OSAL_ADAPT(op,result) ((result = (op)) != 0 ? (errno = result,-1) : 0)
#define OSAL_OSCALL(op,type,failvalue,result) \
    do{ \
	    result = (type)(op); \
	}while(0)
		
#define OSAL_THREAD_FUNC void*
#define OSAL_THREAD_T pthread_t
#define OSAL_THREAD_ID_T pthread_t
#define OSAL_THREAD_RETURN() return (NULL)
#define OSAL_THREAD_START(thread,fn,arg,id,result) \
    do{ \
	   OSAL_THREAD_ID_T* _id=(id); \
	   OSAL_OSCALL(OSAL_ADAPT(pthread_creat(&(thread),NULL,fn,arg),result),int,-1,result); \
       if(0 == result) { if(NULL != id)) {*_id = (thread);}}\
	}while(0)

#define OSAL_THREAD_EXIT() pthread_exit(NULL)
#define OSAL_THREAD_SELF() pthread_self()
#define OSAL_THREAD_EQUAL(thread1,thread2) ( pthread_equal(thread1,thread2) != 0 )
#define OSAL_THREAD_JOIN(thread,result) \
 do{ \
     OSAL_OSCALL(OSAL_ADAPT(pthread_join(thread,NULL),result),int,-1,result); \
 }while(0)   
#define OSAL_THREAD_MUTEX_T pthread_mutex_t
#define OSAL_THREAD_MUTEX_CREATE(mutex,result) \
do {   \
     OSAL_OSCALL(OSAL_ADAPT(pthread_mutex_init(&(mutex),NULL),result),int,-1,result); \
}while(0) 
	
#define OSAL_THREAD_MUTEX_LOCK(mutex,result) \
do{ \
   OSAL_OSCALL(OSAL_ADAPT(pthread_mutex_lock(&(mutex)),result),int,-1,result); \
}while(0)
	
#define OSAL_THREAD_MUTEX_UNLOCK(mutex,result) \
do{ \
   OSAL_OSCALL(OSAL_ADAPT(pthread_mutex_lock(&(mutex)),result),int,-1,result); \
}while(0)
	
#define OSAL_THREAD_MUTEX_DESTROY(mutex,result) \
do{ \
   OSAL_OSCALL(OSAL_ADAPT(pthread_mutex_destroy(&(mutex)),result),int,-1,result); \
}while(0)

#define OSAL_THREAD_SET_NAME(thread,name) pthread_setname_ap((thread),(name))
	
#define OSAL_ASSRT(expr) \
if(!(expr)) {\
   (void)fprint(stderr,\
     "%s:%d:Assertion %s failed in %s", \
	 __FILE__,__LINE__,#expr,__func__); \
    abort(); }
	
#define return_val_if_fail(expr,ret)   \
		if(!expr)  {\
			return ret;\
		}
#endif