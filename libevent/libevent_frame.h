//#ifdef __LIBEVENT_FRAME_H__
//#define __LIBEVENT_FRAME_H__

#include <event2/event.h>
#include <pthread.h>

#include<event.h>
#include "sys/time.h"
struct libevent_thread_t;

typedef void (*event_process)(struct libevent_thread_t *thread,short what,void *arg);
//typedef void (*single_thread_process)(void *data,process_call_item *arg);

typedef struct queue_item_t
{
   void* data;
   struct queue_item_t *next;
}queue_item_s;

typedef struct queue_lock_t
{
   queue_item_s     *head;
   queue_item_s     *tail;
   unsigned int     count;
   unsigned int     max_count;
   pthread_mutex_t     lock;
   pthread_cond_t         cond_v;

}queue_lock_s;


typedef struct libevent_thread_t
{
    pthread_t                thread_id;
    struct event_base        *base;
    struct  event             notify_event;
    int                      notify_receive_fd;
    int                      notify_send_fd;
  
   event_process             process;
   struct  queue_lock_t      *new_conn_queue;
   void(*clean)();
   void *arg;
}libevent_thread;

typedef struct mutex_recursive_t
{
   pthread_mutex_t   lock;
   pthread_mutexattr_t  mattr;
}mutex_recursive;


//typedef void (*single_thread_process)(void *data,process_call_item *arg);
typedef struct condv_mutex_t
{
   pthread_mutex_t   lock;
   pthread_cond_t       cond_v;
}condv_mutex;

typedef struct pararm_process_type_call_t
{
  void *user_data;
  unsigned int  msec_time;
  unsigned int  count ;
  short async;
  condv_mutex wlock;
  struct event  ev_time;
  void* parent;
  short  runflag;
}process_call_item;

typedef struct libevent_thread_lock_t
{
     char tagname[64];
     libevent_thread *evthread;
     int init_count;
     mutex_recursive  init_lock;
     void*  arg;
}libevent_thread_lock;

struct listnode
{
    struct listnode *next;
    struct listnode *prev;
    void            *data;
};
struct utilist
{
      mutex_recursive lock;
      struct listnode  *head;
      struct listnode  *tail;
      unsigned int     count;
      int (*cmp)(void *val,void *val2);
      void (*del)(void *val);
};

typedef struct process_type_call_t
{
     unsigned char using;
     unsigned char type;
     void (*process)(void *data,process_call_item *arg);
     void (*finish)(void *data,process_call_item *arg);
    struct utilist           call_item;
}process_call;
struct thread_param_t
{
     char tagname[64];
     libevent_thread_lock   *evthread_lock;
     libevent_thread        *evthread;
};
//endif
