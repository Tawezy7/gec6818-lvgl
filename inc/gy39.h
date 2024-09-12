// Author: Tawezy7
// Date: 2024-09-09
#ifndef _INC_GY39_H_
#define _INC_GY39_H_

extern int GZ, WD, SD, QY, HB;

#ifdef __cplusplus
extern "C"
{
#endif

    int uart_init(const char *uart_name);
    void get_GZ(int fd);
    void get_other(int fd);

    /*...*/

#ifdef __cplusplus
}
#endif

#endif /* _INC_GY39_H_ */