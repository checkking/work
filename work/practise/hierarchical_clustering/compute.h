/**
 * compute.h
 * 一些计算函数
 * Author: Chen Kang <chenkang02@baidu.com>
 * Date: 2015/07/08
 * Last Modify: 2015/07/08
 */

#ifndef INF_CHENKANG02_PRACTISE_HIERARCHICAL_CLUSTERING_COMPUTE_H
#define INF_CHENKANG02_PRACTISE_HIERARCHICAL_CLUSTERING_COMPUTE_H

#include "point.h"
#include "pair.h"
#include <vector>
#include <iostream>
#include <math.h>

/** 
 * 对集合中的点进行层次聚类，直到最后只剩一个点为止
 * input: points, 点集合 begin: 集合开始的迭代器 end: 集合结束的迭代器, myid: rank id
 * output: none
 */
void clustering(std::vector<Point> &points, int &myid);

/**
 * 找到集合中最短距离点对
 * input: begin, end
 * output: shortest_pair, 找到的最短距离点对
 */
void find_shortest_pair(std::vector<Point>::iterator begin, std::vector<Point>::iterator end, Pair &shortest_pair);

/**
 * 按X轴进行排序的比较函数
 */
bool compareX(const Point& first, const Point& second);

/**
 * 按Y轴进行排序的比较函数
 */
bool compareY(const Point& first, const Point& second);

/**
  * 在points 删除点point
  */
void remove(std::vector<Point> &points, Point &point);

/**
 * 将归并信息输出到流out中
 */
void merge_output(Point &first, Point &second, Point &newPoint, std::ostream &out);

/**
 * 计算两点间的距离
 */
double distance(Point &first, Point &second);

/**
 * 用二分查找法计算某个坐标大于等于x的最小点
 * 其中coord用来标注坐标轴，X_COORD: X轴，Y_COORD: Y轴
 */
std::vector<Point>::iterator find_min_ge(std::vector<Point>::iterator begin, std::vector<Point>::iterator end, double x, int coord);

/**
 * 用二分查找发计算某个坐标小于等于x的最大点
 */
std::vector<Point>::iterator find_max_le(std::vector<Point>::iterator begin, std::vector<Point>::iterator end, double x, int coord);

/**
 * 查找分界线附近的最短距离点对, 前提是vector已经按照x轴进行排序过
 * 输入: begin, end, mid: 分界线iterator
 * 输出：current_pair: Pair
 */
void shortest_of_boundary(std::vector<Point>::iterator begin, std::vector<Point>::iterator end, std::vector<Point>::iterator mid, Pair &current_pair);

/**
 * 随机生成n个点
 */
void random_generate_points(std::vector<Point> &points, int n);

#endif
