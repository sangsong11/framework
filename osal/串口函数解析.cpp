// ���ں�������.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


/*����˵����

	����һ���ַ���"func(12,4,5)"

	������������������������ȥ������Ӧ�ĺ���

	���磺ϵͳ����һ������ void func(int,float);

	�����ַ�����func(12,4.5)��ʱ�������������ô˺���

*/

//����3�����Ժ���
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

//����һ���ṹ�屣��������Ͳ���
typedef struct
{
	char type;	//��������	0����û�����ݣ�1����int��2����float
	int itype;	//int ����
	float ftype;//float����	

}ARGC_TYPE;

//����һ���ṹ�屣�溯��ָ��ͺ�����,Ŀǰֻ֧��void
typedef struct
{
	void *show;					//����ָ��
	const char *func_name;		//������

}FUNC_TYPE;

//��Ҫ�������
char name_len=0;				//���溯��������
char argc_len=0;				//�����������
char func_name[21]={0};			//���֧��20���ַ�������
//char func_name;

char argc_list[10][14]={0};		//���֧��10������,�������� int  float ��ַ���6+6+1+'\0'=14���ֽ�
ARGC_TYPE all_argc[10]={0};		//�������10������
FUNC_TYPE all_func[]=			//����Ҳ��Ҫ�����ô���ӣ��ȸ㼸����������ˮ,�����б�����������
{
	fun1,"fun1()",
	fun2,"fun2(int)",
	fun3,"fun3(float)",

	//2�������ĺ���
	fun4,"fun4(int,int)",
	fun5,"fun5(float,float)",
	fun6,"fun6(int,float)",
	fun7,"fun7(float,int)",

	//3�������ĺ���
	fun8,"fun8(int,int,int)",
	fun9,"fun9(float,float,float)",
	fun10,"fun10(float,int,int)",
};

//---------------------------------�����������躯��-----------------------------

//Ѱ�Һ�����
char find_function_name(char *p,char *f_name);

//Ѱ�Һ������� 
int find_function_para(char *p);

//�ж��ַ������
int isstr(char *p1,char *p2);

//�ַ�������
int mstrlen(char *p);

//�ж��ַ����������ݵ�����
int strtype(char *p);

//�ַ���ת��int
int str_int(char *p);

//�ַ���ת��float
float str_float(char *p);

//һ�����Ĵη�
int chifan(int v,int n);

//�����ַ���ָ����к���
char tran_com(char *p);

//���к���
char run_func();	

//---------------------------------�����������躯��-----------------------------



int _tmain(int argc, _TCHAR* argv[])
{

	printf("BY_HES\n");


	//�账����ַ�������
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

	//printf("��Ҫ����ĺ���ָ�%s\n",p);

	//��ӡ���к���

	printf("�����б�\n");

	int j=0;
	for(j=0;j<sizeof(all_func)/sizeof(FUNC_TYPE);j++)	//�������к���
	{
		printf("���Һ�������%s\n",all_func[j].func_name);

	}

	while(1)

	{
		char buf[50]={0};

		printf("\n�����뺯����\n");

		scanf("%s",buf);		//�����ڵ�Ƭ���õ�һ���ַ���

		tran_com(buf);	//���������еĺ������Ͳ����б�
	
		if(!run_func())	//���Һ�����������
		{
			printf("�Ҳ����ú���\n");	
		}
		//�жϲ�����ֱ�ӵ��ú���
	
	}


	
	while(1);

	return 0;
}

