#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "string.h"
#include <sys/mman.h>

int lcdFd;
unsigned int *plcd;

void lcdInit()
{
	// 1. 打开屏幕
	lcdFd = open("/dev/fb0", O_RDWR);
	if (lcdFd == -1)
	{
		perror("open lcd fail");
		return;
	}

	plcd = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcdFd, 0);
	if (plcd == MAP_FAILED)
	{
		perror("map fail");
		return;
	}
}

void lcdUninit()
{
	munmap(plcd, 800 * 480 * 4);
	close(lcdFd);
}

// 在屏幕上x y 坐标位置画color
void lcdDrawPoint(int x, int y, unsigned int color)
{
	if (x >= 0 && x < 800 && y >= 0 && y < 480)
		*(plcd + y * 800 + x) = color;
}

// 刷屏
void lcdFflush(unsigned int color)
{
	for (int y = 0; y < 480; y++)
	{
		for (int x = 0; x < 800; x++)
		{
			lcdDrawPoint(x, y, color);
		}
	}
}

// x,y起始坐标 width 宽度 length长度 color颜色
void lcdDrawTran(int x, int y, int width, int length, unsigned int color)
{
	for (int i = y; i < y + width; i++)
	{
		for (int j = x; j < x + length; j++)
		{
			lcdDrawPoint(j, i, color);
		}
	}
}
