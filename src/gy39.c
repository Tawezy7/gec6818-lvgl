#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <termios.h> //配置串口的头文件

int uart_init(const char * uart_name)
{
	/*设置串口
	  波特率:9600
	  数据位:8
	  校验位:不要
	  停止位:1
	  数据流控制:无
	  */
	
	int   uart1_fd= open(uart_name, O_RDWR);//打开串口1设备文件
	if (uart1_fd == -1)
	{
		perror("open error:");
		return -1;
	}

	struct termios myserial;
	//清空结构体
	memset(&myserial, 0, sizeof (myserial));
	//O_RDWR               
	myserial.c_cflag |= (CLOCAL | CREAD);
	//设置控制模式状态，本地连接，接受使能
	//设置 数据位
	myserial.c_cflag &= ~CSIZE;   //清空数据位
	//myserial.c_cflag &= ~CRTSCTS; //无硬件流控制
	myserial.c_cflag |= CS8;      //数据位:8

	myserial.c_cflag &= ~CSTOPB;//   //1位停止位
	myserial.c_cflag &= ~PARENB;  //不要校验

	cfsetospeed(&myserial, B9600);  //设置波特率,B9600是定义的宏
	cfsetispeed(&myserial, B9600);

	/* 刷新输出队列,清除正接受的数据 */
	tcflush(uart1_fd, TCIFLUSH);

	/* 改变配置 */
	tcsetattr(uart1_fd, TCSANOW, &myserial);
	return   uart1_fd;
}
unsigned char GY39_cmd1[]={0xA5,0x51,0xF6};//测光照强度命令
unsigned char GY39_cmd2[]={0xA5,0x52,0xF7};//测温度、湿度、大气、海拔命令

int GZ,WD,SD,QY,HB;
void get_GZ(int fd)
{
	int i=0,flag=0;
	unsigned char recvbuf[9]={0};
	// 将发送光照强度的命令  传输到gy39
	write(fd,GY39_cmd1,3);
	while(1)
	{
		unsigned char buf ;
		read(fd,&buf,1);
		if(buf == 0x5A)
		{
			flag = 1;
		}
		if(flag == 1)
		{
			recvbuf[i++] = buf;
			if(i == 9)
			{
				break;
			}
		}
	}
	
	GZ = recvbuf[4]<<24 | recvbuf[5]<<16 | recvbuf[6]<<8 | recvbuf[7];
	GZ = GZ/100;
}

void get_other(int fd)
{
	int i=0,flag=0;
	unsigned char recvbuf[15]={0};
	write(fd,GY39_cmd2,3);
	while(1)
	{
		unsigned char buf ;
		read(fd,&buf,1);
		if(buf == 0x5A)
		{
			flag = 1;
		}
		if(flag == 1)
		{
			recvbuf[i++] = buf;
			if(i == 15)
			{
				break;
			}
		}
		
	}
	WD = recvbuf[4]<<8 | recvbuf[5];
	WD = WD/100;
	QY = recvbuf[6]<<24 | recvbuf[7]<<16 | recvbuf[8]<<8 | recvbuf[9];
	QY = QY/100;
	SD = recvbuf[10]<<8 | recvbuf[11];
	SD = SD/100;
	HB = recvbuf[12]<<8 | recvbuf[13];
}
