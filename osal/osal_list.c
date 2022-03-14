#include "osal_list.h"

/*链表节点结构*/
typedef struct _osal_list_node_t_
{
   void* data;                               /*用户数据*/
   struct _osal_list_node_t_ *prev;          /*前一个节点*/
   struct _osal_list_node_t_ *next;          /*后一个节点*/     
}osal_list_node_t;

typedef struct _osal_list_t_
{
	osal_list_node_t*  first;                  /*链表中的第一个节点*/
	void *data_destroy_ctx;                    /*链表节点销毁函数入参*/
	osal_list_data_destroy_t data_destroy;     /*链表节点销毁函数*/
};

/********************************************************************************
*Function:osal_list_destroy_data
*Description:销毁链表节点用户数据
*Return：void
*@thiz[in]:链表句柄
*@data[in]:用户数据
********************************************************************************/
static void osal_list_destroy_data(osal_list_t *thiz,void *data)
{
	if(NULL != thiz->data_destroy)
	{
		thiz->data_destroy(thiz->data_destroy_ctx,data);
	}
	return ;
}

/********************************************************************************
*Function:osal_list_destroy_data
*Description:销毁链表节点用户数据
*Return：void
*@thiz[in]:链表句柄
*@data[in]:用户数据
********************************************************************************/
static void osal_list_destroy_data(osal_list_t *thiz,void *data)
{
	if(NULL != thiz->data_destroy)
	{
		thiz->data_destroy(thiz->data_destroy_ctx,data);
	}
	return ;
}

/********************************************************************************
*Function:osal_list_create_node
*Description:创建链表节点
*Return：   链表节点句柄
*@thiz[in]:链表句柄
*@data[in]:用户数据
********************************************************************************/
static osal_list_node_t* osal_list_create_node(osal_list_t* thiz,void* data)
{
	osal_list_node_t* node = osal_calloc(1,sizeof(osal_list_node_t));
	if(NULL != node)
	{
		node->data = data;
		node->prev = NULL;
		node->next = NULL;
	}
	void(thiz);
	return node;
}

/********************************************************************************
*Function:osal_list_create
*Description:创建链表
*Return：   链表句柄
*@data_destroy[in]:链表节点用户数据销毁函数
*@data_destroy_ctx[in]:链表节点用户数据销毁函数入参
********************************************************************************/
osal_list_t* osal_list_create(osal_list_data_destroy_t data_destroy,void* data_destroy_ctx)
{
	osal_list_t* thiz = osal_calloc(1,sizeof(osal_list_t));
	if(NULL != thiz)
	{
		thiz->first = NULL;
		thiz->data_destroy_ctx = data_destroy_ctx;
		thiz->data_destroy = data_destroy;
	}

	return thiz;
}

/********************************************************************************
*Function:osal_list_get_node
*Description:根据链表下标获取链表节点
*Return：   链表节点句柄
*@thiz[in]:链表句柄
*@index[in]:链表下标
*@fail_return_last[in]:是否获取失败返回最后一个节点的标识
********************************************************************************/
static osal_list_node_t* osal_list_get_node(osal_list_t *thiz,unsigned int index,int fail_return_last)
{
	osal_list_node_t* it = NULL;
	
	return_val_if_fail(NULL != thiz,NULL);
	
	it = thiz->first;
	
	while(NULL != it && NULL != it->next && index > 0)
	{
		it = it->next;
		index--;
	}
	if(!fail_return_last)
	{
		it = index > 0 ? NULL: it;
	}
	return it;
}

/********************************************************************************
*Function:osal_list_insert
*Description:在指定的链表下标前插入节点
*Return：   插入成功返回0，失败返回-1
*@thiz[in]:链表句柄
*@index[in]:链表下标
*@data[in]:用户数据
********************************************************************************/
int osal_list_insert(osal_list_t *thiz,unsigned int index,void* data)
{
	osal_list_node_t* it = NULL;
	osal_list_node_t* node = NULL;
	
	return_val_if_fail(NULL != thiz,-1);
	
	node = osal_list_create_node(thiz,data);
	if(NULL == node)
	{
		return -1;
	}
	if(NULL == thiz->first)
	{
		thiz->first = node;
		return 0;
	}
	
	it = osal_list_get_node(thiz,index,-1);
	if(index < osal_list_length(thiz))
	{
		if(it == thiz->first)
		{
			thiz->first = node;
		}
		else
		{
			it->prev->next = node;
			node->prev = it->prev;
		}
		node->next = it;
		it->prev = node;
	}
	else
	{
		it->next = node;
		node->prev = it;
	}
	return 0;
}


