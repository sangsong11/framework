#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <sys/types.h>    
#include <sys/stat.h> 
#include <fcntl.h>
#include <signal.h>    //信号相关头文件 
#include <errno.h>     //errno
#include <sys/wait.h>  //waitpid

#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>

typedef pid_t       ngx_pid_t;
ngx_pid_t           ngx_pid;

#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#define NGX_FILE_RDONLY           O_RDONLY
#define NGX_FILE_WRONLY           O_WRONLY
#define NGX_FILE_RDWR             O_RDWR
#define NGX_FILE_CREATE_OR_OPEN   O_CREAT
#define NGX_FILE_OPEN             0
#define NGX_FILE_TRUNCATE        (O_CREAT|O_TRUNC)
#define NGX_FILE_APPEND          (O_WRONLY|O_APPEND)
#define NGX_FILE_NONBLOCK         O_NONBLOCK
#define ngx_getpid   getpid
#define ngx_open_file(name, mode, create, access)                            \
    open(name, mode|create, access)
	
#define ngx_rename_file(o, n)    rename((const char *) o, (const char *) n)	
#define NGX_FILE_DEFAULT_ACCESS  0644 //-rw-r--r--  所有者有读写权限 组群有读写权限，其他人只有读权限

#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6

#define MAXLINE 80
#define NGX_PROCESS_MASTER     0  //master进程，管理进程
#define NGX_PROCESS_WORKER     1  //worker进程，工作进程

#define SERV_PORT 9000

sig_atomic_t  ngx_reap; 
sig_atomic_t  ngx_change_binary ; //平滑升级到新版本的Nginx程序，热升级
sig_atomic_t  ngx_quit_flag;      //旧进程退出标志位
extern char **environ;
size_t  g_argvneedmem=0;        //保存下这些argv参数所需要的内存大小
size_t  g_envneedmem=0;         //环境变量所占内存大小

int     ngx_process;            //进程类型，比如master,worker进程等

int     g_os_argc;              //参数个数 
char    **g_os_argv;            //原始命令行参数数组,在main中会被赋值

//变量声明
static u_char  master_process[] = "master process";


struct ngx_file_s {
  int           fd;            //文件句柄描述符
  char          *name;         //文件名称
  struct stat   info;          //文件大小资源等信息
  off_t         offset;        //文件的何处
  off_t         sys_offset;
};

//一个信号有关的结构 ngx_signal_t
typedef struct 
{
    int           signo;       //信号对应的数字编号 ，每个信号都有对应的#define ，大家已经学过了 
    const  char   *signame;    //信号对应的中文名字 ，比如SIGHUP 

    //信号处理函数,这个函数由我们自己来提供，但是它的参数和返回值是固定的【操作系统就这样要求】,大家写的时候就先这么写，也不用思考这么多；
    void  (*handler)(int signo, siginfo_t *siginfo, void *ucontext); //函数指针,   siginfo_t:系统定义的结构
} ngx_signal_t;


static void ngx_signal_handler(int signo, siginfo_t *siginfo, void *ucontext); 
typedef struct ngx_file_s        ngx_file_t;

ngx_signal_t  signals[] = {
    // signo      signame             handler
    { SIGHUP,    "SIGHUP",           ngx_signal_handler },        //终端断开信号，对于守护进程常用于reload重载配置文件通知--标识1
    { SIGINT,    "SIGINT",           ngx_signal_handler },        //标识2   
	{ SIGTERM,   "SIGTERM",          ngx_signal_handler },        //标识15
    { SIGCHLD,   "SIGCHLD",          ngx_signal_handler },        //子进程退出时，父进程会收到这个信号--标识17
    { SIGQUIT,   "SIGQUIT",          ngx_signal_handler },        //标识3
    { SIGUSR2,   "SIGUSR2",          ngx_signal_handler },
    { SIGIO,     "SIGIO",            ngx_signal_handler },        //指示一个异步I/O事件【通用异步I/O信号】
    { SIGSYS,    "SIGSYS, SIG_IGN",  NULL               },        //我们想忽略这个信号，SIGSYS表示收到了一个无效系统调用，如果我们不忽略，进程会被操作系统杀死，--标识31
                                                                  //所以我们把handler设置为NULL，代表 我要求忽略这个信号，请求操作系统不要执行缺省的该信号处理动作（杀掉我）
    //...日后根据需要再继续增加
    { 0,         NULL,               NULL               }         //信号对应的数字至少是1，所以可以用0作为一个特殊标记
};


