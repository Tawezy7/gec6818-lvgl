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

/*
0123456789
0 1 5
0 4 12
0 2 17
1 4 12
2 5 1
3 4 8
3 6 4
3 8 2
4 5 6
4 6 9
7 8 5
# # 6
*/
#include "gpsdata.h"
int main9()
{
	Graph *graph = graphInit();

	gps_graphVRInput(graph);

	// graphPrint(graph);

	gps_graphDJST(graph, 0, 34);

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
#include "gpsdata.h"

void *timer(void *arg)
{
	while (1)
	{
		lv_tick_inc(20);
		usleep(20 * 1000);
	}
	return 0;
}
#define LEFT_OFFSET 10

static void drag_event_handler(lv_event_t *e);
static void putmark_event_cb(lv_event_t *e);
static void close_event_handler(lv_event_t * e);

void location_cb(lv_event_t *e);
Graph *graph; // 图
lv_obj_t *obj;
lv_obj_t *obj2;
lv_obj_t *canvas;
lv_obj_t *canvas2;
lv_obj_t *bt_location, *label;
static lv_style_t style_line;
lv_obj_t *cont;
lv_obj_t *btn_close;

// 定义碰撞箱结构体
typedef struct {
    int x;
    int y;
    int width;
    int height;
} BoundingBox;

// 判断一个点是否在碰撞箱内的函数
int isPointInBox(BoundingBox box, float px, float py) {
    if (px >= box.x && px <= box.x + box.width &&
        py >= box.y && py <= box.y + box.height) {
        return 1; // 点在碰撞箱内
    }
    return 0; // 点不在碰撞箱内
}

struct bmpInfo bmpInfo_earth; 		//地球 967，279		
struct bmpInfo bmpInfo_sw;			//生物与化学工程学院 523，943
struct bmpInfo bmpInfo_network;		//网络与现代教育技术中心 1175，590
struct bmpInfo bmpInfo_mxyd;		//敏学一栋 1553，424
struct bmpInfo bmpInfo_library;		//图书馆 876，928
struct bmpInfo bmpInfo_lgl;			//理工楼 1206，287

#define BOX_WIDTH 100

BoundingBox earth_boundingBox = {967-BOX_WIDTH/2,279-BOX_WIDTH/2, BOX_WIDTH, BOX_WIDTH};
BoundingBox sw_boundingBox = {523-BOX_WIDTH/2,943-BOX_WIDTH/2, BOX_WIDTH, BOX_WIDTH};
BoundingBox network_boundingBox = {1175-BOX_WIDTH/2,590-BOX_WIDTH/2, BOX_WIDTH, BOX_WIDTH};
BoundingBox mxyd_boundingBox = {1553-BOX_WIDTH/2,424-BOX_WIDTH/2, BOX_WIDTH, BOX_WIDTH};
BoundingBox library_boundingBox = {876-BOX_WIDTH/2,928-BOX_WIDTH/2, BOX_WIDTH, BOX_WIDTH};
BoundingBox lgl_boundingBox = {1206-BOX_WIDTH/2,287-BOX_WIDTH/2, BOX_WIDTH, BOX_WIDTH};

#include "lvgl/src/drivers/evdev/lv_evdev.h" // for touch screen
int main10()
{
	pthread_t tid;
	pthread_t tid_gps;
	pthread_create(&tid_gps, NULL, main7, NULL);
	int rc = pthread_create(&tid, NULL, timer, NULL);
	if (rc)
	{
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}

	graph = graphInit();
	gps_graphVRInput(graph);

	lv_init();
	lv_port_disp_init();
	lv_font_t *font_hm = lv_binfont_create("A:/user/font_hm.bin");
	lv_indev_t *indev = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event0");
	lv_evdev_set_calibration(indev, 0, 0, 1024, 600);

	struct bmpInfo bmpInfo;
	bmpGetInfo("./map.bmp", &bmpInfo);

	bmpGetInfo("./earth.bmp", &bmpInfo_earth);
	bmpGetInfo("./sw.bmp", &bmpInfo_sw);
	bmpGetInfo("./network.bmp", &bmpInfo_network);
	bmpGetInfo("./mxyd.bmp", &bmpInfo_mxyd);
	bmpGetInfo("./library.bmp", &bmpInfo_library);
	bmpGetInfo("./lgl.bmp", &bmpInfo_lgl);

	obj = lv_obj_create(lv_scr_act());
	lv_obj_set_style_bg_color(obj, lv_color_hex(0xaaaaaa), LV_PART_MAIN);
	lv_obj_set_style_radius(obj, 0, LV_PART_MAIN);
	lv_obj_set_size(obj, 600, 460);
	lv_obj_set_y(obj, 10);
	lv_obj_set_x(obj, 10);
	lv_obj_set_style_pad_all(obj, 0, 0);

	canvas = lv_canvas_create(obj);

	lv_obj_remove_style(obj, NULL, LV_PART_SCROLLBAR);
	lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_remove_style(obj, NULL, LV_PART_SCROLLBAR);

	static lv_point_t myLocation;
	myLocation.x = 1000;
	myLocation.y = 500;
	/*Create an array for the points of the line*/

	/*Create style*/
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 8);
	lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_RED));
	lv_style_set_line_rounded(&style_line, true);

	lv_obj_add_flag(canvas, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(canvas, putmark_event_cb, LV_EVENT_SHORT_CLICKED, &myLocation);
	lv_obj_add_event_cb(canvas, drag_event_handler, LV_EVENT_PRESSING, NULL);
	lv_canvas_set_buffer(canvas, bmpInfo.bmpPixelArr, bmpInfo.bmpWidth, bmpInfo.bmpHeight, LV_COLOR_FORMAT_RGB888);

	bt_location = lv_btn_create(lv_scr_act());
	lv_obj_set_pos(bt_location, 675, 380);
	lv_obj_set_size(bt_location, 50, 50);
	label = lv_label_create(bt_location);
	lv_obj_add_event_cb(bt_location, location_cb, LV_EVENT_CLICKED, &myLocation);
	lv_label_set_text(label, LV_SYMBOL_GPS);
	lv_obj_set_align(label, LV_ALIGN_CENTER);

	//图片容器对象
	obj2 = lv_obj_create(lv_scr_act());
	lv_obj_set_style_bg_color(obj2, lv_color_hex(0xaaaaaa), LV_PART_MAIN);
	lv_obj_set_style_radius(obj2, 0, LV_PART_MAIN);
	lv_obj_set_size(obj2, 560, 420);
	lv_obj_set_y(obj2, 20);
	lv_obj_set_x(obj2, 20);
	lv_obj_set_style_pad_all(obj2, 0, 0);
	printf("obj2\n");
	//图片对象
	btn_close = lv_btn_create(obj2);
	lv_obj_set_pos(btn_close, 530, 10);
	lv_obj_set_size(btn_close, 20, 20);
	lv_win_add_button(obj2, LV_SYMBOL_CLOSE, 20);
    lv_obj_add_event_cb(btn_close, close_event_handler, LV_EVENT_CLICKED, obj2);

	printf("2\n");

	lv_obj_t* obj3 = lv_obj_create(obj2);
	canvas2 = lv_canvas_create(obj3);
	lv_obj_add_flag(canvas2, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(canvas2, drag_event_handler, LV_EVENT_PRESSING, NULL);
	lv_obj_set_pos(obj3, 10, 50);
	lv_obj_set_size(obj3, 535, 360);
	lv_obj_remove_style(obj3, NULL, LV_PART_SCROLLBAR);
	lv_obj_remove_flag(obj3, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_radius(obj3, 0, LV_PART_MAIN);
    lv_obj_add_flag(obj2,LV_OBJ_FLAG_HIDDEN); //隐藏图片容器对象


	while (1)
	{
		lv_timer_handler();
		usleep(5 * 1000);
	}
}
#endif

static void close_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_user_data(e);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN); //隐藏图片容器对象
    LV_LOG_USER("Button %d clicked", (int)lv_obj_get_index(obj));
}

