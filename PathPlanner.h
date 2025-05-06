#ifndef PATH_PLANNER
#define PATH_PLANNER
#include <vector>


/***
 * 计算最短路径，使用dijkstra算法
 * @param map 储存地图的二维数组
 * @param width, length 房间长、宽
 * @param start_x, start_y 起点坐标
 * @param end_x, end_y 终点坐标
 * @param path 以 verctor<pair<int,int>> 类型存放的路径
 * @param
 */
void find_shortest_path(const int *map, int width, int length, int start_x, int start_y, int end_x, int end_y,
                        std::vector<std::pair<int, int>> &path);

#endif