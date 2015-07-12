#include "compute.h"
#include "mpi.h"
#include <stdlib.h>
#include <algorithm>
#include <iomanip>

using std::ios;
using std::cout;
using std::endl;
using std::vector;

// 每个rank id 至少要处理10个点, 才进行消息通信
#define MIN_POINTS_OF_EVERY_ID 10

int main(int argc, char *argv[])
{
    int myid = 0; // the rank id of this process
    int np = 0;   // the number of processes
    int n = 0;    // the number of points
    int multiple = 0;  // n / np
    int remainder = 0; // n % np
    if (argc != 2) 
    {
        std::cerr << "Usage: ./hierachical_clustering.o <number of points>" << std::endl;
        exit(-1);
    }
    n = atoi(argv[1]);
    if (n <= 0)
    {
        std::cerr << "<numer of points>  must be positive." << std::endl;
        exit(-1);
    }
    cout.precision(4);
    cout.setf(ios::fixed);
    vector<Point> points; // the points vector
    Pair my_pair;
    // 随机生成点
    random_generate_points(points, n);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid); 
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    if (np <= 0)
    {
        MPI_Finalize();
        std::cerr << "<numer of process>  must be positive." << std::endl;
        exit(-1);
    }
    while (points.size() > np * MIN_POINTS_OF_EVERY_ID) 
    {
        // MARK: There hanpends a big bug when I put these two line out of the while cluase !!!
        multiple = points.size() / np;
        remainder = points.size() % np;
        sort(points.begin(), points.end(), compareX); 
        vector<Point>::iterator my_begin = points.begin() + myid * multiple;
        if (myid < remainder)
        {
            my_begin += myid;
        }
        else 
        {
            my_begin += remainder;
        }
        vector<Point>::iterator my_end = my_begin + multiple;
        if (myid < remainder)
        {
            my_end += 1;
        }
        find_shortest_pair(my_begin, my_end, my_pair);
        double pair_2_arr[5];   // 将my_pair存储在pair_2_arr中
        pair_2_arr[0] = my_pair.first.x;
        pair_2_arr[1] = my_pair.first.y;
        pair_2_arr[2] = my_pair.second.x;
        pair_2_arr[3] = my_pair.second.y;
        pair_2_arr[4] = my_pair.distance;

        // 将我计算的结果传递给其他rank id
        for (int i = 0; i < np; i++)
        {
            if (i != myid)
            {
                MPI_Send((void *)pair_2_arr, 5, MPI_DOUBLE, i, 99, MPI_COMM_WORLD);
            }
        }

        // 获得其他rank id 计算的结果
        double buff[5];
        MPI_Status status;
        int receive_cnt = np - 1;
        while (receive_cnt)
        {
            memset((void *)buff, 0, 5 * sizeof(double));
            MPI_Recv((void *)buff, 5, MPI_DOUBLE, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
            if (buff[4] < my_pair.distance)
            {
                my_pair.first.x = buff[0];
                my_pair.first.y = buff[1];
                my_pair.second.x = buff[2];
                my_pair.second.y = buff[3];
                my_pair.distance = buff[4];
            }

            receive_cnt--;
        }

        // 查看边线部分是否有最短距离点对, 一共有np-1个边线
        for (int i = 0; i < np - 1; i++)
        {
            Pair cpy_of_my_pair = my_pair;
            vector<Point>::iterator inner_begin = points.begin() + i * multiple;
            if (i < remainder)
            {
                inner_begin += i;
            }
            else 
            {
                inner_begin += remainder;
            }
            vector<Point>::iterator inner_mid = inner_begin + multiple;
            if (i < remainder)
            {
                inner_mid += 1;
            }
            vector<Point>::iterator inner_end = points.begin() + (i + 1) * multiple;
            if ((i + 1) < remainder)
            {
                inner_end += (i + 1);
            }
            else 
            {
                inner_end += remainder;
            }
            shortest_of_boundary(inner_begin, inner_end, inner_mid, cpy_of_my_pair);
            if (cpy_of_my_pair.distance > 0 && cpy_of_my_pair.distance < my_pair.distance)
            {
                my_pair = cpy_of_my_pair;
            }
        }

        Point new_point;
        new_point.x = (my_pair.first.x + my_pair.second.x) / 2;
        new_point.y = (my_pair.first.y + my_pair.second.y) / 2;
        if (myid == 0)
        {
            merge_output(my_pair.first, my_pair.second, new_point, std::cout); 
        }
        remove(points, my_pair.first);
        remove(points, my_pair.second);
        points.push_back(new_point);
    }
    clustering(points, myid);

    MPI_Finalize();

    return 0;
}
