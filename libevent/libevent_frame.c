#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "./libevent_frame.h"
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
typedef void (*single_thread_process)(void *data,process_call_item *arg);
#define getdata(X)   ((X)->data)
#define MAX_PROCESS_TYPE  256
void *event_instance = 0;
int flag_single = 1;
queue_item_s *cq_pop(queue_lock_s *cq)
{
   queue_item_s *item = 0;
   if(!cq) return 0;
    pthread_mutex_lock(&(cq->lock));
    item = cq ->head;
    if(0 != item)
    {
      cq->head = item ->next;
      if(0 == cq->head)
      {
        cq->tail = 0;
      }
       item->next = 0;
       cq->count--;
    }
     pthread_mutex_unlock(&(cq->lock));
     return item;
}

void cq_init(queue_lock_s *cq)
{
  if(!cq)  return;
  pthread_mutex_init(&(cq->lock),NULL);
  pthread_cond_init(&(cq->cond_v),NULL);
  cq->head = 0;
  cq->tail = 0;
  cq->count = 0;
  cq->max_count = 512;
}
void cqi_free(queue_item_s *item)
{ 
   free(item);
}

void cq_destroy(queue_lock_s *cq)
{
   queue_item_s *item = 0;
   if(!cq) return ;
   item = cq_pop(cq);
   while(item)
   {
     cqi_free(item);
     item = cq_pop(cq);
   }
  pthread_mutex_destroy(&(cq->lock));
  pthread_cond_destroy(&(cq->cond_v));  
  free(cq);
}
queue_item_s *cqi_new(void)
{
   queue_item_s *item = (queue_item_s*)malloc(sizeof(queue_item_s));
   memset(item,0,sizeof(queue_item_s));
   return item;
}

int cq_push_simp(queue_lock_s *cq,unsigned char flag,int *fd,void *data,int *cachesize)
{
   int rlt = 0;
   queue_item_s *item = 0;
   if(!cq) return -1;
   printf("if(!cq) return -1-------\n");
   pthread_mutex_lock(&(cq->lock));
   if(cq->count < cq->max_count)
   {
     rlt = write(*fd,&flag,1);
     if( 1 == rlt)
     {
       item = cqi_new();
       item->data = data;
       item->next = 0;
       if(0 == cq->tail)
       {
         cq->head = item;
       }
       else
       {
         cq->tail->next = item; 
       }
       cq->tail = item;
       cq->count++;
       if(cachesize)
       {
          *cachesize = cq->count;
       }
      printf("rlt is %d\n",rlt);
     }
   }
   else
   {
     rlt = -1;    
   }
   pthread_mutex_unlock(&(cq->lock));
   return rlt;
}

static struct listnode *listnode_new()
{
	struct listnode *node;
	node = malloc(sizeof(struct listnode));
	memset(node,0,sizeof(struct listnode));
}
void  utilistnode_add(struct utilist *list,void *val)
{
	struct listnode *node;
	node = listnode_new();
	node->prev = list->tail;
	node->data = val;
	if(list->head == 0)
	    list->head = node;
	else
		list->tail->next = node;
	list->tail = node;
	list->count++;		
}
void utilist_free(struct utilist *l)
{
	if(1)
	{
		pthread_mutex_destroy(&(l->lock.lock));
		pthread_mutexattr_destroy(&(l->lock.mattr));
		free(l);
	}
}
static void listnode_free(struct listnode *node)
{
   free(node);
}  
void utilist_delete_all_node(struct utilist *list)
{
  struct listnode *node;
  struct listnode *next;
  if(!list)
  {
	  return ;
  }	 
  for(node = list ->head;node;node = next)
  {
	  next = node ->next;
	  if(list ->del)
		  (*list->del)(node->data);
	  listnode_free(node);
  }	  
  list->head =list ->tail = 0;
  list->count = 0; 
}
static void event_time_cb(evutil_socket_t fd,short which,void *arg)
{
       
   ;
}

