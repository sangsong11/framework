#ifdef __cplusplus
extern "C"
{
#endif
#include "libserialport"
#ifdef __cplusplus
}
#endif

#include "serilaport.h"
#include "q_log_category.h"

#define CHECK(result,log,free_port_mem,port)                                   \
{                                                                              \
     char *error_message;                                                      \
	 switch(result){                                                           \
	 case SP_ERR_ARG:                                                          \
	       q_log_category_error(log,"serial:Error:Invalid argument.");         \
		   free_port_mem ? sp_free_port(port):0;                               \
		   return result;                                                      \
	 case SP_ERR_ALL:                                                          \
	       error_message = sp_last_error_message();                            \
		   q_log_category_error(log,"serial:Error:Failed:%s.",error_message);  \
		   sp_free_error_message(error_message);                               \
		   free_port_mem ? sp_free_port(port):0;                               \
           return result;                                                      \
     case SP_ERR_SUPP:                                                         \
  	       q_log_category_error(log,"serial:Error:Not supported.");            \
		   free_port_mem ? sp_free_port(port):0;                               \
		   return result;                                                      \
	 case SP_ERR_MEM:                                                          \
	       q_log_category_error(log,"serial:Error:Count not allocate mem.");   \
		   free_port_mem ? sp_free_port(port):0;                               \
		   return result;                                                      \
     case SP_OK:                                                               \
     default:                                                                  \
           break;                                                              \
	 }	                                                                       \
}	

/*串口句柄*/
struct _serial_port_t_
{
	struct sp_port *port;
	q_log_category_t *log;
};	


/*******************************************************************************************************
*Function:qkd_serialport_create
*Description:创建串口句柄
*Return:串口对象指针
*Param:
* @log[in]:日志对象指针
*******************************************************************************************************/
serialport_t* qkd_serialport_create(void* log)
{
	serialport_t* port;
	return_val_if_fail(NULL != log,NULL);
	
	port = (serialport_t*)osal_calloc(sizeof(serialport_t),1);
	if(NULL != port)
	{
		port->log = (q_log_category_t*)log;
	}
	return port;
}

/*******************************************************************************************************
*Function:qkd_serialport_destroy
*Description:销毁串口句柄
*Return:void
*Param:
* @port[in]:串口端口对象指针
*******************************************************************************************************/
void qkd_serialport_destroy(serialport_t *port)
{
	return_if_fail(NULL != port);
	//sp_free_port(port->port);
	free(port);
	return ;
}

/*******************************************************************************************************
*Function:qkd_serialport_open
*Description:打开串口端口
*Return:0 成功 -1：失败
*Param:
* @thiz[in]:串口对象指针
* @port_name[in]:串口端口名
* @baud[in]:波特率
* @stopbits[in]:停止位
*******************************************************************************************************/
int qkd_serialport_open(serialport_t *thiz,const char* port_name,unsigned int baud,int stopbits)
{
	return_val_if_fail(NULL != thiz && NULL != port_name,-1);
	
	CHECK(sp_get_port_by_name(port_name,&thiz->port),thiz->log,FALSE,NULL);
	CHECK(sp_open(thiz->port,SP_MODE_READ_WRITE),thiz->log,TRUE,thiz->port);
	CHECK(sp_set_baudrate(thiz->port,baud),thiz->log,TRUE,thiz->port);
	CHECK(sp_set_bits(thiz->port,8),thiz->log,TRUE,thiz->port);
	CHECK(sp_set_parity(thiz->port,SP_PARITY_NONE),thiz->log,TRUE,thiz->port);
	CHECK(sp_set_stopbits(thiz->port,stopbits),thiz->log,TRUE,thiz->port);
	CHECK(sp_set_flowcontrol(thiz->port,SP_FLOWCONTROL_NONE),thiz->log,TRUE,thiz->port);
	return 0;
}

/*******************************************************************************************************
*Function:qkd_serialport_close
*Description:关闭串口端口，释放资源
*Return:0 成功 -1：失败
*Param:
* @port[in]:串口端口对象指针
*******************************************************************************************************/
int qkd_serialport_close(serialport_t *thiz)
{
   return_val_if_fail(NULL != thiz,-1);
   CHECK(sp_close(thiz->port),thiz->log,FALSE,NULL);
   sp_free_port(thiz->port);
   return 0;
}

/*******************************************************************************************************
*Function:qkd_serialport_readdata
*Description:串口接收数据
*Return:0 成功 -1：失败
*Param:
* @thiz[in]:串口对象指针
* @data[in]:接收数据地址
* @length[in]:接收数据长度
*******************************************************************************************************/
int qkd_serialport_readdata(serialport_t *thiz,unsigned char* data,unsigned int length)
{
	int ret = 0;
	return_val_if_fail(NULL != thiz && NULL != data,-1);
	
	ret = sp_blocking_read(thiz->port,data,length,READ_WRITE_TIMEOUT);
	if(ret == length)
	{
		
	}
	else if(ret >= 0)
	{
		q_log_category_warning(thiz->log,"TIME out %d/%d bytes received",ret,length);
		return -1;
	}
	else
	{
		CHECK(ret,thiz->log,FALSE,NULL);
	}
	return 0;
}

/*******************************************************************************************************
*Function:qkd_serialport_writedata
*Description:串口發送数据
*Return:0 成功 -1：失败
*Param:
* @thiz[in]:串口对象指针
* @data[in]:发送数据地址
* @length[in]:发送数据长度
*******************************************************************************************************/
int qkd_serialport_writedata(serialport_t *thiz,unsigned char* data,unsigned int length)
{
	int ret = 0;
	return_val_if_fail(NULL != thiz && NULL != data,-1);
	
	ret = sp_blocking_write(thiz->port,data,length,READ_WRITE_TIMEOUT);
	if(ret == length)
	{
		
	}
	else if(ret >= 0)
	{
		q_log_category_warning(thiz->log,"TIME out %d/%d bytes sent",ret,length);
		return -1;
	}
	else
	{
		CHECK(ret,thiz->log,FALSE,NULL);
	}
	return 0;
}

/*******************************************************************************************************
*Function:qkd_serialport_readchar
*Description:以非阻塞的方式接收一个字节
*Return:0 成功 -1：失败
*Param:
* @thiz[in]:串口对象指针
* @data[in]:接收数据地址
*******************************************************************************************************/
int qkd_serialport_readchar(serialport_t *thiz,unsigned char* data)
{
	int ret = 0;
	return_val_if_fail(NULL != thiz,-1);
	
	ret = sp_nonblocking_read(thiz->port,data,-1);
	if(ret < 0)
	{
		CHECK(ret,thiz->log,FALSE,NULL);
	}
	
	return (1 == ret? 0: -1);
}
