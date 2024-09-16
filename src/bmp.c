#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "lcd.h"
#include "touch.h"
#include "bmp.h"

int bmpGetInfo(char *pathname, struct bmpInfo *bmpInfo)
{
	if (pathname && bmpInfo)
	{
		// 保存bmp路径
		strcpy(bmpInfo->bmpName, pathname);
		// 1-> 打开这个文件
		bmpInfo->bmpFd = open(pathname, O_RDONLY);
		if (bmpInfo->bmpFd == -1)
		{
			perror("open bmp fail");
			return -1;
		}

		// 2-> 读取文件信息
		// 2->1 获取魔数、获取BMP文件大小、获取像素数组地址偏移
		char buf[3] = {0};
		read(bmpInfo->bmpFd, buf, 2);
		puts(buf);

		// 将文件读写位置 偏移到 文件大小
		unsigned int bmpSize = 0;
		lseek(bmpInfo->bmpFd, 0x02, SEEK_SET);
		read(bmpInfo->bmpFd, &bmpSize, 4);
		printf("bmpSize:%d\n", bmpSize);

		// 将文件读写位置 偏移到 位图数据的地址偏移
		lseek(bmpInfo->bmpFd, 0x0A, SEEK_SET);
		read(bmpInfo->bmpFd, &bmpInfo->bmpPixelOffset, 4);
		printf("bmpSize:%d\n", bmpInfo->bmpPixelOffset);

		// 将文件读写位置 偏移到 位图宽度的地址偏移
		lseek(bmpInfo->bmpFd, 0x12, SEEK_SET);
		read(bmpInfo->bmpFd, &bmpInfo->bmpWidth, 4);
		printf("bmpWidth:%d\n", bmpInfo->bmpWidth);

		// 将文件读写位置 偏移到 位图高度的地址偏移
		lseek(bmpInfo->bmpFd, 0x16, SEEK_SET);
		read(bmpInfo->bmpFd, &bmpInfo->bmpHeight, 4);
		printf("bmpHeight:%d\n", bmpInfo->bmpHeight);

		// 将文件读写位置 偏移到 色深的地址偏移
		lseek(bmpInfo->bmpFd, 0x1C, SEEK_SET);
		read(bmpInfo->bmpFd, &bmpInfo->bmpDepth, 2);
		printf("bmpDepth:%hd\n", bmpInfo->bmpDepth);

		// 将文件读写位置 偏移到 像素数组大小的地址偏移
		lseek(bmpInfo->bmpFd, 0x22, SEEK_SET);
		read(bmpInfo->bmpFd, &bmpInfo->bmpPixelArrSize, 4);
		printf("bmpPixelArrSize:%d\n", bmpInfo->bmpPixelArrSize);

		// 将文件读写位置 偏移到 像素数组的地址偏移
		lseek(bmpInfo->bmpFd, bmpInfo->bmpPixelOffset, SEEK_SET);

		int rowSizeWithPadding = (bmpInfo->bmpWidth * 3 + 3) & ~3; // 向上取整到4字节的倍数
		int rowSizeWithoutPadding = bmpInfo->bmpWidth * 3;			// 实际行大小（不含对齐）
		bmpInfo->bmpPixelArr = (unsigned char *)malloc(rowSizeWithoutPadding * bmpInfo->bmpHeight);

		// read(bmpInfo->bmpFd, bmpInfo->bmpPixelArr, bmpInfo->bmpPixelArrSize);
		unsigned char *rowBuffer = (unsigned char *)malloc(rowSizeWithPadding);
		for (int i = 0; i < bmpInfo->bmpHeight; i++)
		{
			// 从文件读取一行数据（包含对齐字节）
			read(bmpInfo->bmpFd, rowBuffer, rowSizeWithPadding);
			// 将该行数据中实际的像素部分拷贝到新数组中
			memcpy(bmpInfo->bmpPixelArr + i * rowSizeWithoutPadding, rowBuffer, rowSizeWithoutPadding);
		}
		free(rowBuffer);

		bmpInfo->bmpPixelArrSize = rowSizeWithoutPadding * bmpInfo->bmpHeight;
		bmpReverse(bmpInfo);

		close(bmpInfo->bmpFd);
	}
}

