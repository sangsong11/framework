#ifndef _OSAL_LIST_H_
#define _OSAL_LIST_H_

#include "osla_defs.h"

OSAL_BEGIN_DECLS

struct _osal_list_t;

typedef struct _osal_list_t_  osal_list_t;

/*链表节点用户数据销毁函数类型*/
typedef void(*osal_list_data_destroy_t)(void* ctx,void* data);

/*链表节点用户数据匹配函数类型*/
typedef int (*osal_list_compare_t)(void*ctx,void*data);

/*链表节点用户数据处理函数类型*/
typedef int (*osal_list_visit_t)(void*ctx,void*data);


osal_list_t* osal_list_create(osal_list_data_destroy_t *data_destory,void* data_destory_ctx);



OSAL_END_DECLS
#endif