static void single_event_process(struct libevent_thread_t* thread,short what,void* arg)
{
   process_call *pro_call = 0;
   int index = 0;
   int flag_succ = 0;
   process_call_item *call_item = arg;
   pro_call = thread->arg;

   if( 0 == what)
   {
     queue_item_s *queue_item = 0;
     int loop_rlt = 0;
     struct listnode *node = 0;
    
     if(call_item && call_item->async)
     {
        pthread_mutex_lock(&call_item->wlock.lock);
     }

     for(index = 0; index < MAX_PROCESS_TYPE;index++)
     {
       if(!pro_call[index].using)
       {
             continue;
       }
       for(node = pro_call[index].call_item.head;node;(node) = (node)->next)
       {
           process_call_item *call_item = getdata(node);
           process_call *pro_call = call_item->parent;
           event_del(&call_item->ev_time);
           pro_call->finish(call_item->user_data,call_item);
           free(call_item);
       }
       utilist_delete_all_node(&pro_call[index].call_item);
       pro_call[index].using = 0;
     }

     if(thread->clean)
     {
         (thread->clean)();
     }
     loop_rlt = event_base_loopbreak(thread->base);
     close(thread->notify_receive_fd);
     close(thread->notify_send_fd);
     
    queue_item = cq_pop(thread->new_conn_queue);
    while(queue_item)
    {
       free(queue_item);
       queue_item = cq_pop(thread->new_conn_queue);
    }
    cq_destroy(thread->new_conn_queue);
    thread->new_conn_queue = 0;

    if(call_item && call_item->async)
    {
        pthread_mutex_unlock(&call_item->wlock.lock);
        pthread_cond_signal(&call_item->wlock.cond_v);
    }
    printf("single thread process destroy\n");
  } // end if 0 == what
  
  flag_succ = 0;
  for(index = 0;index < 256;index++)
  {
     if(!pro_call[index].using)
     {
        continue;
     }
     if((unsigned char)what == pro_call[index].type)
     {
        process_call_item *cp_call_item = 0;
        printf("type is %d\n",what);
        if(call_item == 0) printf("call_item is NULL\n");
        //if(0 != call_item->msec_time && call_item ->async)
        if(call_item ->async)
        {
          printf("sync call but process\n"); 
        }

       if(call_item ->async)
       {
         pthread_mutex_lock(&call_item->wlock.lock);
       }
       printf("type is %d\n",what);
       pro_call[index].process(call_item->user_data,call_item);
       if(call_item ->async)
       {
            cp_call_item = malloc(sizeof(process_call_item));
            memcpy(cp_call_item,call_item,sizeof(process_call_item));
       }
       else
       {
          cp_call_item = call_item;
       }
       if(call_item ->async)
       {
          pthread_mutex_unlock(&call_item->wlock.lock);
          pthread_cond_signal(&call_item->wlock.cond_v);
       }
       call_item = 0;

       if( 0 == cp_call_item->msec_time)
       {
          if(pro_call[index].finish)
          {
             pro_call[index].finish(cp_call_item->user_data,cp_call_item);
          }
          free(cp_call_item);
          cp_call_item = 0;
       }
       else
       {
          struct timeval tv;
          tv.tv_sec = cp_call_item->msec_time/1000;
          tv.tv_usec = (cp_call_item->msec_time%1000)*1000;

          cp_call_item ->parent = &(pro_call[index]);
          utilistnode_add(&(pro_call[index].call_item),cp_call_item);
          event_set(&cp_call_item->ev_time,-1,0,event_time_cb,cp_call_item);

          if(event_base_set(thread->base,&cp_call_item->ev_time))
          {
            printf("runtime error\n");
          }
          else
          {
             if(event_add(&cp_call_item->ev_time,&tv) == -1)
             {
               printf("runtime error\n");
             }
             else
             {  
                 printf("single thread time\n");
             }
          }
       }
     }
     flag_succ = 1;
     break;
  }

   if(!flag_succ && 0!=what)
   {
      printf("this fun is error\n");
   }
}


static void thread_libevent_process(evutil_socket_t fd,short which,void *arg)
{ 
    libevent_thread *me = arg;
    queue_item_s *item = 0;
    char buf;
    int n = read(fd, &buf, sizeof(buf));
    if(n < 0)
    {
      printf("pipe read error\n");
    }
    if(!me->new_conn_queue)
    {
        printf("can't read from libevent pipe,queue null\n");
    }
    item = cq_pop(me->new_conn_queue);
    if(item && item->data)
    {
      printf("me->process(me,buf,item->data)\n");
      me->process(me,buf,item->data);
    }
    else
    {
       printf("me->process(me,buf,0)\n");
       me->process(me,buf,0);
    }
    cqi_free(item);
    
}

static void setup_work_thread(libevent_thread *me)
{
   me->base = event_base_new();
   if(!me->base)
   {
     printf("can't allocate event base\n");
     exit(1);
   }
   event_set(&me->notify_event,(evutil_socket_t)me->notify_receive_fd,EV_READ|EV_PERSIST,thread_libevent_process,me);
   event_base_set(me->base,&me->notify_event);
   if(event_add(&me->notify_event,0) == -1 )
   {
      printf("can't monitor libevent notify pipe\n");
      exit(1);
   }
   me->new_conn_queue = malloc(sizeof(struct queue_lock_t));
   memset(me->new_conn_queue,0,sizeof(struct queue_lock_t));
   cq_init(me->new_conn_queue); 
}