int bmpShow(int x, int y, struct bmpInfo *bmpInfo)
{
	// bmp 信息结构体 以及 像素数组结构体不能为空
	if (bmpInfo && bmpInfo->bmpPixelArr)
	{
#if 0 /*居中*/
		int screen_center_x = 800 / 2;
		int screen_center_y = 480 / 2;
		int image_center_x = bmpInfo->bmpWidth / 2;
		int image_center_y = bmpInfo->bmpHeight / 2;
		int image_x = screen_center_x - image_center_x;
		int image_y = screen_center_y - image_center_y;
#else
		int image_x = 0;
		int image_y = 0;
#endif
		unsigned int color = 0;
		unsigned int k = 0;
		for (int i = 0; i < bmpInfo->bmpHeight; i++)
		{
			for (int j = 0; j < bmpInfo->bmpWidth; j++)
			{
				// RGB888
				if (bmpInfo->bmpDepth == 24)
				{
					color = bmpInfo->bmpPixelArr[k + 0] | bmpInfo->bmpPixelArr[k + 1] << 8 | bmpInfo->bmpPixelArr[k + 2] << 16 | 0 << 24;

					lcdDrawPoint(x + j + image_x, y + i + image_y, color);
					k += 3;
				}
				// ARGB8888
				else if (bmpInfo->bmpDepth == 32)
				{
					color = bmpInfo->bmpPixelArr[k + 0] | bmpInfo->bmpPixelArr[k + 1] << 8 | bmpInfo->bmpPixelArr[k + 2] << 16 | bmpInfo->bmpPixelArr[k + 3] << 24;

					lcdDrawPoint(x + j + image_x, y + i + image_y, color);
					k += 4;
				}
			}
		}
	}
}

int bmpReverse(struct bmpInfo *bmpInfo)
{
	unsigned char *buff = malloc(bmpInfo->bmpPixelArrSize);
	memcpy(buff, bmpInfo->bmpPixelArr, bmpInfo->bmpPixelArrSize);

	int n = 0;
	for (int i = (bmpInfo->bmpHeight * bmpInfo->bmpWidth) - 1; i >= 0; i--)
	{
		bmpInfo->bmpPixelArr[n] = buff[i * 3 + 0];
		bmpInfo->bmpPixelArr[n + 1] = buff[i * 3 + 1];
		bmpInfo->bmpPixelArr[n + 2] = buff[i * 3 + 2];
		n += 3;
	}
	free(buff);
	// 水平翻转
	for (int i = 0; i < bmpInfo->bmpHeight; i++)
	{
		for (int j = 0; j < bmpInfo->bmpWidth / 2; j++)
		{
			unsigned char temp;
			temp = bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + j * 3 + 0]; // R
			bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + j * 3 + 0] = bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + (bmpInfo->bmpWidth - j - 1) * 3 + 0];
			bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + (bmpInfo->bmpWidth - j - 1) * 3 + 0] = temp;

			temp = bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + j * 3 + 1]; // G
			bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + j * 3 + 1] = bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + (bmpInfo->bmpWidth - j - 1) * 3 + 1];
			bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + (bmpInfo->bmpWidth - j - 1) * 3 + 1] = temp;

			temp = bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + j * 3 + 2]; // B
			bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + j * 3 + 2] = bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + (bmpInfo->bmpWidth - j - 1) * 3 + 2];
			bmpInfo->bmpPixelArr[i * bmpInfo->bmpWidth * 3 + (bmpInfo->bmpWidth - j - 1) * 3 + 2] = temp;
		}
	}
}
