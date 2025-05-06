#ifndef LASER_DETECTOR
#define LASER_DETECTOR

//判断(x,y)区域是否有静态障碍物。其中的width和length分别表示房间的宽度和长度。
//静态障碍物生成的位置是随机的，数量固定为10个。
bool mapping_detect(int x, int y, int width, int length);

//判断(x,y)区域是否有动态障碍物。其中的width和length分别表示房间的宽度和长度。
//动态障碍物生成的位置是随机的，数量固定为5个。
bool sweeping_detect(int x, int y, int width, int length);

//判断(x,y)区域原有的动态障碍物是否消失。
bool targeted_detect(int x, int y);

#endif
