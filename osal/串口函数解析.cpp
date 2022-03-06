// 串口函数解析.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


/*功能说明：

	给出一个字符串"func(12,4,5)"

	解析出函数名，函数参数，去调用相应的函数

	例如：系统内有一个函数 void func(int,float);

	输入字符串“func(12,4.5)”时，可以正常调用此函数

*/

//定义3个测试函数
void fun1();
void fun2(int v);
void fun3(float v);

void fun4(int v,int v1);
void fun5(float v,float v1);
void fun6(int v,float v1);
void fun7(float v,int v1);

void fun8(int v,int v1,int v2);
void fun9(float v,float v1,float v2);
void fun10(float v,int v1,int v2);

//定义一个结构体保存各种类型参数
typedef struct
{
	char type;	//数据类型	0代表没有数据，1代表int，2代表float
	int itype;	//int 数据
	float ftype;//float数据	

}ARGC_TYPE;

//定义一个结构体保存函数指针和函数名,目前只支持void
typedef struct
{
	void *show;					//函数指针
	const char *func_name;		//函数名

}FUNC_TYPE;

//主要分两大块
char name_len=0;				//保存函数名长度
char argc_len=0;				//保存参数长度
char func_name[21]={0};			//最大支持20个字符的名字
//char func_name;

char argc_list[10][14]={0};		//最大支持10个参数,保存类型 int  float 最长字符串6+6+1+'\0'=14个字节
ARGC_TYPE all_argc[10]={0};		//保存最大10个参数
FUNC_TYPE all_func[]=			//我们也不要搞得这么复杂，先搞几个函数试试水,函数列表可以自行添加
{
	fun1,"fun1()",
	fun2,"fun2(int)",
	fun3,"fun3(float)",

	//2个参数的函数
	fun4,"fun4(int,int)",
	fun5,"fun5(float,float)",
	fun6,"fun6(int,float)",
	fun7,"fun7(float,int)",

	//3个参数的函数
	fun8,"fun8(int,int,int)",
	fun9,"fun9(float,float,float)",
	fun10,"fun10(float,int,int)",
};

//---------------------------------定义所有所需函数-----------------------------

//寻找函数名
char find_function_name(char *p,char *f_name);

//寻找函数参数 
int find_function_para(char *p);

//判断字符串相等
int isstr(char *p1,char *p2);

//字符串长度
int mstrlen(char *p);

//判断字符串保存数据的类型
int strtype(char *p);

//字符串转成int
int str_int(char *p);

//字符串转成float
float str_float(char *p);

//一个数的次方
int chifan(int v,int n);

//处理字符串指令并运行函数
char tran_com(char *p);

//运行函数
char run_func();	

//---------------------------------定义所有所需函数-----------------------------



int _tmain(int argc, _TCHAR* argv[])
{

	printf("BY_HES\n");


	//需处理的字符串命令
	char fun1_name[]="fun1()";
	char fun2_name[]="fun2(1)";
	char fun3_name[]="fun3(12.5)";

	char fun4_name[]="fun4(12,15)";
	char fun5_name[]="fun5(12.5,13.6)";
	char fun6_name[]="fun6(88,12.5)";
	char fun7_name[]="fun7(12.5,99)";

	char fun8_name[]="fun8(12,99,1234)";
	char fun9_name[]="fun9(12.4,99.5,4568.6)";
	char fun10_name[]="fun10(12.6,99,4568)";

	char *p=fun7_name;

	//printf("需要处理的函数指令：%s\n",p);

	//打印所有函数

	printf("函数列表：\n");

	int j=0;
	for(j=0;j<sizeof(all_func)/sizeof(FUNC_TYPE);j++)	//遍历所有函数
	{
		printf("查找函数名：%s\n",all_func[j].func_name);

	}

	while(1)

	{
		char buf[50]={0};

		printf("\n请输入函数：\n");

		scanf("%s",buf);		//类似于单片机得到一个字符串

		tran_com(buf);	//解析命令中的函数名和参数列表
	
		if(!run_func())	//查找函数并且运行
		{
			printf("找不到该函数\n");	
		}
		//判断参数，直接调用函数
	
	}


	
	while(1);

	return 0;
}

