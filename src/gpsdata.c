#include "graph.h"
#include "gpsdata.h"
#include <math.h>
#include <limits.h>

const my_point_t gps_data[48] = {
    {2, 240},
    {132, 652},
    {4, 740},
    {288, 760},
    {290, 990},
    {560, 750},
    {608, 750},
    {592, 970},
    {629, 960},
    {605, 1070},
    {331, 1085},
    {339, 1020},
    {642, 1090},
    {1071, 1027},
    {1049, 795},
    {1029, 760},
    {995, 755},
    {749, 787},
    {728, 729},
    {825, 710},
    {875, 656},
    {893, 597},
    {917, 282},
    {843, 140},
    {1350, 5},
    {1332, 289},
    {1324, 404},
    {1300, 467},
    {1285, 505},
    {1030, 420},
    {1250, 635},
    {1225, 705},
    {1335, 635},
    {1545, 640},
    {1597, 579},
    {1595, 465},
    {1160, 910},
    {1330, 910},
    {1550, 910},
    {1545, 910},
    {1140, 1020},
    {1595, 632},
    {1100, 285},
    {1084, 239},
    {1094, 314},
    {950, 322},
    {943, 278},
    {942, 245}};
int distance(int index1, int index2)
{
    return sqrt(pow(gps_data[index1].x - gps_data[index2].x, 2) + pow(gps_data[index1].y - gps_data[index2].y, 2));
}

#define GRAPH_MAP(index1, index2) graph->adj[index1][index2] = distance(index1, index2), graph->adj[index2][index1] = distance(index1, index2)
void gps_graphVRInput(Graph *graph)
{
    graph->Vnum = 48;

    for (int i = 0; i < graph->Vnum; i++)
    {
        graph->V[i] = i;
    }

    GRAPH_MAP(0, 1);
    GRAPH_MAP(1, 2);
    GRAPH_MAP(1, 3);
    GRAPH_MAP(3, 4);
    GRAPH_MAP(3, 5);
    GRAPH_MAP(4, 11);
    GRAPH_MAP(11, 10);
    GRAPH_MAP(5, 6);
    GRAPH_MAP(5, 7);
    GRAPH_MAP(7, 8);
    GRAPH_MAP(7, 9);
    GRAPH_MAP(9, 10);
    GRAPH_MAP(6, 8);
    GRAPH_MAP(6, 18);
    GRAPH_MAP(8, 12);
    GRAPH_MAP(12, 13);
    GRAPH_MAP(13, 14);
    GRAPH_MAP(14, 15);
    GRAPH_MAP(15, 16);
    GRAPH_MAP(16, 17);
    GRAPH_MAP(17, 18);
    GRAPH_MAP(18, 19);
    GRAPH_MAP(19, 20);
    GRAPH_MAP(20, 21);
    GRAPH_MAP(21, 22);
    GRAPH_MAP(21, 33);
    GRAPH_MAP(22, 23);
    GRAPH_MAP(23, 24);
    GRAPH_MAP(24, 25);
    GRAPH_MAP(25, 26);
    GRAPH_MAP(26, 27);
    GRAPH_MAP(27, 28);
    GRAPH_MAP(27, 35);
    GRAPH_MAP(28, 29);
    GRAPH_MAP(28, 30);
    GRAPH_MAP(30, 31);
    GRAPH_MAP(30, 32);
    GRAPH_MAP(31, 21);
    GRAPH_MAP(31, 36);
    GRAPH_MAP(32, 33);
    GRAPH_MAP(32, 37);
    GRAPH_MAP(33, 41);
    GRAPH_MAP(33, 39);
    GRAPH_MAP(41, 34);
    GRAPH_MAP(34, 35);
    GRAPH_MAP(36, 37);
    GRAPH_MAP(36, 40);
    GRAPH_MAP(40, 13);
    GRAPH_MAP(37, 38);
    GRAPH_MAP(38, 39);
    GRAPH_MAP(39, 40);
    GRAPH_MAP(25, 42);
    GRAPH_MAP(42, 43);
    GRAPH_MAP(42, 44);
    GRAPH_MAP(44, 45);
    GRAPH_MAP(45, 46);
    GRAPH_MAP(46, 47);
    GRAPH_MAP(43, 47);
    GRAPH_MAP(46, 22);
    
}

int gps_getminDistPoint(my_point_t point, my_point_t* pointlist, int size)
{
    int res;
    int dif = INT_MAX;
    for (int i = 0; i < size; i++)
    {
        int temp = sqrt(pow(point.x - pointlist[i].x,2) + pow(point.y - pointlist[i].y,2));
        if (temp < dif)
        {
            dif = temp;
            res = i;
        }
    }
    return res;
}