/********************************************************************************
*Function:osal_list_insert
*Description:删除链表中胡指定下标的节点
*Return：   插入成功返回0，失败返回-1
*@thiz[in]:链表句柄
*@index[in]:链表下标
********************************************************************************/
int osal_list_delete(osal_list_t *thiz,unsigned int index)
{
    osal_list_node_t* node = NULL;
    return_val_if_fail(NULL != thiz,-1);
	
	node = osal_list_get_node(thiz,index,0);
	if(NULL != node)
	{
		if(node == thiz->first)
		{
			thiz->first = node->next;
		}
		if(NULL != node->next)
		{
			node->next->prev = node->prev;
		}
		if(NULL != node->prev)
		{
			node->prev->next = node->next;
		}
		osal_list_destroy_node(thiz,node);
	}
	
	return 0;
}

/********************************************************************************
*Function:osal_list_length
*Description:获取链表长度
*Return：   链表长度
*@thiz[in]:链表句柄
********************************************************************************/
unsigned int osal_list_length(osal_list_t *thiz)
{
	unsigned int len = 0;
	return_val_if_fail(NULL != thiz,0);
	
	it = thiz->first;
	while(NULL != it)
	{
		len++;
		it = it->next;
	}
	return len;
}

/********************************************************************************
*Function:osal_list_prepend
*Description:链表首部插入节点
*Return：   插入成功返回0，失败返回-1
*@thiz[in]:链表句柄
********************************************************************************/
int osal_list_prepend(osal_list_t *thiz,void* data)
{
	return osal_list_insert(thiz,0,data);
}


/********************************************************************************
*Function:osal_list_get_by_index
*Description:根据链表下标获取用户数据
*Return：   插入成功返回0，失败返回-1
*@thiz[in]:链表句柄
*@index[in]:链表下标
*@data[in]:保存用户数据的缓冲区地址
********************************************************************************/
int osal_list_get_by_index(osal_list_t *thiz,unsigned int index,void* data)
{
	osal_list_node_t* node = NULL;
	
	return_val_if_fail(NULL != thiz && NULL != data ,-1);
	
	node = osal_list_get_node(thiz,index,0);
	if(NULL != node)
	{
		*data = node->data;
		return 0;
	}
	return -1;
}

/********************************************************************************
*Function:osal_list_find
*Description:查找用户数据匹配的链表下标
*Return：   链表下标
*@thiz[in]:链表句柄
*@cmp[in]:用户匹配函数
*@ctx[in]:对比数据
********************************************************************************/
int osal_list_find(osal_list_t *thiz,osal_list_compare_t cmp,void* ctx)
{
	unsigned int index = 0;
    osal_list_node_t* it = NULL;
	
	return_val_if_fail(NULL != thiz && NULL != cmp ,-1);
	
	it = thiz->first;
	while(NULL != it)
	{
		if( cmp(ctx,it->data) == 0 )
		{
			break;
		}
		index++;
		it = it->next;
	}
	return (index >= osal_list_length(thiz))? -1:index;
}

/********************************************************************************
*Function:osal_list_foreach
*Description:查找用户数据匹配的链表下标
*Return：   链表下标
*@thiz[in]:链表句柄
*@visit[in]:任务函数
*@ctx[in]:函数入参
********************************************************************************/
int osal_list_foreach(osal_list_t *thiz,osal_list_visit_t visit,void* ctx)
{
     int ret = 0;
	 osal_list_node_t* it = NULL;
	 
	 return_val_if_fail(NULL != thiz && NULL != visit ,-1);
	 
	 it = thiz->first;
	 while(NULL != it)
	 {
		 ret = visit(ctx,it->data);
		 it = it->next;
	 }
	 return ret;
}


/********************************************************************************
*Function:osal_list_destroy
*Description:销毁链表
*Return：   wu
*@thiz[in]:链表句柄
********************************************************************************/
void osal_list_destroy(osal_list_t *thiz)
{
	osal_list_node_t* it = NULL;
	osal_list_node_t* node = NULL;
	
	if( NULL != thiz )
	{
		it = thiz->first;
		while(NULL != it)
		{
			next = it->next;
			osal_list_destroy_node(thiz,it);
			it = next;
		}
		thiz->first = NULL;
		osal_free(thiz);
	}
	return ;
}