static void * worker_libevent(void *arg)
{
    int rlt = -1;
    struct thread_param_t *tp = arg;
    libevent_thread *me = tp->evthread;
    pthread_mutex_lock(&(tp->evthread_lock->init_lock.lock));
    tp->evthread_lock->init_count++;
    pthread_mutex_unlock(&(tp->evthread_lock->init_lock.lock));
    rlt = event_base_loop(me->base,0);
   // free(tp);
   // pthread_mutex_destroy(&(tp->evthread_lock->init_lock.lock));
   // pthread_mutexattr_destroy(&(tp->evthread_lock->init_lock.mattr));

    free(tp);
    pthread_exit(NULL);    
}


static void create_work_thread(void* (*func)(void*),void *arg)
{
    pthread_t  thread;
    int ret;
    struct  thread_param_t *tp = arg;
    libevent_thread        *me = 0;
    if(0 != (ret = pthread_create(&thread,0,func,arg)))
    {
        printf("create pthread fail\n");
        exit(1);
    }

    me = tp->evthread;
    me->thread_id = thread;
    sleep(1);

}


void event_thread_init(libevent_thread_lock **evthread,int nthreads,event_process process,const char *tagname)
{
   int i = 0;
   libevent_thread *threads = 0;
   *evthread = (libevent_thread_lock*)malloc(sizeof(libevent_thread_lock));
   memset(*evthread,0,sizeof(libevent_thread_lock));
   (*evthread)->init_count = 0;
   pthread_mutexattr_init(&((*evthread)->init_lock.mattr));
   pthread_mutexattr_settype(&((*evthread)->init_lock.mattr), PTHREAD_MUTEX_RECURSIVE);
   pthread_mutex_init(&((*evthread)->init_lock.lock),&((*evthread)->init_lock.mattr));
//   (*evthread)->evthread
   threads = (libevent_thread*)malloc(nthreads * sizeof(libevent_thread));
   (*evthread)->evthread = threads;

   for(i = 0; i < nthreads; i++)
   {
      int fds[2] = {0};
      pipe(fds);
      threads[i].notify_receive_fd = fds[0];
      threads[i].notify_send_fd  = fds[1];
      threads[i].process = process;
      setup_work_thread(&threads[i]);
   }

   for(i = 0; i < nthreads; i++)
   {
      struct thread_param_t *tp = malloc(sizeof(struct thread_param_t));
      strcpy(tp->tagname,tagname);
      tp->evthread_lock = *evthread;
      tp->evthread = &threads[i];
      create_work_thread(worker_libevent,tp);
   }

}


void create_single_thread(void **instance,void(*clean)(),const char *tagname)
{
   libevent_thread_lock *evthread_single_lock  = 0;
   process_call *pro_call = 0;
   if(*instance)
   {
      printf("run time error,single thread instance been created,%s\n",tagname);
   }
   event_thread_init(&evthread_single_lock,1,single_event_process,tagname);
   strcpy(evthread_single_lock->tagname,tagname);
   
   pro_call = (process_call*)malloc(sizeof(process_call)*200);
   evthread_single_lock->evthread[0].arg = pro_call;
   evthread_single_lock->evthread[0].clean = clean;
   *instance = (void*)evthread_single_lock;
}

void event_thread_destroy(libevent_thread_lock **evthread,int nthreads)
{
   int index = 0;
   for(index = 0 ;index < nthreads;index++)
   {
      event_base_free((*evthread)->evthread[index].base);     
      (*evthread)->evthread[index].base = 0;
      if((*evthread)->evthread[index].new_conn_queue)
      {
        cq_destroy((*evthread)->evthread[index].new_conn_queue);   
      }
   }
   free((*evthread)->evthread);
   pthread_mutex_destroy(&((*evthread)->init_lock.lock));
   pthread_mutexattr_destroy(&((*evthread)->init_lock.mattr));   
   free((*evthread));
   *evthread = 0;
}

