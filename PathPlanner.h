#ifndef PATH_PLANNER
#define PATH_PLANNER
#include <vector>

//寻找(start_x, start_y)区域到(end_x, end_y)区域的最短路径。其它参数意义如下：
//map: 房间网格区域的二维数组首地址
//width: 房间宽度
//length: 房间长度
//path: 找到的最短路径，以pair的形式存放路径上的每个区域的坐标，包含起点和终点。如果终点不可达，path.size()为0。
//Dijkstra算法详情请见：https://blog.csdn.net/qq_35644234/article/details/60870719
void find_shortest_path(const int *map, int width, int length, int start_x, int start_y, int end_x, int end_y,
                        std::vector<std::pair<int, int>> &path);

#endif