ssize_t
ngx_write_file(ngx_file_t *file, char *buf, size_t size, off_t offset)
{
	ssize_t  n, written;
	written = 0;
	
	if (file->sys_offset != offset) {
        if (lseek(file->fd, offset, SEEK_SET) == -1) {
            return NGX_ERROR;
        }

        file->sys_offset = offset;
    }

    for ( ;; ) {
        n = write(file->fd, buf + written, size);

        if (n == -1) {
            return NGX_ERROR;
        }

        file->offset += n;
        written += n;

        if ((size_t) n == size) {
            return written;
        }

        size -= n;
    }	
}

int ngx_create_pidfile(char* name)
{
    ngx_file_t  file;
    ngx_memzero(&file, sizeof(ngx_file_t));
    file.name = name;
	
    file.fd = ngx_open_file((const char*)file.name, NGX_FILE_RDWR,
                            NGX_FILE_TRUNCATE, NGX_FILE_DEFAULT_ACCESS);
   
    char pid[20] = {0};
    ngx_pid = ngx_getpid();
    sprintf(pid,"%d",ngx_pid);
	
    if (ngx_write_file(&file, pid, strlen(pid), 0) == NGX_ERROR) {
        return NGX_ERROR;
    }
	
    return NGX_OK;
}

static void ngx_signal_handler(int signo, siginfo_t *siginfo, void *ucontext)
{    
    //printf("来信号了\n");    
    ngx_signal_t    *sig;    //自定义结构
    char            *action; //一个字符串，用于记录一个动作字符串以往日志文件中写
    
    for (sig = signals; sig->signo != 0; sig++) //遍历信号数组    
    {         
        //找到对应信号，即可处理
        if (sig->signo == signo) 
        { 
            break;
        }
    } //end for

    action = (char *)"";  //目前还没有什么动作；

    if(ngx_process == NGX_PROCESS_MASTER)      //master进程，管理进程，处理的信号一般会比较多 
    {
        //master进程的往这里走
        switch (signo)
        {
            case SIGCHLD:  //一般子进程退出会收到该信号
                 ngx_reap = 1;  //标记子进程状态变化，日后master主进程的for(;;)循环中可能会用到这个变量【比如重新产生一个子进程】
                 break;
                //.....其他信号处理以后待增加
            case SIGUSR2:
			     ngx_change_binary = 1;
				 break;
		    case SIGQUIT:
			     ngx_quit_flag = 1;
				 printf("SIGQUIT------\n");
				 break;
            default:
                break;
        } //end switch
    }
    else if(ngx_process == NGX_PROCESS_WORKER) //worker进程，具体干活的进程，处理的信号相对比较少
    {
        //worker进程的往这里走
        //......以后再增加
        //....
    }
    else
    {
        //非master非worker进程，先啥也不干
        //do nothing
    } //end if(ngx_process == NGX_PROCESS_MASTER)

    //这里记录一些日志信息
    //siginfo这个
    if(siginfo && siginfo->si_pid)  //si_pid = sending process ID【发送该信号的进程id】
    {
        //ngx_log_error_core(NGX_LOG_NOTICE,0,"signal %d (%s) received from %P%s", signo, sig->signame, siginfo->si_pid, action); 
    }
    else
    {
        //ngx_log_error_core(NGX_LOG_NOTICE,0,"signal %d (%s) received %s",signo, sig->signame, action);//没有发送该信号的进程id，所以不显示发送该信号的进程id
    }

    //.......其他需要扩展的将来再处理；

    //子进程状态有变化，通常是意外退出【既然官方是在这里处理，我们也学习官方在这里处理】
    if (signo == SIGCHLD) 
    {
        //ngx_process_get_status(); //获取子进程的结束状态
    } //end if

    return;
}


