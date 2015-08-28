#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<Message.h>
#include <sys/types.h>
#include <sys/stat.h>
#define SERVE_FIFO "serve_public"
void sys_err(char *str)
{
	perror(str);
	exit(1);
}
int main(int argc,char* argv[])
{
	int plic_id,pive_id,read_id;
	int fbg;
	char buf[1024];
    Mesg loin,mes;
	if(argc<2)//判断输入格式是否正确
	{
		printf("%s\n","please input like:./clent username");
		return 0;
	}
	//检查公共管道是否存在，不存在就建立
	if(access(SERVE_FIFO,F_OK))
		mkfifo(SERVE_FIFO,0777);
	//不阻塞状态打开与服务器的公共管道
    if((plic_id = open(SERVE_FIFO,O_WRONLY | O_NONBLOCK))<0)
		sys_err("open serve_fifo error");
	//检查私有管道是否存在，不存在就建立
	if(access(argv[1],F_OK))
		mkfifo(argv[1],0777);
	//以不阻塞状态打开私有管道
	if((pive_id=open(argv[1],O_RDONLY | O_NONBLOCK))<0)
		sys_err("open prvate_fifo error");
	//打包登录数据包
     loin.id=1;
	 strcpy(loin.username,argv[1]);
	 //从公共管道传送到服务器，进行登录
     write(plic_id,&loin,sizeof(loin));
	 //以只读方式打开输入终端
	 fbg = fcntl(STDIN_FILENO,F_GETFL);
	 fbg = fbg|O_NONBLOCK;
	 fcntl(STDIN_FILENO,F_SETFL,fbg);
	 char tui[]="T~";
	 char tu[]="t";
	 while(1)
	 {
	   strcpy(mes.username,argv[1]);
        int n =0;
		n = read(STDIN_FILENO,buf,1024);
		if(n > 0)
		{
          mes.id = 2;
          buf[n]='\0';
		  strcpy(mes.destname,strtok(buf," "));
		  strcpy(mes.message,strtok(NULL," "));
		  if((strncmp(mes.message,tui,2)==0))	
			  mes.id = 0;
		  write(plic_id,&mes,sizeof(mes));
		
		}
		int n1 =0;
		n1 = read(pive_id,&mes,sizeof(mes));
		if(n1>0)
		{
		printf("%s",mes.username);
		printf(":\n");
		printf("%s\n",mes.message);
		if(mes.id==-1)
		{
			exit(1);
		}
		}

	 }
   return 0;
}
