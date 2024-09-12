#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <linux/input.h>
#include <math.h>
#include "touch.h"

int touchFd = 0;

// 触摸状态  默认松开
TouchState touchstate = Release;
int touchX = 0, touchY = 0;

int touchInit()
{
	touchFd = open("/dev/input/event0", O_RDONLY);
	if (touchFd == -1)
	{
		perror("open touch fail");
		return -1;
	}

	return 0;
}

void touchUninit()
{
	close(touchFd);
}

int touchGetXY(int *x, int *y)
{
	struct input_event ev;
	int x1 = -1, y1 = -1;
	while (1)
	{
		read(touchFd, &ev, sizeof(ev));
		if (ev.type == EV_ABS && ev.code == ABS_X)
			x1 = ev.value;
		else if (ev.type == EV_ABS && ev.code == ABS_Y)
			y1 = ev.value;

		if (x1 != -1 && y1 != -1)
			break;
	}

	// x 理论 0-799  实际 0-1023 等比例缩小
	*x = x1 * 800 / 1024;
	*y = y1 * 480 / 600;

	return 0;
}

// 获取坐标 直到松开
int touchGetXYRelease(int *x, int *y)
{
	struct input_event ev;
	int x1 = -1, y1 = -1;
	while (1)
	{
		read(touchFd, &ev, sizeof(ev));
		if (ev.type == EV_ABS && ev.code == ABS_X)
			x1 = ev.value;
		else if (ev.type == EV_ABS && ev.code == ABS_Y)
			y1 = ev.value;
		else if (ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0)
			break;
	}

	// x 理论 0-799  实际 0-1023 等比例缩小
	*x = x1 * 800 / 1024;
	*y = y1 * 480 / 600;

	return 0;
}

// 函数判断此时是否有按下
TouchState touchPress()
{
	struct input_event ev;
	read(touchFd, &ev, sizeof(ev));
	// 表示按下
	if (ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 1)
		return Press;

	return Release;
}

// 判断触摸方向
int touchFx(int x1, int y1, int x2, int y2)
{

	if (abs(x1 - x2) > abs(y1 - y2))
	{
		// 左滑
		if (x1 > x2)
		{
			return 1; // 左滑
		}
		else
		{
			return 2; // 右滑
		}
	}
	//
	else if (abs(x1 - x2) < abs(y1 - y2))
	{
		if (y1 > y2)
		{
			return 3; // 上滑
		}
		else
		{
			return 4; // 下滑
		}
	}
}
