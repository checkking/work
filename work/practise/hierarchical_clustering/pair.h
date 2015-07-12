/**
 * pair.h
 * 定义最短距离点对
 * Author: Chen Kang <chenkang02@baidu.com>
 * Date: 2015/07/08
 * Last Modify: 2015/07/08
 */

#ifndef INF_CHENKANG02_PRACTISE_HIERARCHICAL_CLUSTERING_PAIR_H_
#define INF_CHENKANG02_PRACTISE_HIERARCHICAL_CLUSTERING_PAIR_H_

#include "point.h"

typedef struct _Pair {
    Point first;
    Point second;
    double distance;
    bool operator==(const struct _Pair& pair)
    {
        return first ==  pair.first && second == pair.second && fabs(distance - pair.distance) < EQUAL_PRECISION;
    }
    struct _Pair& operator=(const struct _Pair& pair)
    {
        first = pair.first;
        second = pair.second;
        distance = pair.distance;
        return *this;
    }
}Pair;

#endif
