/**
 * compute.cpp
 * 一些计算函数实现文件
 * Author: Chen Kang <chenkang02@baidu.com>
 * Date: 2015/07/08
 * Last Modify: 2015/07/08
 */

#include "compute.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

Point NULL_POINT = {MAX_X + 1, MAX_Y + 1};
Pair NULL_PAIR = {NULL_POINT, NULL_POINT, LONG_MAX};

void clustering(vector<Point> &points, int &myid)
{
    Pair pair;
    while (points.size() > 1)
    {
        find_shortest_pair(points.begin(), points.end(), pair);
        Point new_point;
        new_point.x = (pair.first.x + pair.second.x) / 2;  
        new_point.y = (pair.first.y + pair.second.y) / 2;
        if (myid == 0)
        {
            merge_output(pair.first, pair.second, new_point, cout); 
        }
        // 在points中删除pair中的点
        remove(points, pair.first);
        remove(points, pair.second);
        // 将newPair添加到points中
        points.push_back(new_point); 
    }
}

void find_shortest_pair(vector<Point>::iterator begin, vector<Point>::iterator end, Pair &shortest_pair)
{
    shortest_pair = NULL_PAIR;
    if (end - begin <= 1) 
    {
        shortest_pair = NULL_PAIR;
        return;
    }
    if (end - begin == 2)
    {
        shortest_pair.first = *begin;
        shortest_pair.second = *(begin + 1);
        shortest_pair.distance = distance(*begin, *(begin + 1));
        return;
    }
    if (end - begin == 3)
    {
        Pair tmp_pair;
        tmp_pair.first = *begin;
        tmp_pair.second = *(begin + 1);
        tmp_pair.distance = distance(*begin, *(begin + 1));
        if (distance(*begin, *(begin + 2)) < tmp_pair.distance)
        {
            tmp_pair.first = *(begin);
            tmp_pair.second = *(begin + 2);
            tmp_pair.distance = distance(*begin, *(begin + 2));
        }
        if (distance(*(begin+1), *(begin + 2)) < tmp_pair.distance)
        {
            tmp_pair.first = *(begin + 1);
            tmp_pair.second = *(begin + 2);
            tmp_pair.distance = distance(*(begin+1), *(begin + 2));
        }
        shortest_pair = tmp_pair;

        return;
    }

    // 将points按照X轴排序, 由于调用函数的时候已经排好序了，这里不用再排了
    //sort(begin, end, compareX); 
    vector<Point>::iterator mid = begin + (end - begin) / 2;
    
    // 递归的方式分别找到左边和右边的最短距离点对
    Pair left_pair;
    Pair right_pair;
    find_shortest_pair(begin, mid, left_pair);
    find_shortest_pair(mid, end, right_pair);
    if (left_pair == NULL_PAIR && !(right_pair == NULL_PAIR) && right_pair.distance > 0)
    {
        shortest_pair = right_pair;
    }
    else if (right_pair == NULL_PAIR && !(left_pair == NULL_PAIR) && left_pair.distance > 0) 
    {
        shortest_pair = left_pair;
    }
    else if ( !(left_pair == NULL_PAIR) && !(right_pair == NULL_PAIR))
    {
        if (left_pair.distance > 0 && right_pair.distance == 0)
        {
            shortest_pair = left_pair;
        }
        else if (left_pair.distance == 0 && right_pair.distance > 0)
        {
            shortest_pair = right_pair;
        }
        else if (left_pair.distance > 0 && right_pair.distance > 0)
        {
            shortest_pair = left_pair.distance < right_pair.distance ? left_pair : right_pair;
        }
            
    }

    /* 找到中位线附近的最短距离点对 */
    Pair cpy_of_current_pair = shortest_pair;
    shortest_of_boundary(begin, end, mid, cpy_of_current_pair);
    if (cpy_of_current_pair.distance > 0 && cpy_of_current_pair.distance < shortest_pair.distance)
    {
        shortest_pair = cpy_of_current_pair;
    }
}