void syncrun_single_thread(void *instance,unsigned char type,void *data)
{
   int rlt = 0;
   process_call_item *item_data = 0;
   libevent_thread_lock *evthread_single_lock = 0;
   libevent_thread *evthread = 0;
   if(!instance || MAX_PROCESS_TYPE <= type)
   {
       printf("runtime error,run single sync process fail\n");
   }
   evthread_single_lock=(libevent_thread_lock*)instance;
   evthread = evthread_single_lock->evthread;
   if(!evthread[0].new_conn_queue)
   {
     printf("new_conn_queue---- error\n");
   }
  
    item_data = malloc(sizeof(process_call_item));
    item_data->user_data = data;
    item_data->async =1;
    
    pthread_mutex_init(&(item_data->wlock.lock),NULL);
    pthread_cond_init(&(item_data->wlock.cond_v),NULL);
    pthread_mutex_lock(&(item_data->wlock.lock));
    rlt = cq_push_simp(evthread[0].new_conn_queue,type,&(evthread[0].notify_send_fd),item_data,0);
    if(0!=rlt)
    {
       printf("0!=rlt---------\n");
      pthread_mutex_unlock(&(item_data->wlock.lock));
      pthread_mutex_destroy(&(item_data->wlock.lock));
      pthread_cond_destroy(&(item_data->wlock.cond_v));
      free(data);
      printf("0!=rlt---------\n");
      free(item_data);
      item_data = 0;
    }
    else
    {
      pthread_cond_wait(&(item_data->wlock.cond_v),&(item_data->wlock.lock));
      pthread_mutex_unlock(&(item_data->wlock.lock));
      pthread_mutex_destroy(&(item_data->wlock.lock));
      pthread_cond_destroy(&(item_data->wlock.cond_v));
      free(item_data);
    }
    
}

void asyncrun_single_thread_debug(void *instance,unsigned char type,void*data,int *cachesize)
{
  int rlt = 0;
  process_call_item *item_data = 0;
  libevent_thread_lock *evthread_single_lock = 0;
  libevent_thread *evthread = 0;
  if(!instance || MAX_PROCESS_TYPE <= type)
  {
    printf("runtime error,run single sync process fail\n");
  }
  evthread_single_lock=(libevent_thread_lock*)instance;
  evthread = evthread_single_lock->evthread;
  if(!evthread[0].new_conn_queue)
  {
    printf("new_conn_queue---- error\n");
  }
  item_data = malloc(sizeof(process_call_item));
  item_data->user_data = data;
  rlt = cq_push_simp(evthread[0].new_conn_queue,type,&(evthread[0].notify_send_fd),item_data,0);
  if(0!=rlt)
  {
     free(data);
     free(item_data);
     item_data = 0;
     printf("run single async process fail\n");
  }
}

void regist_single_thread_process(void *instance,unsigned char type,single_thread_process process,single_thread_process finish)
{
   libevent_thread_lock *evthread_single_lock = 0;
   process_call *pro_call = 0;
   int index = 0;
   int flag_succ = 0;
   if(!instance || 0 == type || MAX_PROCESS_TYPE <= type || !process || !finish)
   {
      printf("runtime error,regist single process fail\n");
   }

   evthread_single_lock = (libevent_thread_lock*)instance;
   pro_call = evthread_single_lock->evthread[0].arg;
   flag_succ =0;
   for(index= 0;index < MAX_PROCESS_TYPE;index++)
   {
      if(!pro_call[index].using)
      {
         pro_call[index].type = type;
         pro_call[index].process = process;
         pro_call[index].finish = finish;
         pro_call[index].using = 1;
         flag_succ = 1;
         break;
      }
      if(type == pro_call[index].type)
      {
         printf("runtime error,regist single process error\n");
      }
      
   }
  if(!flag_succ)
  {
    printf("runtime error,regist single process error\n");
  }

}

void destroy_single_thread(void **instance)
{
    libevent_thread_lock *evthread_single_lock = 0;
    libevent_thread      *evthread = 0;
    process_call         *proc_call = 0;
    if(!*instance)
    {
      return;
    }
    evthread_single_lock = (libevent_thread_lock*)*instance;
    evthread = evthread_single_lock->evthread;
    
    syncrun_single_thread(*instance,0,0);

    pthread_join((evthread[0].thread_id),NULL);
    printf("thread process release finish\n");
    proc_call = evthread[0].arg;
    free(proc_call);
    proc_call = 0;
    evthread[0].arg = 0;
    event_thread_destroy(&evthread_single_lock,1);
    *instance = 0;
} 

void test_func_process(void *data,process_call_item *arg)
{
   printf("this test_func_process------\n");
}

void test_func_finish(void *data,process_call_item *arg)
{
    printf("this test_func_finish-----\n");
}
void sigroutine(int dunno)
{
   flag_single = 0;
}
int main(int argc, char **argv)
{
    char * buf;
    signal(SIGINT, sigroutine);
    create_single_thread(&event_instance,0,"eventframetest");
    regist_single_thread_process(event_instance,1,test_func_process,test_func_finish);
    while(flag_single)
    { 
      buf =(char*)malloc(50);
      strcpy(buf,"this test content"); 
      syncrun_single_thread(event_instance,1,buf);
      sleep(10);       
    }
   return 0;   
}
