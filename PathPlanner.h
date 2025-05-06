#ifndef PATH_PLANNER
#define PATH_PLANNER
#include <vector>

//Ѱ��(start_x, start_y)����(end_x, end_y)��������·�������������������£�
//map: ������������Ķ�ά�����׵�ַ
//width: ������
//length: ���䳤��
//path: �ҵ������·������pair����ʽ���·���ϵ�ÿ����������꣬���������յ㡣����յ㲻�ɴpath.size()Ϊ0��
//Dijkstra�㷨���������https://blog.csdn.net/qq_35644234/article/details/60870719
void find_shortest_path(const int *map, int width, int length, int start_x, int start_y, int end_x, int end_y,
                        std::vector<std::pair<int, int>> &path);

#endif