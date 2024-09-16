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
					//printf("%s\n", buff);
					paserrGps(buff);
					//  处理完一条语句，重置 buff 和 buff_index
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
#include "lv_port_indev.h"
#include "pthread.h"
#include "lv_demos.h"

void *timer(void *arg)
{
	while (1)
	{
		lv_tick_inc(20);
		usleep(20 * 1000);
	}
	return 0;
}

void touching_cb(lv_event_t *e);
void released_cb(lv_event_t *e);
lv_obj_t *obj;

#include "lvgl/src/drivers/evdev/lv_evdev.h" // for touch screen
int main10()
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
	lv_indev_t *indev = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event0");
	lv_evdev_set_calibration(indev, 0, 0, 1024, 600);

	struct bmpInfo bmpInfo;
	bmpGetInfo("./map.bmp", &bmpInfo);
	obj = lv_obj_create(lv_scr_act());
	lv_obj_set_style_bg_color(obj, lv_color_hex(0xaaaaaa), LV_PART_MAIN);
	lv_obj_set_style_radius(obj,0,LV_PART_MAIN);
	lv_obj_set_size(obj, 700, 460);
	lv_obj_set_y(obj, 10);
	
	lv_obj_t *canvas = lv_canvas_create(obj);

	lv_obj_remove_style(obj, NULL, LV_PART_SCROLLBAR);
	// lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_add_event_cb(canvas, touching_cb, LV_EVENT_PRESSING, NULL);
	lv_obj_add_event_cb(canvas, released_cb, LV_EVENT_RELEASED, NULL);

	    /*Create an array for the points of the line*/
    static lv_point_precise_t line_points[] = { {5, 5}, {70, 70}, {120, 10}, {180, 60}, {240, 10} };

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 8);
	
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_line, true);

    /*Create a line and apply the new style*/
    lv_obj_t * line1;
    line1 = lv_line_create(canvas);
    lv_line_set_points(line1, line_points, 5);     /*Set the points*/
	lv_obj_add_style(line1, &style_line, 0);

	lv_canvas_set_buffer(canvas, bmpInfo.bmpPixelArr, bmpInfo.bmpWidth, bmpInfo.bmpHeight, LV_COLOR_FORMAT_RGB888);

	// lv_obj_t *label = lv_label_create(lv_scr_act());
	// lv_font_t *font_hm = lv_binfont_create("A:/user/font_hm.bin");
	// printf("Success to load font\n");
	// lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
	// lv_obj_set_width(label, 320);
	// lv_obj_set_style_text_font(label, font_hm, 0);
	// lv_label_set_text(label, "ESP32-S3 是一款集成 2.4 GHz Wi-Fi 和 Bluetooth 5 (LE) 的 MCU 芯片，支持远距离模式 (Long Range)。 ESP32-S3 搭载 Xtensa® 32 位 LX7 双核处理器，主频高达 240 MHz，内置 512 KB SRAM (TCM)，具有 45 个可编程 GPIO 管脚和丰富的通信接口。ESP32-S3 支持更大容量的高速 Octal SPI flash 和片外 RAM，支持用户配置数据缓存与指令缓存。ESP32-S3 集成 2.4 GHz Wi-Fi (802.11 b/g/n)，支持 40 MHz 带宽；其低功耗蓝牙子系统支持 Bluetooth 5 (LE) 和 Bluetooth Mesh，可通过 Coded PHY 与广播扩展实现远距离通信。它还支持 2 Mbps PHY，用于提高传输速度和数据吞吐量。ESP32-S3 的 Wi-Fi 和 Bluetooth LE 射频性能优越，在高温下也能稳定工作。");

	while (1)
	{
		lv_timer_handler();
		usleep(5 * 1000);
	}
}
#endif

static int offsetx, offsety, tuoching;
void touching_cb(lv_event_t *e)
{
	printf("touching\n");
	lv_obj_move_foreground(obj);

	lv_point_t click_point;
	lv_indev_get_point(lv_indev_get_act(), &click_point);
	printf("x:%d y:%d\n", click_point.x, click_point.y);
	if (tuoching == 0 && click_point.y - lv_obj_get_y(obj) > 20)
		return;

	if (tuoching == 0)
	{
		offsetx = click_point.x - lv_obj_get_x(obj);
		offsety = click_point.y - lv_obj_get_y(obj);
		tuoching = 1;
		return;
	}
	lv_obj_set_pos(obj, click_point.x - offsetx, click_point.y - offsety);
}
void released_cb(lv_event_t *e)
{
	printf("released\n");
	tuoching = 0;
}

int main()
{
	main10();

	return 0;
}
