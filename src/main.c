#include "bmp.h"
#include "lcd.h"
#include "touch.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "word.h"
#include "lcd_config.h"
#include "gy39.h"
#include "gps.h"

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

int main5()
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

int main6()
{
	lcdInit();

	// 显示图片之前一定要 对屏幕做初始化
	lcdFflush(WHITE);

	// 设置前景色
	frontColorSet(ORANGE);
	// 设置背景色
	bgColorSet(BLACK);

	lcdShowOneWord16x16(100, 100, word1);

	lcdShowString8x16(100, 200, "hello wwwww");

	lcdUninit();

	return 0;
}

#include "minmea.h"
void paserrGps(char *buff)
{
	char *line = buff;

	switch (minmea_sentence_id(line))
	{
	case MINMEA_SENTENCE_RMC:
	{
		struct minmea_sentence_rmc frame;
		if (minmea_parse_rmc(&frame, line))
		{
			printf("$RMC: 原始坐标和速度: (%d/%d,%d/%d) %d/%d\n",
				   frame.latitude.value, frame.latitude.scale,
				   frame.longitude.value, frame.longitude.scale,
				   frame.speed.value, frame.speed.scale);
			printf("$RMC 定点坐标和速度缩放到小数点后三位: (%d,%d) %d\n",
				   minmea_rescale(&frame.latitude, 1000),
				   minmea_rescale(&frame.longitude, 1000),
				   minmea_rescale(&frame.speed, 1000));
			printf("$RMC 浮点度坐标和速度: (%f,%f) %f\n",
				   minmea_tocoord(&frame.latitude),
				   minmea_tocoord(&frame.longitude),
				   minmea_tofloat(&frame.speed));
		}
	}
	break;

	case MINMEA_SENTENCE_GGA:
	{
		struct minmea_sentence_gga frame;
		if (minmea_parse_gga(&frame, line))
		{
			printf("$GGA: 定位状态: %d\n", frame.fix_quality);
		}
	}
	break;

	case MINMEA_SENTENCE_GSV:
	{
		struct minmea_sentence_gsv frame;
		if (minmea_parse_gsv(&frame, line))
		{
			printf("$GSV: message %d of %d\n", frame.msg_nr, frame.total_msgs);
			printf("$GSV: satellites in view: %d\n", frame.total_sats);
			for (int i = 0; i < 4; i++)
				printf("$GSV: sat 噪声比 %d, 海拔: %d, 方位角: %d, 信噪比: %d dbm\n",
					   frame.sats[i].nr,
					   frame.sats[i].elevation,
					   frame.sats[i].azimuth,
					   frame.sats[i].snr);
		}
	}
	break;
	}
}

int main7()
{
	// 初始化串口，设置波特率为9600
	int fd_gps = initSerial("/dev/ttySAC2", 9600);
	if (-1 == fd_gps)
	{
		perror("gps init error");
		return -1;
	}
	// 循环地从串口中读取数据
	char str[512] = {0};   // 用于存放每次读取的数据
	char buff[1024] = {0}; // 用于存放完整的GPS语句，大小可以根据实际情况调整
	int buff_index = 0;	   // 用于记录buff中的写入位置

	while (1)
	{
		int res = read(fd_gps, str, 512); // 假设fd_gps已经正确初始化

		if (res > 0)
		{
			for (int i = 0; i < res; i++)
			{
				// 如果遇到 \r\n，表示一句GPS语句的结束
				if (str[i] == '\r' && i + 1 < res && str[i + 1] == '\n')
				{
					buff[buff_index] = '\0'; // 将语句以 \0 结尾

					// 打印完整的 GPS 语句
					printf("%s\n", buff);
					//paserrGps(buff);
					// 处理完一条语句，重置 buff 和 buff_index
					memset(buff, 0, sizeof(buff));
					buff_index = 0;
					// 跳过 \r\n
					i++;
				}
				else
				{
					// 将字符逐个追加到 buff 中
					buff[buff_index++] = str[i];

					// 防止 buff 溢出
					if (buff_index >= sizeof(buff) - 1)
					{
						buff[buff_index] = '\0'; // 防止溢出时加上字符串结束符
						printf("Buffer overflow, truncated sentence: %s\n", buff);
						memset(buff, 0, sizeof(buff)); // 清空缓冲区
						buff_index = 0;
					}
				}
			}
		}
	}
	// 分析并计算数据
	// 关闭串口
	close(fd_gps);

	return 0;
}

int main8()
{
	// ttySAC1  com1
	// ttySAC2  com2
	lcdInit();
	int uart1_fd = uart_init("/dev/ttySAC1");
	lcdFflush(0x00FFFFFF);
	struct bmpInfo bmpInfo;
	memset(&bmpInfo, 0, sizeof(bmpInfo));
	bmpGetInfo("./wd.bmp", &bmpInfo);
	bmpReverse(&bmpInfo);

	while (1)
	{
		get_GZ(uart1_fd);
		printf("GZ:%d\n", GZ);
		sleep(1);
		get_other(uart1_fd);
		if (WD >= 31)
		{
			bmpShow(0, 0, &bmpInfo);
		}
		else
		{
			lcdFflush(0x00FFFFFF);
		}
		char str[50];
		printf("WD:%3d SD:%3d QY:%6d HB:%4d\n", WD, SD, QY, HB);
		sprintf(str, "WD:%3d SD:%3d QY:%6d HB:%4d", WD, SD, QY, HB);
		lcdShowString8x16(0, 0, str);
		sleep(1);
	}

	close(uart1_fd);
}

#include "graph.h"
/*
ABCDEFGHI
A B 5
A E 12
A C 17
B E 12
C F 1
D E 8
D G 4
D I 2
E F 6
E G 9
H I 5
# # 6
*/
int main9()
{
	Graph *graph = graphInit();

	graphVRInput(graph);

	graphPrint(graph);

	graphDJST(graph, 0);

	graphUninit(graph);

	return 0;
}

#define LVGL
#ifdef LVGL

#include "lvgl.h"
#include "lv_port_disp.h"
#include "pthread.h"
#include "lv_demos.h"

void *timer(void *arg)
{
	while (1)
	{
		lv_tick_inc(1);
		usleep(1000);
	}
	return 0;
}


int main()
{
	pthread_t tid;
	int rc = pthread_create(&tid, NULL, timer, NULL);
	if (rc)
	{
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
	lv_init();
	lv_port_disp_init();
	lv_demo_benchmark();

	while (1)
	{
		lv_timer_handler();
		usleep(5 * 1000);
	}
}
#endif


