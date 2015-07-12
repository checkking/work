/** 
 * point.h
 * 坐标点头文件，定义坐标点结构体 
 * Author: Chen Kang (chenkang02@baidu.com)
 * Date: 2015/07/08
 * Last Modify: 2015/07/08
 */
#ifndef _POINT_H_
#define _POINT_H_

#include <cmath>

#define MAX_X 10000000  // x 坐标的最大值
#define MAX_Y 10000000  // y 坐标的最大值

#define EQUAL_PRECISION 0.00001

typedef struct _Point {
    double x;
    double y;
    bool operator==(const struct _Point& point)
    {
        return (fabs(x - point.x) < EQUAL_PRECISION) && (fabs(y - point.y)) < EQUAL_PRECISION;
    }
    struct _Point& operator=(const struct _Point& point)
    {
        x = point.x;
        y = point.y;
        return *this;
    }
} Point;

#define X_COORD 0   // 标记X轴
#define Y_COORD 1   // 标记Y轴

#endif