int ngx_init_signals()
{
    ngx_signal_t      *sig;  
    struct sigaction   sa;   

    for (sig = signals; sig->signo != 0; sig++) 
    {        
        memset(&sa,0,sizeof(struct sigaction));

        if (sig->handler)  
        {
            sa.sa_sigaction = sig->handler; 
            sa.sa_flags = SA_SIGINFO;       
        }
        else
        {
            sa.sa_handler = SIG_IGN;                                        
        } //end if

        sigemptyset(&sa.sa_mask);   
        //设置信号处理动作(信号处理函数)，说白了这里就是让这个信号来了后调用我的处理程序，有个老的同类函数叫signal，不过signal这个函数被认为是不可靠信号语义，不建议使用，大家统一用sigaction
        if (sigaction(sig->signo, &sa, NULL) == -1) //参数1：要操作的信号
                                                     //参数2：主要就是那个信号处理函数以及执行信号处理函数时候要屏蔽的信号等等内容
                                                      //参数3：返回以往的对信号的处理方式【跟sigprocmask()函数边的第三个参数是的】，跟参数2同一个类型，我们这里不需要这个东西，所以直接设置为NULL；
        {   
            //ngx_log_error_core(NGX_LOG_EMERG,errno,"sigaction(%s) failed",sig->signame); //显示到日志文件中去的 
            return -1; //有失败就直接返回
        }	
        else
        {            
            //ngx_log_error_core(NGX_LOG_EMERG,errno,"sigaction(%s) succed!",sig->signame);     //成功不用写日志 
            //ngx_log_stderr(0,"sigaction(%s) succed!",sig->signame); //直接往屏幕上打印看看 ，不需要时可以去掉
        }
    } //end for
    return 0; //成功    
}

int ngx_setproctitle()
{
	size_t size;
    int    i;
    size = sizeof(master_process);  //注意我这里用的是sizeof，所以字符串末尾的\0是被计算进来了的
    size += g_argvneedmem;          //argv参数长度加进来  
    if(size < 1000) //长度小于这个，我才设置标题
    {
        char title[1000] = {0};
        strcpy(title,(const char *)master_process); //"master process"
        strcat(title," ");  //跟一个空格分开一些，清晰    //"master process "
        for (i = 0; i < g_os_argc; i++)         //"master process ./nginx"
        {
            strcat(title,g_os_argv[i]);
        }
        size_t ititlelen = strlen(title); 
		size_t esy = g_argvneedmem + g_envneedmem;
        if( esy <= ititlelen)
		{
			return 1;
		}
		g_os_argv[1] = NULL;  
		char *ptmp = g_os_argv[0];
		strcpy(ptmp,title);
		ptmp += ititlelen;
		
		size_t cha = esy - ititlelen;
		memset(ptmp,0,cha);
    }    	
	return 0;
}

static int ret_net_listen_fd(void)
{
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
 
    int listenfd, connfd;
 
    char buf[MAXLINE];
    //ipv4的地址长度
    char str[INET_ADDRSTRLEN];
 
    int i, n;
 
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
 
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 20);


    return listenfd;
}


static int ngx_exec_new_binary()
{
	int fd ,i;
	pid_t  pid;
	ngx_rename_file("./nginx.pid","./nginx.pid.oldbin");
	fd = ret_net_listen_fd();
	
	//增加环境变量
	/*char *new_env = (char*)malloc(25);
	memset(new_env,0,25);
	sprintf(new_env,"nginx=%d",fd);
	for(i = 0; environ[i]; i++) 
    {
	   printf("%s --\n",environ[i]);
    } 
	environ[i] = new_env;*/
	
	//创建进程
	pid = fork();

    switch (pid) {

    case -1:
        return -1;

    case 0: //子进程
        ngx_pid = ngx_getpid();  // 设置子进程ID  
        char *argv[]={"nginx", NULL};     
　　    char *envp[]={"PATH=/bin", NULL};
        execve("/home/wl/nginx",argv,envp);
        break;

    default: //父进程，但是这时候打印的pid为子进程ID
        //printf(" ......master.....pid:%u, %u\n", pid, ngx_pid); master.....pid:14127, 14126
        break;
    }
	
	return 0;
}


