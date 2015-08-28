#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<Message.h>
#include <sys/types.h>
#include <sys/stat.h>
#define SERVE_FIFO "serve_public"
Mesg loin,mes;
loing* head=NULL;
void sys_err(char *str)
{
	perror(str);
	exit(1);
}
void dong(int i)
{


	int ret = Deletename(head,mes.destname);
	int	write_id = Selectname(head,mes.username);
	char buf[]=":异常下线";
	char name[50];
	strcpy(name,mes.username);
	strcat(name,buf);
	strcpy(mes.username,mes.destname);
	strcpy(mes.message,name);
	write(write_id,&mes,sizeof(mes));
}
int main(void)
{
	head = Init();
	int plic_id,pive_id,read_id;
	char buf[1024];

	signal(SIGPIPE,dong);
	//检查公共管道是否存在，不存在就建立
	if(access(SERVE_FIFO,F_OK))
		mkfifo(SERVE_FIFO,0777);
	//打开公共管道
	if((plic_id = open(SERVE_FIFO,O_RDONLY ))<0)
		sys_err("open plice fifo error");
	while(1)
	{
		int n = 0;
		//从公共管道中读取数据
		n = read(plic_id,&mes,sizeof(mes));
		if(n>0)
		{     //如果id是1则代表登录
			if(mes.id == 1)
			{
				//登录时打开私有管道，并把pid写如链表
				if(( pive_id = open(mes.username,O_WRONLY ))<0)
					sys_err("open prive fifo error");
				int ret= Inset(head,pive_id,mes.username);
				if(ret>0)//此处说明此用户已经登录过
				{
					strcpy(mes.username,mes.username);
					char mms[]="请不要重复登录";
					strcpy(mes.message,mms);
					mes.id=-1;
					write(ret,&mes,sizeof(mes));

				}
				else
				{
					strcpy(mes.username,mes.username);
					char bbf[]="登录成功";
					strcpy(mes.message,bbf);
					write(pive_id,&mes,sizeof(mes));
				}
			}
			if(mes.id == 2)
			{
				if(strcmp(mes.username,mes.destname)==0)
					continue;
				int write_id = Selectname(head,mes.destname);
				if(write_id == 0)
				{
					write_id = Selectname(head,mes.username);
					char mms[50];
					strcpy(mms,mes.destname);
					char mm[] = " :没有在线，请稍等片刻";
					strcat(mms,mm);
					strcpy(mes.username,mes.username);
					strcpy(mes.message,mms);
					write(write_id,&mes,sizeof(mes));
				}
				else
					write(write_id,&mes,sizeof(mes));
			}
			if(mes.id == 0)
			{
				int ret = Deletename(head,mes.username);
				char tui[] ="退出成功！";
				strcpy(mes.username,mes.username);
				strcpy(mes.message,tui);
				mes.id = -1;
				write(ret,&mes,sizeof(mes));
			}
		}
	}
	return 0;
}