//Ѱ�Һ�����
char find_function_name(char *p,char *f_name)
{
	char *temp=p;
	char i=0;
	char flag=0;
	
	while(*(temp+i))
	{
		//˵������������20���ַ���
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

//Ѱ�Һ�������
int find_function_para(char *p)
{
	
	
	char index=0;
	char i=0 ,j=0;

	//����������
	if(*(p+index)==')')
	{
		return i;
	}

	while(*(p+index))
	{
		//printf("index:%c\n",*(p+index));

		//�ҵ���һ������
		if(*(p+index)==',')
		{
			argc_list[i][j]=0;
			j=0;
			i++;
			index++;
			continue;
		}
		//����������
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

//�ж��ַ������
int isstr(char *p1,char *p2)
{
	//������Ȳ�һ��
	if(mstrlen(p1)!=mstrlen(p2))return 0;

	while (*p1&&*p2)
	{
		if(*(p1++)!=*(p2++))return 0;
	}

	return 1;
}

//�ַ�������
int mstrlen(char *p)
{
	int c=0;

	while(*(p+c))c++;

	return c;

}

//�ж��ַ����������ݵ�����
int strtype(char *p)
{

	while(*p)
	{
		if(*(p++)=='.')return 1;
	}
	return 0;

}

//�ַ���ת��int
int str_int(char *p)
{
	int value=0;
	char zf=1;			//����������
	char len=0;
	
	int bei_buf[14]={0};	//�������б���

	char i=0;

	len = mstrlen(p);

	if(len==0)return 0;

	//printf("��һ����%c\n",*p);

	if((*p)=='-')	//������ڸ���
	{
		zf=-1;
		p++;
		len=len-1;
		///printf("%c\n",*p);;
	}

	bei_buf[i]=1;
	//���� 1 10 100 1000...���
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
		//printf("��%d %d\n",i+1,(*(p+i)-'0')*bei_buf[len-i-1]);
		i++;
		if(i==len)return value*zf;
	}



	return 0;
}

//�ַ���ת��float
float str_float(char *p)
{
	char int_buf[2][6]={0};		//������������ С������
	char zf=1;					//��ʶ��������
	int value1=0,value2=0;
	float value=0;
	char len=0;				//���ĳ���
	char i=0;
	char j=0;
	char k=0;
	len = mstrlen(p);

	if(len==0)return 0;

	if(*(p)=='-')	//�������-��
	{
		zf=-1;
		p++;
	}

	//�Ȳ��������С������
	while(*(p+i))
	{
		//�������С����
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
	

	//printf("�������֣�%s\n",int_buf[0]);
	//printf("С�����֣�%s\n",int_buf[1]);
	//���洦�����������С���Ѿ�����
	value1=str_int(int_buf[0]);
	//printf("������������%d\n",value1);
	value2=str_int(int_buf[1]);
	//printf("������С����%f\n",(float)value2/(chifan(10,mstrlen(int_buf[1]))));
	value=(value1+(float)value2/(chifan(10,mstrlen(int_buf[1]))))*zf;

	return value;


}

//һ�����Ĵη�
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

//�����ַ���ָ����к���
char tran_com(char *p)
{
	
	
	name_len=find_function_name(p,func_name);		//Ѱ�Һ������������س���

	//�ҵ�������
	if(name_len)
	{
		printf("�ҵ���������%s\n",func_name);
	}

	argc_len=find_function_para((p+name_len+1));	//Ѱ�Ҳ��������ز�������

	printf("����������%d\n",argc_len);

	//�ҵ����в���,�������ɿ�����������
	if(argc_len)	
	{
		char i=0;
		
		for(i=0;i<argc_len;i++)
		{
			//printf("����%d��%s\n",i+1,argc_list[i]);

				char type=strtype(argc_list[i]);

				if(!type)
				{
					int value=str_int(argc_list[i]);
					
					all_argc[i].type=1;		//����Ϊint
					all_argc[i].itype=value;
					//printf("intת����%d\n",value);
				}else
				{
					float value=str_float(argc_list[i]);
					all_argc[i].type=2;		//����Ϊint
					all_argc[i].ftype=value;
					//printf("floatת����%f\n",value);
				}

		}

		return argc_len;

	}
	else
	{
		printf("û�в���\n");
		return 0;
	}

	//----------------------------------�������Ѿ��������ַ������������  ������  �����б�



	return 0;
}

//���к���
char run_func()
{
	int j=0;
	for(j=0;j<sizeof(all_func)/sizeof(FUNC_TYPE);j++)	//�������к���
	{

		char temp_name[21]={0};
			
		find_function_name((char*)all_func[j].func_name,temp_name);

		//printf("all_func[j]���Һ�������%s\n",temp_name);
	
		if(isstr(func_name,temp_name))	//�ҵ���������ͬ�ĺ���
		{
			printf("���Һ�������%s\n",all_func[j].func_name);
			//�ҵ�������Ҫ���жϺ����Ƿ��в���
			if(argc_len==0)
			{
			
				(*(unsigned int(*)())all_func[j].show)();		//���øú���
				return 1; 
			}
			else	//����˵���в���
			{
				//�в�����ʱ���ؾͷֶ�������ˣ���һ����������ڶ��������Ͳ�ͬ�����֧��10������
				switch (argc_len)
				{
				case 1:		//һ��������ʱ��ֻ��2�������Ҫôint��Ҫôfloat
					if(all_argc[0].type==1)
					{
					
						(*(unsigned int(*)(int))all_func[j].show)(all_argc[0].itype);		//���øú���
						
						return 1; 
					}
					if(all_argc[0].type==2)
					{
						
					
						(*(unsigned int(*)(float))all_func[j].show)(all_argc[0].ftype);		//���øú���
						return 1; 
						
					}
					

					break;
				case 2:		//���������������������������int int �� int float�� float float��  float int
						if((all_argc[0].type==1)&&(all_argc[1].type==1))	//���2������int
						{
							
							(*(unsigned int(*)(int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype);		//���øú���
							return 1; 
						}
						if(all_argc[0].type==2&&all_argc[1].type==2)	//���2������float
						{
							(*(unsigned int(*)(float,float))all_func[j].show)(all_argc[0].ftype,all_argc[1].ftype);		//���øú���
							return 1; 
						}
						if(all_argc[0].type==1&&all_argc[1].type==2)	//�����int float
						{
							(*(unsigned int(*)(int,float))all_func[j].show)(all_argc[0].itype,all_argc[1].ftype);		//���øú���
							return 1; 
						}
						if(all_argc[0].type==2&&all_argc[1].type==1)	//�����float int
						{
							(*(unsigned int(*)(float,int))all_func[j].show)(all_argc[0].ftype,all_argc[1].itype);		//���øú���
							return 1; 
						}


					break;
				case 3:		//3����������2^3 �����
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==1))	//���3������int
						{
							//printf("%d %d %d\n",all_argc[0].itype,all_argc[1].itype,all_argc[2].itype);
							(*(unsigned int(*)(int,int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].itype);		//���øú���
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==2)&&(all_argc[2].type==2))	//���3������float
						{
							(*(unsigned int(*)(float,float,float))all_func[j].show)(all_argc[0].ftype,all_argc[1].ftype,all_argc[2].ftype);		//���øú���
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==1)&&(all_argc[2].type==1))	//���float int int
						{
							(*(unsigned int(*)(float,int,int))all_func[j].show)(all_argc[0].ftype,all_argc[1].itype,all_argc[2].itype);		//���øú���
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==2)&&(all_argc[2].type==1))	//���float float int
						{
							(*(unsigned int(*)(float,float,int))all_func[j].show)(all_argc[0].ftype,all_argc[1].ftype,all_argc[2].itype);		//���øú���
							return 1; 
						}
						if((all_argc[0].type==1)&&(all_argc[1].type==2)&&(all_argc[2].type==2))	//���int float float
						{
							(*(unsigned int(*)(int,float,float))all_func[j].show)(all_argc[0].itype,all_argc[1].ftype,all_argc[2].ftype);		//���øú���
							return 1; 
						}
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==2))	//���int int float
						{
							(*(unsigned int(*)(int,int,float))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].ftype);		//���øú���
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==1)&&(all_argc[2].type==2))	//���float int float
						{
							(*(unsigned int(*)(float,int,float))all_func[j].show)(all_argc[0].ftype,all_argc[1].itype,all_argc[2].ftype);		//���øú���
							return 1; 
						}
						if((all_argc[0].type==1)&&(all_argc[1].type==2)&&(all_argc[2].type==1))	//���int float int
						{
							(*(unsigned int(*)(int,float,int))all_func[j].show)(all_argc[0].itype,all_argc[1].ftype,all_argc[2].itype);		//���øú���
							return 1; 
						}
					break;
				case 4:			//������4��������ʱ�� ��2^4�����
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==1)&&(all_argc[3].type==1))	//���int int int int
						{
							(*(unsigned int(*)(int,int,int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].itype,all_argc[3].itype);		//���øú���
							return 1; 
						}
						if((all_argc[0].type==2)&&(all_argc[1].type==2)&&(all_argc[2].type==2)&&(all_argc[3].type==2))	//���float float float float
						{
							(*(unsigned int(*)(float,float,float,float))all_func[j].show)(all_argc[0].ftype,all_argc[1].ftype,all_argc[2].ftype,all_argc[3].ftype);		//���øú���
							return 1; 
						}


					break;
				case 5:
							//������5��������ʱ�� ��2^5�����
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==1)&&(all_argc[3].type==1)&&(all_argc[4].type==1))	//���int int int int int int
						{
							(*(unsigned int(*)(int,int,int,int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].itype,all_argc[3].itype,all_argc[4].itype);		//���øú���
							return 1; 
						}

					break;
				case 6:
						//������6��������ʱ�� ��2^6�����
						if((all_argc[0].type==1)&&(all_argc[1].type==1)&&(all_argc[2].type==1)&&(all_argc[3].type==1)&&(all_argc[4].type==1)&&(all_argc[5].type==1))	//���int int int int int int
						{
							(*(unsigned int(*)(int,int,int,int,int,int))all_func[j].show)(all_argc[0].itype,all_argc[1].itype,all_argc[2].itype,all_argc[3].itype,all_argc[4].itype,all_argc[5].itype);		//���øú���
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

//------------------------���Ժ���-----------------------------
void fun1()
{

	printf("\n���к���1�޲���\n");

}
void fun2(int v)
{

	printf("\n���к���2 һ��int������%d\n",v);

}
void fun3(float v)
{

	printf("\n���к���3 һ��float������%f\n",v);

}

void fun4(int v,int v1)
{

	printf("\n���к���4 ����int������%d %d\n",v,v1);
}
void fun5(float v,float v1)
{

	printf("\n���к���5 ����float������%f %f\n",v,v1);
}
void fun6(int v,float v1)
{
	printf("\n���к���6 һ��int һ��float������%d %f\n",v,v1);

}
void fun7(float v,int v1)
{

	printf("\n���к���7 һ��float һ��int������%f %d\n",v,v1);
}

void fun8(int v,int v1,int v2)
{

	printf("\n���к���8 ����int������%d %d %d\n",v,v1,v2);

}
void fun9(float v,float v1,float v2)
{
	printf("\n���к���9 ����float������%f %f %f\n",v,v1,v2);

}
void fun10(float v,int v1,int v2)
{
	printf("\n���к���10 ����int������%f %d %d\n",v,v1,v2);

}