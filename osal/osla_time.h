#ifdef  _OSAL_TIME_H_
#define _OSAL_TIME_H_
#include "osla_def.h"

#define OSAL_TIMER_NAME_MAX 256 /*定时器名称最大长度*/
#define OSAL_TIMER_MAX      128
#define OSAL_TIMER_TICK 100      /*100 MILLISECONDS*/
#define OSAL_TIMER_TICK_USEC (OSAL_TIMER_TICK * 1000)

typedef enum _osal_timer_state_t_
{
	OSAL_TIMER_IDLE = 0x01,    /*空闲*/
	OSAL_TIMER_STOP = 0X02,
	OSAL_TIMER_ACTIVE = 0X04,
	OSAL_TIMER_TIMEOUT = 0X08,
	OSAL_TIMER_UNKNOW = 0X10
}osal_timer_state_t;

typedef enum _osal_timer_elapsed_t_
{
	struct timeval begin_time;
	struct timeval end_time;
}osal_time_elapsed_t;

struct _osal_timer_t_;
typedef struct _osal_timer_t_  osal_timer_t;

/*定时器超时回调函数类型*/
typedef void (*osal_timer_timeout_cb_t)(osal_timer_t* timer,void *arg);

OSAL_BEGIN_DECLS

/***********************************************************************************************
FUCTION:osal_timer_init
DEscription：初始化定时器
Return:void
Param:
@arg[in]函数入参
************************************************************************************************/
int osal_timer_init();

/***********************************************************************************************
FUCTION:osal_timer_deinit
DEscription：初始化定时器
Return:void
Param:
@arg[in]函数入参
************************************************************************************************/
void osal_timer_deinit();

/***********************************************************************************************
FUCTION:osal_timer_create
DEscription：创建定时器
Return:定时器句柄
Param:
@name[in]：名称
@interval[in]：超时时间
@cb[in]:超时时间回调函数
@arg[in]:回调函数入参
************************************************************************************************/
osal_timer_t* osal_timer_create(const char* name, uint32_t interval,osal_timer_timeout_cb_t cb,void *arg);

/***********************************************************************************************
FUCTION:osal_timer_destroy
DEscription：销毁定时器
Return:void
Param:
@arg[in]函数入参
************************************************************************************************/
void osal_timer_destroy(osal_timer_t *timer);

/***********************************************************************************************
FUCTION:osal_timer_start
DEscription：开始定时器
Return:void
Param:
@timer[in]定时器句柄
************************************************************************************************/
int osal_timer_start(osal_timer_t* timer);

/***********************************************************************************************
FUCTION:osal_timer_restart
DEscription：重启定时器
Return:void
Param:
@timer[in]定时器句柄
************************************************************************************************/
int osal_timer_restart(osal_timer_t* timer);

/***********************************************************************************************
FUCTION:osal_timer_stop
DEscription：停止定时器
Return:void
Param:
@timer[in]定时器句柄
************************************************************************************************/
int osal_timer_stop(osal_timer_t* timer);

/***********************************************************************************************
FUCTION:osal_timer_set_interval
DEscription:设置定时器超时时间
Return:成功 0 失败返回-1
Param:
@timer[in]定时器句柄
@interval[in]:超时时间
************************************************************************************************/
int osal_timer_set_interval(osal_timer_t* timer,uint32_t interval);

/***********************************************************************************************
FUCTION:oasl_timer_set_cb
DEscription:设置定时器回调函数
Return:成功 0 失败返回-1
Param:
@timer[in]定时器句柄
@cb[in]：回调函数
************************************************************************************************/
int oasl_timer_set_cb(osal_timer_t*timer,osal_timer_timeout_cb_t cb);

/***********************************************************************************************
FUCTION:osal_time_elapsed_init
DEscription:初始化时间消耗统计
Return:void
Param:
@t[in]:时间统计消耗句柄
************************************************************************************************/
void osal_time_elapsed_init(osal_time_elapsed_t* t);

/***********************************************************************************************
FUCTION:osal_time_elapsed_begin
DEscription:开始时间消耗统计
Return:void
Param:
@t[in]:时间统计消耗句柄
************************************************************************************************/
void osal_time_elapsed_begin(osal_time_elapsed_t* t);

/***********************************************************************************************
FUCTION:osal_time_elapsed_end
DEscription:结束时间消耗统计
Return:void
Param:
@t[in]:时间统计消耗句柄
************************************************************************************************/
void osal_time_elapsed_end(osal_time_elapsed_t* t);

/***********************************************************************************************
FUCTION:osal_time_elapsed_end
DEscription:计算耗时时间
Return:void
Param:
@t[in]:时间统计消耗句柄
************************************************************************************************/
double oasl_time_elapsed_calculate(osal_time_elapsed_t* t);

OSAL_END_DECLS
#endif
