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
#include "pthread.h"

int touchFd = 0;

// 触摸状态  默认松开
TouchState touchstate = Release;
int touchX = 0, touchY = 0, pressed = 0;
int lookup_table[601] = {0, 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9, 10, 11, 12, 12, 13, 14, 15, 16, 16, 17, 18, 19, 20, 20, 21, 22, 23, 24, 24, 25, 26, 27, 28, 28, 29, 30, 31, 32, 32, 33, 34, 35, 36, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 44, 45, 46, 47, 48, 48, 49, 50, 51, 52, 52, 53, 54, 55, 56, 56, 57, 58, 59, 60, 60, 61, 62, 63, 64, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 72, 73, 74, 75, 76, 76, 77, 78, 79, 80, 80, 81, 82, 83, 84, 84, 85, 86, 87, 88, 88, 89, 90, 91, 92, 92, 93, 94, 95, 96, 96, 97, 98, 99, 100, 100, 101, 102, 103, 104, 104, 105, 106, 107, 108, 108, 109, 110, 111, 112, 112, 113, 114, 115, 116, 116, 117, 118, 119, 120, 120, 121, 122, 123, 124, 124, 125, 126, 127, 128, 128, 129, 130, 131, 132, 132, 133, 134, 135, 136, 136, 137, 138, 139, 140, 140, 141, 142, 143, 144, 144, 145, 146, 147, 148, 148, 149, 150, 151, 152, 152, 153, 154, 155, 156, 156, 157, 158, 159, 160, 160, 161, 162, 163, 164, 164, 165, 166, 167, 168, 168, 169, 170, 171, 172, 172, 173, 174, 175, 176, 176, 177, 178, 179, 180, 180, 181, 182, 183, 184, 184, 185, 186, 187, 188, 188, 189, 190, 191, 192, 192, 193, 194, 195, 196, 196, 197, 198, 199, 200, 200, 201, 202, 203, 204, 204, 205, 206, 207, 208, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 216, 217, 218, 219, 220, 220, 221, 222, 223, 224, 224, 225, 226, 227, 228, 228, 229, 230, 231, 232, 232, 233, 234, 235, 236, 236, 237, 238, 239, 240, 240, 241, 242, 243, 244, 244, 245, 246, 247, 248, 248, 249, 250, 251, 252, 252, 253, 254, 255, 256, 256, 257, 258, 259, 260, 260, 261, 262, 263, 264, 264, 265, 266, 267, 268, 268, 269, 270, 271, 272, 272, 273, 274, 275, 276, 276, 277, 278, 279, 280, 280, 281, 282, 283, 284, 284, 285, 286, 287, 288, 288, 289, 290, 291, 292, 292, 293, 294, 295, 296, 296, 297, 298, 299, 300, 300, 301, 302, 303, 304, 304, 305, 306, 307, 308, 308, 309, 310, 311, 312, 312, 313, 314, 315, 316, 316, 317, 318, 319, 320, 320, 321, 322, 323, 324, 324, 325, 326, 327, 328, 328, 329, 330, 331, 332, 332, 333, 334, 335, 336, 336, 337, 338, 339, 340, 340, 341, 342, 343, 344, 344, 345, 346, 347, 348, 348, 349, 350, 351, 352, 352, 353, 354, 355, 356, 356, 357, 358, 359, 360, 360, 361, 362, 363, 364, 364, 365, 366, 367, 368, 368, 369, 370, 371, 372, 372, 373, 374, 375, 376, 376, 377, 378, 379, 380, 380, 381, 382, 383, 384, 384, 385, 386, 387, 388, 388, 389, 390, 391, 392, 392, 393, 394, 395, 396, 396, 397, 398, 399, 400, 400, 401, 402, 403, 404, 404, 405, 406, 407, 408, 408, 409, 410, 411, 412, 412, 413, 414, 415, 416, 416, 417, 418, 419, 420, 420, 421, 422, 423, 424, 424, 425, 426, 427, 428, 428, 429, 430, 431, 432, 432, 433, 434, 435, 436, 436, 437, 438, 439, 440, 440, 441, 442, 443, 444, 444, 445, 446, 447, 448, 448, 449, 450, 451, 452, 452, 453, 454, 455, 456, 456, 457, 458, 459, 460, 460, 461, 462, 463, 464, 464, 465, 466, 467, 468, 468, 469, 470, 471, 472, 472, 473, 474, 475, 476, 476, 477, 478, 479, 480};

void *touch_thread(void *arg)
{
	touchInit();
	while (1)
	{
		touchGetXYPress();
		//printf("x=%d,y=%d\n", touchX, touchY);
	}
	return 0;
}

int touchInit()
{
	touchFd = open("/dev/input/event0", O_RDONLY);
	if (touchFd == -1)
	{
		perror("open touch fail");
		return -1;
	}

	pthread_t tid;
	pthread_create(&tid, NULL, touch_thread, NULL);
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

int touchGetXYPress()
{
	struct input_event ev;
	static int tempX = -1, tempY = -1;
	read(touchFd, &ev, sizeof(ev));

	if (ev.type == EV_ABS && ev.code == ABS_X)
	{
		tempX = (ev.value * 800) >> 10;
	}
	else if (ev.type == EV_ABS && ev.code == ABS_Y)
	{
		tempY = lookup_table[ev.value];
	}
	if (ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 1)
	{
		touchX=tempX;
		touchY=tempY;
		pressed = 1;
		printf("x=%d,y=%d\n", touchX, touchY);
	}
	else if (ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0)
	{
		touchX=tempX;
		touchY=tempY;
		pressed = 0;
		printf("x=%d,y=%d\n", touchX, touchY);
	}

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