int main(int argc,char* argv[])
{
	int i = 0;
	if(ngx_create_pidfile("nginx.pid") != 0)
	{
		printf("ngx_create_pidfile --- exit\n");
		goto lblexit; 
	}
	
	if(ngx_init_signals() != 0) //信号初始化
    {
		printf("ngx_init_signals --- exit\n");
        goto lblexit;
    } 
    ngx_process = NGX_PROCESS_MASTER;  
    sigset_t set;        //信号集
    sigemptyset(&set);   //清空信号集

    //下列这些信号在执行本函数期间不希望收到【考虑到官方nginx中有这些信号，老师就都搬过来了】（保护不希望由信号中断的代码临界区）
    //建议fork()子进程时学习这种写法，防止信号的干扰；
    sigaddset(&set, SIGCHLD);     //子进程状态改变
    sigaddset(&set, SIGALRM);     //定时器超时
    sigaddset(&set, SIGIO);       //异步I/O
    sigaddset(&set, SIGINT);      //终端中断符
    sigaddset(&set, SIGHUP);      //连接断开
    sigaddset(&set, SIGUSR1);     //用户定义信号
    sigaddset(&set, SIGUSR2);     //用户定义信号
    sigaddset(&set, SIGWINCH);    //终端窗口大小改变
    sigaddset(&set, SIGTERM);     //终止
    sigaddset(&set, SIGQUIT);     //终端退出符
    //.........可以根据开发的实际需要往其中添加其他要屏蔽的信号......
    
    //设置，此时无法接受的信号；阻塞期间，你发过来的上述信号，多个会被合并为一个，暂存着，等你放开信号屏蔽后才能收到这些信号。。。
    //sigprocmask()在第三章第五节详细讲解过
    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) //第一个参数用了SIG_BLOCK表明设置 进程 新的信号屏蔽字 为 “当前信号屏蔽字 和 第二个参数指向的信号集的并集
    {        
        //ngx_log_error_core(NGX_LOG_ALERT,errno,"ngx_master_process_cycle()中sigprocmask()失败!");
        printf("sigprocmask --- exit\n");
		goto lblexit;
    }

	
	g_argvneedmem = 0;
    for(i = 0; i < argc; i++)  //argv =  ./nginx -a -b -c asdfas
    {
        g_argvneedmem += strlen(argv[i]) + 1; //+1是给\0留空间。
    } 
	
	g_os_argc = argc;           //保存参数个数
    g_os_argv = (char **) argv; //保存参数指针
	
	//统计环境变量所占的内存。注意判断方法是environ[i]是否为空作为环境变量结束标记
    for(i = 0; environ[i]; i++) 
    {
        g_envneedmem += strlen(environ[i]) + 1; //+1是因为末尾有\0,是占实际内存位置的，要算进来
		printf("%s\n",environ[i]);
    } 
	
	ngx_setproctitle();
	sigemptyset(&set); 
	for(;;)
    {
		 sigsuspend(&set); 
		 
		 if(ngx_change_binary == 1)
		 {
			 ngx_exec_new_binary();
			 ngx_change_binary = 0;
		 }
		 if(ngx_quit_flag == 1)
		 {
			 ngx_quit_flag = 0;
			 printf("ngx_quit_flag exit\n");
			 break;
		 }
	}
lblexit:
	return 0;
}



// sudo kill -USR2 `cat /home/wl/nginx.pid`
// kill -3 `cat /home/wl/nginx.pid.oldbin`




