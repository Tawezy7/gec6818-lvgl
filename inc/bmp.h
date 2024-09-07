// Author: Tawezy7
// Date: 2024-09-05
#ifndef _INC_BMP_H_
#define _INC_BMP_H_

#ifdef __cplusplus
extern "C"
{
#endif
	struct bmpInfo
	{
		// 保存bmp文件名字
		char bmpName[100];
		// 保存文件描述符
		char bmpFd;
		// 保存了图片像素数组偏移
		unsigned int bmpPixelOffset;
		// 保存了图片色深数据
		unsigned short bmpDepth;
		// 保存了图片位图宽度
		unsigned int bmpWidth;
		// 保存了图片位图高度
		unsigned int bmpHeight;
		// 保存了图片位图数据大小
		unsigned int bmpPixelArrSize;
		// 保存了位图数据
		unsigned char *bmpPixelArr;
	};

	int bmpGetInfo(char *pathname, struct bmpInfo *bmpInfo);
	int bmpShow(int x, int y, struct bmpInfo *bmpInfo);
	int bmpReverse(struct bmpInfo *bmpInfo);

	/*...*/

#ifdef __cplusplus
}
#endif

#endif /* _INC_BMP_H_ */