void shortest_of_boundary(vector<Point>::iterator begin, vector<Point>::iterator end, vector<Point>::iterator mid, Pair &current_pair)
{
    // 1. 用两个临时vector分别保存在中位线左边和右边距离中位线小于等于current_pair.distance的点
    vector<Point> left_points, right_points;
    for (vector<Point>::iterator iter = begin; iter != mid; iter++)
    {
        if (mid->x - iter->x <= current_pair.distance)
        {
            left_points.push_back(*iter);
        }
        else 
        {
            break;  // 由于points已经按照x轴排好序了
        }
    }
    for (vector<Point>::iterator iter = mid; iter != end; iter++)
    {
        if (iter->x - mid->x <= current_pair.distance)
        {
            right_points.push_back(*iter);
        }
        else 
        {
            break;  // 由于points已经按照x轴排好序了
        }
    }

    if (left_points.size() < 0 || right_points.size() < 0)
    {
        return;
    }
    
    // 2. 将points按照Y轴进行排序
    sort(left_points.begin(), left_points.end(), compareY);
    sort(right_points.begin(), right_points.end(), compareY);

    // 3. 遍历left_points中的每个点，计算它与right_points中合适位置的点的距离，如果 < current_pair.distacne，则更新current_pair
    for (vector<Point>::iterator iter = left_points.begin(); iter != left_points.end(); iter++)
    {
        // 在right_points中找到y坐标大于等于iter->y的最小点
        vector<Point>::iterator min_ge_iter = find_min_ge(right_points.begin(), right_points.end(), iter->y, Y_COORD);
        if (min_ge_iter != right_points.end())
        {
            for (vector<Point>::iterator inner_iter = min_ge_iter; inner_iter != right_points.end(); inner_iter++)
            {
                if (inner_iter->y - iter->y > current_pair.distance) 
                {
                    break;
                }
                else
                {
                    double inner_distance = distance(*iter, *inner_iter);
                    if (current_pair == NULL_PAIR)
                    {
                        current_pair.first = *iter;
                        current_pair.second = *inner_iter;
                        current_pair.distance = inner_distance;
                    }
                    else if (inner_distance < current_pair.distance)
                    {
                        current_pair.first = *iter;
                        current_pair.second = *inner_iter;
                        current_pair.distance = inner_distance;
                    }
                }
            }
        }
        // 在right_points找到y坐标小于等于iter->x的最大点
        vector<Point>::iterator max_le_iter = find_max_le(right_points.begin(), right_points.end(), iter->y, Y_COORD);
        if (max_le_iter != right_points.end())
        {
            for (vector<Point>::iterator inner_iter2 = max_le_iter; inner_iter2 >= right_points.begin(); inner_iter2--)
            {
                if (iter->y - inner_iter2->y > current_pair.distance)
                {
                    break;
                }
                else
                {
                    double inner_distance2 = distance(*iter, *inner_iter2);
                    if (current_pair == NULL_PAIR )
                    {
                        current_pair.first = *iter;
                        current_pair.second = *inner_iter2;
                        current_pair.distance = inner_distance2;
                    }
                    else if (inner_distance2 < current_pair.distance)
                    {
                        current_pair.first = *iter;
                        current_pair.second = *inner_iter2;
                        current_pair.distance = inner_distance2;
                    }
                }
            }
        }
    }
}

bool compareX(const Point& first, const Point& second)
{
    return first.x < second.x;
}

bool compareY(const Point& first, const Point& second)
{
    return first.y < second.y;
}

void remove(vector<Point> &points, Point &point)
{
   vector<Point>::iterator iter = find(points.begin(), points.end(), point); 
   if (iter != points.end())
   {
       points.erase(iter);
   }
}

double distance(Point &first, Point &second)
{
    return sqrt((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y));
}

