// Author: Tawezy7
// Date: 2024-09-18
#ifndef _INC_GPSDATA_H_
#define _INC_GPSDATA_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
    int32_t latitude;
    int32_t longitude;
} GPSData;
void gps_graphVRInput(Graph *graph);
    /*...*/

#ifdef __cplusplus
}
#endif

#endif /* _INC_GPSDATA_H_ */