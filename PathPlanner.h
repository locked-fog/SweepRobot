#ifndef PATH_PLANNER
#define PATH_PLANNER
#include <vector>


/***
 * �������·����ʹ��dijkstra�㷨
 * @param map �����ͼ�Ķ�ά����
 * @param width, length ���䳤����
 * @param start_x, start_y �������
 * @param end_x, end_y �յ�����
 * @param path �� verctor<pair<int,int>> ���ʹ�ŵ�·��
 * @param
 */
void find_shortest_path(const int *map, int width, int length, int start_x, int start_y, int end_x, int end_y,
                        std::vector<std::pair<int, int>> &path);

#endif