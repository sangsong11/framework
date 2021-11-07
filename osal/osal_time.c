#include "osal_timer.h"

/*定时器结构体信息*/
struct _osal_timer_t_
{
	 char name[OSAL_TIMER_NAME_MAX];    /*名称*/
	 osal_timer_state_t state;          /*状态*/
	 uint32_t  interval;                /*定时器超时时间（tick数）*/
	 uint32_t counter;                  /*定时器剩余时间（tick数）*/
	 osal_timer_timeout_cb_t cb;        /*超时回调函数*/
	 void *arg                          /*回调函数参数*/
};

static OSAL_THREAD_MUTEX_T osal_timer_mutex;
static int osal_timer_running = 0;
static OSAL_THREAD_T osal_timer_thread;
static osal_timer_t osal_timer_table[OSAL_TIMER_MAX];

#define OSAL_TIMER_LOG(timer) (void )(timer)


/*****************************************************************************
Function:osal_timer_dispatch
Description:定时器分发任务
return:条件变量句柄
@arg[in]:函数入参
*****************************************************************************/
static OSAL_THREAD_FUNC osal_timer_dispatch(void *arg)
{
	printf("%s start \n",__func__);
	(void)(arg);
	
	while(osal_timer_running)
	{
		int ret = 0;
		int i =0;
		osal_timer_t *timer = NULL;
		struct timeval tv;
		
		tv.tv_sec = 0;
		tv.tv_usec = OSAL_TIMER_TICK_USEC;
		select(0,NULL,NULL,NULL,&TV);
		
		timer = &osal_timer_table[0];
		for(i = 0; i < OSAL_TIMER_MAX; i++,timer++)
		{
			 OSAL_TIMER_LOG(timer)
			 if(timer->state == OSAL_TIMER_ACTIVE)
			 {
				 OSAL_THREAD_MUTEX_LOCK(osal_timer_mutex,ret);
				 timer->counter--;
				 if(timer->counter <= 0)
				 {
					timer->state = OSAL_TIMER_TIMEOUT;
					OSAL_TIMER_LOG(timer);
					OSAL_THREAD_MUTEX_UNLOCK(osal_timer_mutex,ret);
					timer->cb(timer,timer->arg);
					OSAL_THREAD_MUTEX_LOCK(osal_timer_mutex,ret);
				 }
				 OSAL_THREAD_MUTEX_UNLOCK(osal_timer_mutex,ret);
			 }
		}
	}
	printf("%s exit\n",__func__);
	OSAL_THREAD_RETURN();
}

/*********************************************************************************
Function:osal_timer_init
Description:初始化定时器
Return：成功：0 失败：-1
Param:
   @arg[in]:函数入参
*********************************************************************************/
int osal_timer_init()
{
	int ret = 0;
	int i = 0;
	for(i = 0;i < OSAL_TIMER_MAX;i++)
	{
		osal_timer_table[i] = OSAL_TIMER_IDLE;
	}
	
	OSAL_THREAD_MUTEX_CREATE(osal_timer_mutex,ret);
	if(ret != 0)
	{
		return ret;
	}
	osal_timer_running = 1;
	OSAL_THREAD_START(osal_timer_thread,osal_timer_dispatch,NULL,NULL,ret);
	return ret;
}

/*********************************************************************************
Function:osal_timer_deinit
Description:出初始化定时器
Return：成功：0 失败：-1
Param:
   @arg[in]:函数入参
*********************************************************************************/
int osal_timer_deinit()
{
	int ret = 0;
	OSAL_THREAD_MUTEX_LOCK(osal_timer_mutex,ret);
	osal_timer_running = 0;
	OSAL_THREAD_MUTEX_UNLOCK(osal_timer_mutex,ret);
	
	OSAL_THREAD_JOIN(osal_timer_thread,ret);
}


/*********************************************************************************
Function:osal_timer_create
Description:创建定时器
Return：成功：0 失败：-1
Param:
   @name[in]:名称
   @interval[in]:超时时间
   @cb[in]:超时回调函数
   @arg[in]:回调函数参数
*********************************************************************************/
osal_timer_t * osal_timer_create(const char* name,uint32_t interval,osal_timer_timeout_cb_t cb,void* arg)
{
	int i =0;
	int ret = 0;
	osal_timer_t* timer = NULL;
	osal_timer_t* idle = NULL;
	
	return_val_if_fail(NULL != name,NULL);
	timer = &osal_timer_table[0];
	
	for(i = 0; i < OSAL_TIMER_MAX;i++,timer++)
	{
		if(timer->state == OSAL_TIMER_IDER)
		{
			idle = timer;
			break;
		}
		else if(strncmp(timer->name,name,sizeof(timer->name)) == 0 )
		{
			printf("%s -timer %s is existed\n",__func,name);
			return timer;
		}
	}
	
	if(NULL == idle)
	{
	   printf("%s -timer queue is oveflow\n",__func);
	   return timer;
	}
	
	OSAL_THREAD_MUTEX_LOCK(osal_timer_mutex,ret);
	strncpy(idle->name,name,sizeof(idle->name));
	idle->interval = (interval/OSAL_TIMER_TICK);
	idle->counter = idle->interval;
	idle->cb = cb;
	idle->arg = arg;
	idle->state = OSAL_TIMER_STOP;
	OSAL_THREAD_MUTEX_UNLOCK(osal_timer_mutex,ret);
	
	return idle;
}

