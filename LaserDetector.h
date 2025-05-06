#ifndef LASER_DETECTOR
#define LASER_DETECTOR


/***
 * 判断指定位置是否存在静态障碍物
 * @param x, y 目标位置的坐标
 * @param width, length 房间长宽
 * 注：数量固定为10
 */
bool mapping_detect(int x, int y, int width, int length);


/***
 * 判断指定位置是否存在动态障碍物
 * @param x, y 目标位置的坐标
 * @param width, length 房间长宽
 * 注：数量固定为5
 */
bool sweeping_detect(int x, int y, int width, int length);

/***
 * 判断目标位置的障碍物是否消失
 * @param x, y 目标位置的坐标
 */
bool targeted_detect(int x, int y);

#endif
