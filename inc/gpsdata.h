// Author: Tawezy7
// Date: 2024-09-18
#ifndef _INC_GPSDATA_H_
#define _INC_GPSDATA_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
    int32_t x;
    int32_t y;
} my_point_t;

extern const my_point_t gps_data[48];

void gps_graphVRInput(Graph *graph);
int gps_getminDistPoint(my_point_t point, my_point_t* pointlist, int size);
    /*...*/

#ifdef __cplusplus
}
#endif

#endif /* _INC_GPSDATA_H_ */