void merge_output(Point &first, Point &second, Point &newPoint, std::ostream &out)
{
    out << "Merge <" << first.x << ", " << first.y << "> and <";
    out << second.x << "," << second.y << "> to <" << newPoint.x << ", ";
    out << newPoint.y << ">" << std::endl;
}

vector<Point>::iterator find_min_ge(vector<Point>::iterator begin, vector<Point>::iterator end, double x, int coord)
{
    vector<Point>::iterator middle;
    if (begin >= end) // no elements between [begin, end)
    {
        return end;
    }
    if (X_COORD == coord)
    {
        if (begin == end -1)  // one element between [begin, end)
        {
            if (begin->x >= x)
            {
                return begin;
            }
            else 
            {
                return end;
            }
        }
        vector<Point>::iterator left = begin;
        vector<Point>::iterator right = end - 1;
        while (left <= right)
        {
            middle = left + (right - left) / 2;
            if ((middle->x == x) || (middle->x > x && ((middle - 1) < left || (middle - 1)->x < x))) 
            {
                return middle; 
            }
            else if (middle->x > x)
            {
                right = middle - 1; 
            }
            else if (middle->x < x)
            {
                left  = middle + 1;
            }
        }
    }
    else if (Y_COORD == coord)
    {
        if (begin == end -1)  // one element between [begin, end)
        {
            if (begin->y >= x)
            {
                return begin;
            }
            else 
            {
                return end;
            }
        }
        vector<Point>::iterator left = begin;
        vector<Point>::iterator right = end - 1;
        while (left <= right)
        {
            middle = left + (right - left) / 2;
            if ((middle->y == x) || (middle->y > x && ((middle - 1) < left || (middle - 1)->y < x))) 
            {
                return middle; 
            }
            else if (middle->y > x)
            {
                right = middle - 1; 
            }
            else if (middle->y < x)
            {
                left  = middle + 1;
            }
        }
    }
    
    return end;
}

vector<Point>::iterator find_max_le(vector<Point>::iterator begin, vector<Point>::iterator end, double x, int coord)
{
    vector<Point>::iterator middle;
    if (begin >= end) // no elements between [begin, end)
    {
        return end;
    }
    if (X_COORD == coord)
    {
        if (begin == end -1)  // one element between [begin, end)
        {
            if (begin->x <= x)
            {
                return begin;
            }
            else 
            {
                return end;
            }
        }
        vector<Point>::iterator left = begin;
        vector<Point>::iterator right = end - 1;
        while (left <= right)
        {
            middle = left + (right - left) / 2;
            if ((middle->x == x) || (middle->x < x && ((middle - 1) < left || (middle - 1)->x > x))) 
            {
                return middle; 
            }
            else if (middle->x > x)
            {
                right = middle - 1; 
            }
            else if (middle->x < x)
            {
                left  = middle + 1;
            }
        }

    }
    else if (Y_COORD == coord)
    {
        if (begin == end -1)  // one element between [begin, end)
        {
            if (begin->y <= x)
            {
                return begin;
            }
            else 
            {
                return end;
            }
        }
        vector<Point>::iterator left = begin;
        vector<Point>::iterator right = end - 1;
        while (left <= right)
        {
            middle = left + (right - left) / 2;
            if ((middle->y == x) || (middle->y < x && ((middle - 1) < left || (middle - 1)->y > x))) 
            {
                return middle; 
            }
            else if (middle->y > x)
            {
                right = middle - 1; 
            }
            else if (middle->y < x)
            {
                left  = middle + 1;
            }
        }
    }
    
    return end;
}

void random_generate_points(vector<Point> &points, int n)
{
    points.clear();
    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        Point point;
        point.x = static_cast<double> (rand()) / (static_cast<double> (RAND_MAX / MAX_X));
        point.y = static_cast<double> (rand()) / (static_cast<double> (RAND_MAX / MAX_Y));
        points.push_back(point);
    }
}