//寻找函数名
char find_function_name(char *p,char *f_name)
{
	char *temp=p;
	char i=0;
	char flag=0;
	
	while(*(temp+i))
	{
		//说明函数名大于20个字符了
		if(i>20)
		{
			flag=0;
			break;
		}
		if(temp[i]=='(')
		{
			flag=1;
			break;
		}

		//func_name[i]=temp[i];
		f_name[i]=temp[i];

		i++;
	}

	
	
	if(flag>0)
	{
		//func_name[i]=0;
		f_name[i]=0;

		name_len=i;

		return i;
	}
	//func_name[0]=0;
	f_name[i]=0;
	return 0;
}

//寻找函数参数
int find_function_para(char *p)
{
	
	
	char index=0;
	char i=0 ,j=0;

	//参数找完了
	if(*(p+index)==')')
	{
		return i;
	}

	while(*(p+index))
	{
		//printf("index:%c\n",*(p+index));

		//找到第一个参数
		if(*(p+index)==',')
		{
			argc_list[i][j]=0;
			j=0;
			i++;
			index++;
			continue;
		}
		//参数找完了
		if(*(p+index)==')')
		{
			return i+1;
		}
		argc_list[i][j]=*(p+index);	

		index++;
		j++;
		
	}

	
	return 0;
}

//判断字符串相等
int isstr(char *p1,char *p2)
{
	//如果长度不一样
	if(mstrlen(p1)!=mstrlen(p2))return 0;

	while (*p1&&*p2)
	{
		if(*(p1++)!=*(p2++))return 0;
	}

	return 1;
}

//字符串长度
int mstrlen(char *p)
{
	int c=0;

	while(*(p+c))c++;

	return c;

}

//判断字符串保存数据的类型
int strtype(char *p)
{

	while(*p)
	{
		if(*(p++)=='.')return 1;
	}
	return 0;

}

//字符串转成int
int str_int(char *p)
{
	int value=0;
	char zf=1;			//保存正负数
	char len=0;
	
	int bei_buf[14]={0};	//保存所有倍率

	char i=0;

	len = mstrlen(p);

	if(len==0)return 0;

	//printf("第一个：%c\n",*p);

	if((*p)=='-')	//如果等于负数
	{
		zf=-1;
		p++;
		len=len-1;
		///printf("%c\n",*p);;
	}

	bei_buf[i]=1;
	//倍率 1 10 100 1000...存放
	for(i=1;i<len;i++)
	{
		bei_buf[i]=bei_buf[i-1]*10;
	}
	//for(i=0;i<len;i++)
	//{
	//	printf("%d\n",bei_buf[i]);
	//}

	i=0;

	//printf("len-1:%d\n",bei_buf[len-1]);

	while(*(p+i))
	{
		
		value+=(*(p+i)-'0')*bei_buf[len-i-1];
		//printf("次%d %d\n",i+1,(*(p+i)-'0')*bei_buf[len-i-1]);
		i++;
		if(i==len)return value*zf;
	}



	return 0;
}

//字符串转成float
float str_float(char *p)
{
	char int_buf[2][6]={0};		//保存整数部分 小数部分
	char zf=1;					//标识正数或负数
	int value1=0,value2=0;
	float value=0;
	char len=0;				//数的长度
	char i=0;
	char j=0;
	char k=0;
	len = mstrlen(p);

	if(len==0)return 0;

	if(*(p)=='-')	//如果等于-号
	{
		zf=-1;
		p++;
	}

	//先拆分整数和小数部分
	while(*(p+i))
	{
		//如果碰到小数点
		if(*(p+i)=='.')
		{
			i++;
			j++;
			k=0;
			continue;
		}
		int_buf[j][k]=*(p+i);


		k++;
		i++;
		int_buf[j][k]=0;
	}
	

	//printf("整数部分：%s\n",int_buf[0]);
	//printf("小数部分：%s\n",int_buf[1]);
	//上面处理完后，整数和小数已经分离
	value1=str_int(int_buf[0]);
	//printf("处理后的整数：%d\n",value1);
	value2=str_int(int_buf[1]);
	//printf("处理后的小数：%f\n",(float)value2/(chifan(10,mstrlen(int_buf[1]))));
	value=(value1+(float)value2/(chifan(10,mstrlen(int_buf[1]))))*zf;

	return value;


}

//一个数的次方
int chifan(int v,int n)
{
	int temp=1;

	if(n==0||v==0)return 0;

	while(n--)
	{
		temp*=v;
	}

	return temp;
}

