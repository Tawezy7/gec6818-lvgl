// Author: Tawezy7
// Date: 2024-09-05
#ifndef _INC_TOUCH_H_
#define _INC_TOUCH_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        Release,
        Press
    } TouchState;

    int touchInit(void);
    void touchUninit(void);
    int touchGetXY(int *x, int *y);
    int touchGetXYRelease(int *x, int *y);
    TouchState touchPress(void);
    int touchFx(int x1, int y1, int x2, int y2);
    /*...*/

#ifdef __cplusplus
}
#endif

#endif /* _INC_TOUCH_H_ */