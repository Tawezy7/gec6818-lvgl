#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

/***************************************************
 * 功能：打开并设置串口的波特率
 * 返回值：
 *      -1：设置串口波特率失败
 *      其他值：打开的串口文件的文件描述符
 * 参数：
 *      file：字符串，用来指定要打开的设备文件的路径
 *      baudrate：要设置的串口的波特率(9600、115200、19200)
***************************************************/
int initSerial(const char *file, int baudrate)
{ 
	int fd = open(file, O_RDWR);
	if (fd == -1)
	{
		perror("open device error:");
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
	myserial.c_cflag |= CS8;      //数据位:8
	myserial.c_cflag &= ~CSTOPB;//   //1位停止位
	myserial.c_cflag &= ~PARENB;  //不要校验
	switch (baudrate)
	{
		case 9600:
			cfsetospeed(&myserial, B9600);  //设置波特率
			cfsetispeed(&myserial, B9600);
			break;
		case 115200:
			cfsetospeed(&myserial, B115200);  //设置波特率
			cfsetispeed(&myserial, B115200);
			break;
		case 19200:
			cfsetospeed(&myserial, B19200);  //设置波特率
			cfsetispeed(&myserial, B19200);
			break;
	}
	/* 刷新输出队列,清除正接受的数据 */
	tcflush(fd, TCIFLUSH);
	/* 改变配置 */
	tcsetattr(fd, TCSANOW, &myserial);
	return fd;
}
