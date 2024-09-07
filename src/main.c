#include "bmp.h"
#include "lcd.h"
#include "touch.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "word.h"
#include "lcd_config.h"

int main1()
{
	lcdInit();

	// 显示图片之前一定要 对屏幕做初始化
	struct bmpInfo bmpInfo;
	memset(&bmpInfo, 0, sizeof(bmpInfo));
	bmpGetInfo("./lean.bmp", &bmpInfo);
	lcdFflush(0x00FFFFFF);
	bmpShow((400 - (bmpInfo.bmpWidth / 2)), (240 - (bmpInfo.bmpHeight / 2)), &bmpInfo);

	lcdUninit();

	return 0;
}

int main2()
{
	touchInit();

	int x, y;
	touchGetXY(&x, &y);
	printf("(%d, %d)\r\n", x, y);

	touchUninit();

	return 0;
}

int main3()
{
	touchInit();

	int x1, y1;
	int x2, y2;

	extern TouchState touchstate;
	// 等待 按下的行为
	while ((touchstate = touchPress()) == Release)
		;
	if (touchstate == Press)
		touchGetXY(&x1, &y1);

	// 一直获取坐标 直到松开
	touchGetXYRelease(&x2, &y2);

	int ret = touchFx(x1, y1, x2, y2);
	if (ret == 1)
		printf("touch Left\n");
	else if (ret == 2)
	{
		printf("touch Right\n");
	}
	else if (ret == 3)
	{
		printf("touch up\n");
	}
	else if (ret == 4)
	{
		printf("touch down\n");
	}

	touchUninit();

	return 0;
}

int main4()
{
	lcdInit();
	touchInit();
	// 显示图片之前一定要 对屏幕做初始化
	struct bmpInfo bmpInfo;
	memset(&bmpInfo, 0, sizeof(bmpInfo));
	bmpGetInfo("./lean1024.bmp", &bmpInfo);
	lcdFflush(0x00FFFFFF);


	// 翻转图片
	bmpReverse(&bmpInfo);
	bmpShow(0, 0, &bmpInfo);
	int offsetx = 0;
	int offsety = 0;
	while (1)
	{
		int x1, y1;
		int x2, y2;
		int offset = 40;
		extern TouchState touchstate;
		// 等待 按下的行为
		while ((touchstate = touchPress()) == Release)
			;
		if (touchstate == Press)
			touchGetXY(&x1, &y1);

		// 一直获取坐标 直到松开
		touchGetXYRelease(&x2, &y2);

		int ret = touchFx(x1, y1, x2, y2);
		if (ret == 1)
		{
			offsetx -= offset;
			bmpShow(offsetx, offsety, &bmpInfo);
			printf("touch Left\n");
		}
		else if (ret == 2)
		{
			offsetx += offset;
			bmpShow(offsetx, offsety, &bmpInfo);
			printf("touch Right\n");
		}
		else if (ret == 3)
		{
			offsety -= offset;
			bmpShow(offsetx, offsety, &bmpInfo);
			printf("touch up\n");
		}
		else if (ret == 4)
		{
			offsety += offset;
			bmpShow(offsetx, offsety, &bmpInfo);
			printf("touch down\n");
		}
		printf("offsetx=%d, offsety=%d\n", offsetx, offsety);
	}

	lcdUninit();
	touchUninit();

	return 0;
}

int main()
{
	lcdInit();

	// 显示图片之前一定要 对屏幕做初始化
	lcdFflush(0x00FFFFFF);

	int offset = 0;
	while (1)
	{
		if (offset >= 800)
		{
			offset = 0;
		}
		usleep(1000);
		for (int i = 0; i < 64; i++)
		{
			lcdDrawPoint((700 - 1 + offset) % 800, 100 + i, WHITE);
		}
		lcdShowOneWord16x16(700 + offset, 100, word);
		offset++;
	}

	lcdUninit();

	return 0;
}