/*********************************************************************************
Function:osal_timer_destroy
Description:销毁定时器
Return：成功返回：0，失败返回-1
Param:
   @timer[in]:定时器句柄
*********************************************************************************/
int osal_timer_destroy(osal_timer_t* timer)
{
	int ret = 0;
	OSAL_THREAD_MUTEX_LOCK(osal_timer_mutex,ret);
	timer->state = OSAL_TIMER_IDLE;
	OSAL_THREAD_MUTEX_UNLOCK(osal_timer_mutex,ret);
	
	return;
}

/*********************************************************************************
Function:osal_timer_start
Description:开启定时器
Return：成功返回：0，失败返回-1
Param:
   @timer[in]:定时器句柄
*********************************************************************************/
int osal_timer_start(osal_timer_t* timer)
{
	int ret = 0;
	
	OSAL_THREAD_MUTEX_LOCK(osal_timer_mutex,ret);
	timer->state = OSAL_TIMER_ACTIVE;
	timer->counter = timer->interval;
	OSAL_THREAD_MUTEX_UNLOCK(osal_timer_mutex,ret);
	
	return ret;
}

/*********************************************************************************
Function:osal_timer_restart
Description:重启定时器
Return：成功返回：0，失败返回-1
Param:
   @timer[in]:定时器句柄
*********************************************************************************/
int osal_timer_restart(osal_timer_t* timer)
{
	return osal_timer_start(timer);
}

/*********************************************************************************
Function:osal_timer_stop
Description:停止定时器
Return：成功返回：0，失败返回-1
Param:
   @timer[in]:定时器句柄
*********************************************************************************/
int osal_timer_stop(osal_timer_t* timer)
{
	int ret = 0;
	OSAL_THREAD_MUTEX_LOCK(osal_timer_mutex,ret);
	timer->state = OSAL_TIMER_STOP;
	OSAL_TIMER_LOG(timer);
	OSAL_THREAD_MUTEX_UNLOCK(osal_timer_mutex,ret);
	return ret;
}

/*********************************************************************************
Function:osal_timer_set_interval
Description:设置定时器超时时间
Return：成功返回：0，失败返回-1
Param:
   @timer[in]:定时器句柄
   @interval[in]:超时时间
*********************************************************************************/
int osal_timer_set_interval(osal_timer_t* timer,uint32_t,interval)
{
	int ret = 0;
	
	OSAL_THREAD_MUTEX_LOCK(osal_timer_mutex,ret);
	timer->interval = (interval/OSAL_TIMER_TICK);
	timer->counter =  timer->interval;
	OSAL_THREAD_MUTEX_UNLOCK(osal_timer_mutex,ret);
	
	return ret;
}

/*********************************************************************************
Function:osal_timer_set_cb
Description:设置定时器定时回调函数
Return：成功返回：0，失败返回-1
Param:
   @timer[in]:定时器句柄
   @interval[in]:超时时间
*********************************************************************************/
int osal_timer_set_cb(osal_timer_t* timer,osal_timer_timeout_cb_t cb)
{
	int ret = 0;
	OSAL_THREAD_MUTEX_LOCK(osal_timer_mutex,ret);
	timer->cb = cb;
	OSAL_THREAD_MUTEX_UNLOCK(osal_timer_mutex,ret);
	
	return ret;
}

/*********************************************************************************
Function:osal_time_elapsed_init
Description:开始时间消耗统计
Return：成功返回：0，失败返回-1
Param:
   @t[in]:时间消耗统计句柄
*********************************************************************************/
void osal_time_elapsed_init(osal_time_elapsed_t* t)
{
	(void)t;
}

/*********************************************************************************
Function:osal_time_elapsed_begin
Description:开始时间消耗统计
Return：成功返回：0，失败返回-1
Param:
   @t[in]:时间消耗统计句柄
*********************************************************************************/
void osal_time_elapsed_begin(osal_time_elapsed_t* t)
{
	gettimeofday(&t->begin_time,NULL);
}

/*********************************************************************************
Function:osal_time_elapsed_end
Description:结束时间消耗统计
Return：成功返回：0，失败返回-1
Param:
   @t[in]:时间消耗统计句柄
*********************************************************************************/
void osal_time_elapsed_end(osal_time_elapsed_t* t)
{
	gettimeofday(&t->end_time,NULL);
}

/*********************************************************************************
Function:osal_time_timeval_substract
Description:计算两个时间的时间差
Return：时间x小于时间y返回1 ，否则返回0
Param:
   @result[in]:保存时间差信息
   @x[in]:第一个时间信息
   @y[in]:第二个时间信息
*********************************************************************************/
static int osal_time_timeval_substract(struct timeval* result,struct timeval* x,struct timeval* y)
{
	if(x->tv_usec < y->tv_usec)
	{
		int nsec = (y->tv_usec - x->tv_usec)/1000000 +1;
		y->tv_usec -= 1000000 *nsec;
		y->tv_sec += nsec;
	}
	if(x->tv_usec - y->tv_usec > 1000000)
	{
		int nsec = (x->tv_usec - y->tv_usec)/1000000 +1;
		y->tv_usec += 1000000 *nsec;
		y->tv_sec -= nsec;
	}
	
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;
	
	return x->tv_sec < y->tv_sec;
}
/*********************************************************************************
Function:osal_time_elapsed_calculate
Description:计算耗时时间
Return：成功返回：0，失败返回-1
Param:
   @t[in]:时间消耗统计句柄
*********************************************************************************/
double osal_time_elapsed_calculate(osal_time_elapsed_t* t)
{
	struct imeval result = {0};
	osal_time_timeval_substract(&result,&t->end_time,&t->begin_time);
	
	return (result.tv_sec*1000.0 + result.tv_usec/1000.0);
}