//处理字符串指令并运行函数
char tran_com(char *p)
{
	
	
	name_len=find_function_name(p,func_name);		//寻找函数名，并返回长度

	//找到函数名
	if(name_len)
	{
		printf("找到函数名：%s\n",func_name);
	}

	argc_len=find_function_para((p+name_len+1));	//寻找参数，返回参数个数

	printf("参数个数：%d\n",argc_len);

	//找到所有参数,并解析成可用数据类型
	if(argc_len)	
	{
		char i=0;
		
		for(i=0;i<argc_len;i++)
		{
			//printf("参数%d：%s\n",i+1,argc_list[i]);

				char type=strtype(argc_list[i]);

				if(!type)
				{
					int value=str_int(argc_list[i]);
					
					all_argc[i].type=1;		//保存为int
					all_argc[i].itype=value;
					//printf("int转换后：%d\n",value);
				}else
				{
					float value=str_float(argc_list[i]);
					all_argc[i].type=2;		//保存为int
					all_argc[i].ftype=value;
					//printf("float转换后：%f\n",value);
				}

		}

		return argc_len;

	}
	else
	{
		printf("没有参数\n");
		return 0;
	}

	//----------------------------------由上面已经把所有字符串命令解析出  函数名  参数列表



	return 0;
}

//运行函数
char run_func()
{
	int j=0;
	for(j=0;j<sizeof(all_func)/sizeof(FUNC_TYPE);j++)	//遍历所有函数
	{

		char temp_name[21]={0};
			
		find_function_name((char*)all_func[j].func_name,temp_name);

		//printf("all_func[j]查找函数名：%s\n",temp_name);
	
		if(isstr(func_name,temp_name))	//找到函数名相同的函数
		{
			printf("查找函数名：%s\n",all_func[j].func_name);
			//找到函数后，要先判断函数是否有参数
			if(argc_len==0)
			{
			
				(*(unsigned int(*)())all_func[j].show)();		//调用该函数
				return 1; 
			}
			else	//这里说明有参数
			{
				//有参数的时候呢就分多钟情况了，第一多个参数，第二参数类型不同，最多支持10个参数
				switch (argc_len)
				{
				case 1:		//一个参数的时候只有2种情况，要么int，要么float
					if(all_argc[0].type==1)
					{
					
						(*(unsigned int(*)(int))all_func[j].show)(all_argc[0].itype);		//调用该函数
						
						return 1; 
					}
					if(all_argc[0].type==2)
					{
						
					
						(*(unsigned int(*)(float))all_func[j].show)(all_argc[0].ftype);		//调用该函数
						return 1; 
						
					}
					

					break;
				case 2:		//这里有两个参数，有如下情况：int int ， int float， float float，  float int
						if((all_argc[0].type==1)&&(all_argc[1].type==1))	//如果2个都是int
						{
							
							(*(unsigned int(*)(int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype);		//调用该函数
							return 1; 
						}
						if(all_argc[0].type==2&&all_argc[1].type==2)	//如果2个都是float
						{
							(*(unsigned int(*)(float,float))all_func[j].show)(all_argc[0].ftype,all_argc[1].ftype);		//调用该函数
							return 1; 
						}
						if(all_argc[0].type==1&&all_argc[1].type==2)	//如果是int float
						{
							(*(unsigned int(*)(int,float))all_func[j].show)(all_argc[0].itype,all_argc[1].ftype);		//调用该函数
							return 1; 
						}
						if(all_argc[0].type==2&&all_argc[1].type==1)	//如果是float int
						{
							(*(unsigned int(*)(float,int))all_func[j].show)(all_argc[0].ftype,all_argc[1].itype);		//调用该函数
							return 1; 
						}


					break;
				case 3:		//3个参数就是2^3 种情况
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==1))	//如果3个都是int
						{
							//printf("%d %d %d\n",all_argc[0].itype,all_argc[1].itype,all_argc[2].itype);
							(*(unsigned int(*)(int,int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].itype);		//调用该函数
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==2)&&(all_argc[2].type==2))	//如果3个都是float
						{
							(*(unsigned int(*)(float,float,float))all_func[j].show)(all_argc[0].ftype,all_argc[1].ftype,all_argc[2].ftype);		//调用该函数
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==1)&&(all_argc[2].type==1))	//如果float int int
						{
							(*(unsigned int(*)(float,int,int))all_func[j].show)(all_argc[0].ftype,all_argc[1].itype,all_argc[2].itype);		//调用该函数
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==2)&&(all_argc[2].type==1))	//如果float float int
						{
							(*(unsigned int(*)(float,float,int))all_func[j].show)(all_argc[0].ftype,all_argc[1].ftype,all_argc[2].itype);		//调用该函数
							return 1; 
						}
						if((all_argc[0].type==1)&&(all_argc[1].type==2)&&(all_argc[2].type==2))	//如果int float float
						{
							(*(unsigned int(*)(int,float,float))all_func[j].show)(all_argc[0].itype,all_argc[1].ftype,all_argc[2].ftype);		//调用该函数
							return 1; 
						}
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==2))	//如果int int float
						{
							(*(unsigned int(*)(int,int,float))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].ftype);		//调用该函数
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==1)&&(all_argc[2].type==2))	//如果float int float
						{
							(*(unsigned int(*)(float,int,float))all_func[j].show)(all_argc[0].ftype,all_argc[1].itype,all_argc[2].ftype);		//调用该函数
							return 1; 
						}
						if((all_argc[0].type==1)&&(all_argc[1].type==2)&&(all_argc[2].type==1))	//如果int float int
						{
							(*(unsigned int(*)(int,float,int))all_func[j].show)(all_argc[0].itype,all_argc[1].ftype,all_argc[2].itype);		//调用该函数
							return 1; 
						}
					break;
				case 4:			//这里是4个参数的时候 有2^4种情况
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==1)&&(all_argc[3].type==1))	//如果int int int int
						{
							(*(unsigned int(*)(int,int,int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].itype,all_argc[3].itype);		//调用该函数
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==2)&&(all_argc[2].type==2)&&(all_argc[3].type==2))	//如果float float float float
						{
							(*(unsigned int(*)(float,float,float,float))all_func[j].show)(all_argc[0].ftype,all_argc[1].ftype,all_argc[2].ftype,all_argc[3].ftype);		//调用该函数
							return 1; 
						}


					break;
				case 5:
							//这里是5个参数的时候 有2^5种情况
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==1)&&(all_argc[3].type==1)&&(all_argc[4].type==1))	//如果int int int int int int
						{
							(*(unsigned int(*)(int,int,int,int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].itype,all_argc[3].itype,all_argc[4].itype);		//调用该函数
							return 1; 
						}

					break;
				case 6:
						//这里是6个参数的时候 有2^6种情况
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==1)&&(all_argc[3].type==1)&&(all_argc[4].type==1)&&(all_argc[5].type==1))	//如果int int int int int int
						{
							(*(unsigned int(*)(int,int,int,int,int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].itype,all_argc[3].itype,all_argc[4].itype,all_argc[5].itype);		//调用该函数
							return 1; 
						}
					break;
				case 7:
					break;
				case 8:
					break;
				case 9:
					break;
				case 10:
					break;
				default:
					break;
				}


			}

		}
	}

	return 0;
}

//------------------------测试函数-----------------------------
void fun1()
{

	printf("\n运行函数1无参数\n");

}
void fun2(int v)
{

	printf("\n运行函数2 一个int参数：%d\n",v);

}
void fun3(float v)
{

	printf("\n运行函数3 一个float参数：%f\n",v);

}

void fun4(int v,int v1)
{

	printf("\n运行函数4 两个int参数：%d %d\n",v,v1);
}
void fun5(float v,float v1)
{

	printf("\n运行函数5 两个float参数：%f %f\n",v,v1);
}
void fun6(int v,float v1)
{
	printf("\n运行函数6 一个int 一个float参数：%d %f\n",v,v1);

}
void fun7(float v,int v1)
{

	printf("\n运行函数7 一个float 一个int参数：%f %d\n",v,v1);
}

void fun8(int v,int v1,int v2)
{

	printf("\n运行函数8 三个int参数：%d %d %d\n",v,v1,v2);

}
void fun9(float v,float v1,float v2)
{
	printf("\n运行函数9 三个float参数：%f %f %f\n",v,v1,v2);

}
void fun10(float v,int v1,int v2)
{
	printf("\n运行函数10 两个int参数：%f %d %d\n",v,v1,v2);

}