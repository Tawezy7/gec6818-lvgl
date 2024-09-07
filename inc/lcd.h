#ifndef LCD_H
#define LCD_H

extern int lcdFd;
extern unsigned int *plcd;

void lcdInit();

void lcdUninit();

// 在屏幕上x y 坐标位置画color
void lcdDrawPoint(int x, int y, unsigned int color);

// 刷屏
void lcdFflush(unsigned int color);

// x,y起始坐标 width 宽度 length长度 color颜色
void lcdDrawTran(int x, int y, int width, int length, unsigned int color);
void lcdDrawFont(int x, int y); // 画字体

#endif /* lcd.h */