static void putmark_event_cb(lv_event_t *e)
{
	// printf("putmark\n");
	lv_obj_t *obj = canvas;
	lv_point_t *mylocation = lv_event_get_user_data(e);

	static lv_obj_t *mark;
	if (mark == NULL)
	{
		mark = lv_label_create(obj);
		lv_label_set_text(mark, LV_SYMBOL_CHARGE);
	}
	lv_indev_t *indev = lv_indev_active();

	lv_point_t point;
	lv_indev_get_point(indev, &point);
	// printf("lv_obj_get_width:%d",lv_obj_get_width(mark));
	int32_t x = point.x - lv_obj_get_x_aligned(obj) - lv_obj_get_width(mark);
	int32_t y = point.y - lv_obj_get_y_aligned(obj) - lv_obj_get_height(mark);
	lv_obj_set_pos(mark, x, y);
	static lv_obj_t *line1;
	if (line1 == NULL)
	{
		line1 = lv_line_create(obj);
		lv_obj_add_style(line1, &style_line, 0);
	}
	my_point_t end_point = {x, y};
	my_point_t start_point = {mylocation->x, mylocation->y};

	int a = gps_getminDistPoint(start_point, gps_data, 48);
	int b = gps_getminDistPoint(end_point, gps_data, 48);
	my_minPath_t path = gps_graphDJST(graph, a, b);

	static lv_point_precise_t *line_points;
	if (line_points != NULL)
	{
		free(line_points);
	}
	line_points = (lv_point_precise_t *)malloc(sizeof(lv_point_precise_t) * (path.size + 2));

	line_points[0].x = mylocation->x, line_points[0].y = mylocation->y;
	for (int i = 0; i < path.size; i++)
	{
		line_points[i + 1].x = gps_data[path.path[i]].x, line_points[i + 1].y = gps_data[path.path[i]].y;
	}
	line_points[path.size + 1].x = x, line_points[path.size + 1].y = y;

	lv_line_set_points(line1, line_points, path.size + 2); /*Set the points*/

	// 点击显示图片
	printf("x:%d,y:%d\n", x, y);
	if (isPointInBox(earth_boundingBox,x, y))
	{
		lv_obj_remove_flag(obj2, LV_OBJ_FLAG_HIDDEN); //显示图片容器对象
		lv_canvas_set_buffer(canvas2, bmpInfo_earth.bmpPixelArr, bmpInfo_earth.bmpWidth, bmpInfo_earth.bmpHeight, LV_COLOR_FORMAT_RGB888);
	}
	else if (isPointInBox(sw_boundingBox,x, y))
	{
		lv_obj_remove_flag(obj2, LV_OBJ_FLAG_HIDDEN); //显示图片容器对象
		lv_canvas_set_buffer(canvas2, bmpInfo_sw.bmpPixelArr, bmpInfo_sw.bmpWidth, bmpInfo_sw.bmpHeight, LV_COLOR_FORMAT_RGB888);
	}
	else if (isPointInBox(network_boundingBox,x, y))
	{
		lv_obj_remove_flag(obj2, LV_OBJ_FLAG_HIDDEN); //显示图片容器对象
		lv_canvas_set_buffer(canvas2, bmpInfo_network.bmpPixelArr, bmpInfo_network.bmpWidth, bmpInfo_network.bmpHeight, LV_COLOR_FORMAT_RGB888);
	}
	else if (isPointInBox(mxyd_boundingBox,x, y))
	{
		lv_obj_remove_flag(obj2, LV_OBJ_FLAG_HIDDEN); //显示图片容器对象
		lv_canvas_set_buffer(canvas2, bmpInfo_mxyd.bmpPixelArr, bmpInfo_mxyd.bmpWidth, bmpInfo_mxyd.bmpHeight, LV_COLOR_FORMAT_RGB888);
	}
	else if (isPointInBox(library_boundingBox,x, y))
	{
		lv_obj_remove_flag(obj2, LV_OBJ_FLAG_HIDDEN); //显示图片容器对象
		lv_canvas_set_buffer(canvas2, bmpInfo_library.bmpPixelArr, bmpInfo_library.bmpWidth, bmpInfo_library.bmpHeight, LV_COLOR_FORMAT_RGB888);
	}
	else if (isPointInBox(lgl_boundingBox,x, y))
	{
		lv_obj_remove_flag(obj2, LV_OBJ_FLAG_HIDDEN); //显示图片容器对象
		lv_canvas_set_buffer(canvas2, bmpInfo_lgl.bmpPixelArr, bmpInfo_lgl.bmpWidth, bmpInfo_lgl.bmpHeight, LV_COLOR_FORMAT_RGB888);
	}
	else
	{
	    lv_obj_add_flag(obj2, LV_OBJ_FLAG_HIDDEN); //隐藏图片容器对象
	}

}

static void drag_event_handler(lv_event_t *e)
{
	lv_obj_t *obj = lv_event_get_target(e);

	lv_indev_t *indev = lv_indev_active();
	if (indev == NULL)
		return;

	lv_point_t vect;
	lv_indev_get_vect(indev, &vect);

	int32_t x = lv_obj_get_x_aligned(obj) + vect.x;
	int32_t y = lv_obj_get_y_aligned(obj) + vect.y;
	lv_obj_set_pos(obj, x, y);
}

void location_cb(lv_event_t *e)
{
	lv_point_t *point = lv_event_get_user_data(e);
	static lv_obj_t *label;
	if (label == NULL)
	{
		label = lv_label_create(canvas);
		lv_label_set_text(label, LV_SYMBOL_HOME);
	}
	lv_obj_set_pos(label, point->x, point->y);
	lv_obj_set_pos(canvas, (600 / 2) - point->x, (460 / 2) - point->y);
}

int main()
{
	main10();

	return 0;
}
