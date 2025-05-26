#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include "LaserDetector.h"
#include "PathPlanner.h"
using namespace std;

//房间区块尺寸
const int ROOM_WIDTH = 12;
const int ROOM_LENGTH = 20;


//枚举类型RoomGrid列出了房间每块区域的状态
enum RoomGrid {
	GRID_UNKNOWN = 0,	//建图之前未知
	GRID_NORMAL = 1,		//没有障碍物
	GRID_CLEANED = 2,	//已经被打扫
	GRID_STATIC_OBSTACLE = -1,	//有静态障碍物（建图过程发现的障碍物）
	GRID_DYNAMIC_OBSTACLE = -2,	//有动态障碍物（打扫过程发现的障碍物）
	GRID_ROBOT = 3	//机器人
};

//清扫逻辑控制结构体
struct LogicController {
	int room[ROOM_WIDTH][ROOM_LENGTH]; //二维数组来表示房间区块的状态，并将所有区块初始化为未知状态GRID_UNKNOWN
	int robotX;	//机器人目前的x坐标
	int robotY;	//机器人目前的y坐标
};

// 定义房间区块不同状态对应的ANSI颜色编码
const string GRID_UNKNOWN_COLOR = "\033[47m  \033[0m";	//白色，表示未知区域
const string GRID_NORMAL_COLOR = "\033[46m  \033[0m";	//蓝 色，表示已建图后的可清扫区域
const string GRID_STATIC_OBSTACLE_COLOR = "\033[41m  \033[0m";	//红色，表示已建图后的静态障碍物区域
const string GRID_DYNAMIC_OBSTACLE_COLOR = "\033[45m  \033[0m";	//品红，表示清扫时发现的动态障碍物区域
const string GRID_CLEANED_COLOR = "\033[42m  \033[0m";	//绿色，表示已经清扫的区域
const string GRID_ROBOT_COLOR = "\033[43m  \033[0m";	//黄色，表示扫地机器人当前所在区域

unordered_map<int,string> colorMap;	//记录枚举对应颜色

/***
 * 设置控制台为虚拟终端序列模式，使其能够支持可控制的光标移动、彩色文本等功能
 * [1] https://learn.microsoft.com/zh-cn/windows/console/setconsolemode
 * [2] https://learn.microsoft.com/zh-cn/windows/console/console-virtual-terminal-sequences
 */
void config_screen() {
	// 获取控制台句柄
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		cout << "Error:" << GetLastError() << endl;
		return;
	}

	// 获取控制台当前模式
	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) {
		cout << "Error:" << GetLastError() << endl;
		return;
	}

	// 控制台当前模式中增加虚拟终端序列模式
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) {
		cout << "Error:" << GetLastError() << endl;
		return;
	}
}

/***
 * 将光标移动至指定位置
 * @param x 指定的 x 坐标
 * @param y 指定的 y 坐标
 */
void gotoxy(int x, int y) {
	//为了将房间区域不顶格显示，所以统一加上偏移量
	x += 2;
	y += 2;
	x *= 2;	//因为使用2个空格表示一个房间区域，所以要乘上2
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


/***
 * 打印某个时刻房间所有区域的状态
 * @param controller 目标扫地机器人控制器
 */
void print_room_map(const LogicController controller) {
	//Locked_Fog 25/5/6
	/***
	 * 思路：移动光标到初始位置，然后按controller内的内容打印图像
	 */
	for(int i=0;i<ROOM_WIDTH;i++){
		for(int j=0;j<ROOM_LENGTH;j++){
			gotoxy(j,i);		//注意：数组和坐标的关系应当为controller.room[y][x]
			if(j==controller.robotX && i==controller.robotY){
				cout<<colorMap[GRID_ROBOT];
			}else{
				cout<<colorMap[controller.room[i][j]];
			}
		}
	}
	return;
}
/***
 * 建图时绕过障碍物算法
 */
bool mappingBypass(int curPosX,int curPosY,int &aimPosX,int &aimPosY,LogicController &controller,bool &isHeadingRight){
	std::vector<std::pair<int, int>> shortestPath;
	find_shortest_path(*controller.room,ROOM_WIDTH,ROOM_LENGTH,curPosX,curPosY,aimPosX,aimPosY,shortestPath);

	/***
	 * 对路径进行检查
	 */
	bool isFinished=true;
	for(pair<int,int> pii : shortestPath){
		if(!mapping_detect(pii.first,pii.second,ROOM_WIDTH,ROOM_LENGTH)){
			curPosX = pii.first;curPosY=pii.second;//位置更新
			controller.robotX = curPosX;controller.robotY=curPosY;
			controller.room[curPosY][curPosX] = GRID_NORMAL;
			print_room_map(controller);
			Sleep(100);
		}else{
			controller.room[pii.second][pii.first] = GRID_STATIC_OBSTACLE;
			isFinished=false;
			//目标位置检查
			if(pii.first == aimPosX && pii.second == aimPosY){ //目标是障碍
				if(isHeadingRight){
					if(aimPosX >= ROOM_LENGTH -1){
						aimPosY ++;
						isHeadingRight = false;
					}else{
						aimPosX ++;
					}
				}else{
					if(aimPosX <= 0){
						aimPosY ++;
						isHeadingRight = true;
					}else{
						aimPosX --;
					}
				}
				if(aimPosY>=ROOM_WIDTH)return false;
			}
			break;
		}
	}
	if(!isFinished){
		return mappingBypass(curPosX,curPosY,aimPosX,aimPosY,controller,isHeadingRight);
	}else return true;
}

/***
 * 建图
 * @param controller 目标扫地机器人控制器
 */
void mapping(LogicController &controller) {
    //Locked_Fog 25/5/26
	/***
	 * 思路：行扫描
	 */

	bool isHeadingRight = true; //控制向右
	controller.robotX = 0; controller.robotY = 0; //机器人位置初始化
	while(true){
		int currentPositionX = controller.robotX, currentPositionY = controller.robotY; //当前位置
		int nextPositionX = controller.robotX, nextPositionY = controller.robotY; //下一位置
		
		/***
		 * 处理下一目标点位置
		 */
		if(isHeadingRight){
			if(currentPositionX >= ROOM_LENGTH-1){ //边界，换向
				nextPositionY++;
				isHeadingRight = false;
			}else{
				nextPositionX++;
			}
		}else{
			if(currentPositionX <= 0){ //边界，换向
				nextPositionY++;
				isHeadingRight = true;
			}else{
				nextPositionX--;
			}
		}

		/***
		 * 处理边界
		 */
		if(nextPositionY >= ROOM_WIDTH)break;

		/***
		 * 处理障碍物逻辑
		 */
		if(mapping_detect(nextPositionX,nextPositionY,ROOM_WIDTH,ROOM_LENGTH)){ // 返回true有障碍物
			controller.room[nextPositionY][nextPositionX] = GRID_STATIC_OBSTACLE; //标记障碍格
			/***
			 * 更新目标，处理边界和方向
			 */
			if(isHeadingRight){
				if(nextPositionX >= ROOM_LENGTH-1){
					nextPositionY++;
					isHeadingRight=false;
				}else{
					nextPositionX++;
				}
			}else{
				if(nextPositionX <= 0){
					nextPositionY++;
					isHeadingRight=true;
				}else{
					nextPositionX--;
				}
			}
			if(nextPositionY>=ROOM_WIDTH)break;
			if(!mappingBypass(currentPositionX,currentPositionY,nextPositionX,nextPositionY,controller,isHeadingRight))break;
			
		}else{
			controller.robotX = nextPositionX; controller.robotY = nextPositionY; //没有障碍物，直接通过
			controller.room[controller.robotY][controller.robotX] = GRID_NORMAL; //标记正常格
			print_room_map(controller);
			Sleep(100);
		}
		
	}

	/***
	 * 回家
	 */
	std::vector<std::pair<int, int>> shortestPath;
	find_shortest_path(*controller.room,ROOM_WIDTH,ROOM_LENGTH,controller.robotX,controller.robotY,0,0,shortestPath);
	for(pair<int,int>pii : shortestPath){
		controller.robotX = pii.first;
		controller.robotY = pii.second;
		print_room_map(controller);
	}
}


//清扫
/***
 * 清扫
 * @param controller 目标扫地机器人控制器
 */
void scanning_sweep(LogicController &controller) {
	//TODO
}

/***
 * 定点清扫
 * @param x 目标地点的 x 坐标
 * @param y 目标地点的 y 坐标
 * @param controller 目标扫地机器人控制器
 */
// void target_sweep(int x,int y,LogicController &controller) {
	//TODO
// }

/***
 * 控制机器人回到起点
 * @param controller 目标扫地机器人控制器
 */
void goto_start_point(LogicController &controller) {
	vector<pair<int, int>> path;
	find_shortest_path(*controller.room, ROOM_WIDTH, ROOM_LENGTH, controller.robotX, controller.robotY, 0, 0, path);
	for (unsigned int i = 0; i < path.size(); i++) {
		controller.robotX = path[i].first;
		controller.robotY = path[i].second;
		print_room_map(controller);
	}
}

/***
 * 打印提示信息，完成打印后将显示“输入任意键继续...”
 * @param msg 预计将要打印的信息
 */
void step_over(string msg) {
	cout << endl << msg << endl;
	cout << "输入任意键继续..." << endl;
	getch();	//等待键盘任意键按下，不需要按回车键确认输入
	system("cls");	//清除控制台界面上所有的字符，并将光标移动到左上方
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	//配置屏幕，使其支持光标位置控制和彩色字体
	config_screen();

	//初始化colorMap
	colorMap[GRID_UNKNOWN] = GRID_UNKNOWN_COLOR;
	colorMap[GRID_NORMAL] = GRID_NORMAL_COLOR;
	colorMap[GRID_CLEANED] = GRID_CLEANED_COLOR;
	colorMap[GRID_STATIC_OBSTACLE] = GRID_STATIC_OBSTACLE_COLOR;
	colorMap[GRID_DYNAMIC_OBSTACLE] = GRID_DYNAMIC_OBSTACLE_COLOR;
	colorMap[GRID_ROBOT] = GRID_ROBOT_COLOR;

	//创建逻辑控制器
	LogicController controller;
	//将房间的所有区块初始化为未知状态
	for (int i = 0; i < ROOM_WIDTH; i++)
		for (int j = 0; j < ROOM_LENGTH; j++)
			controller.room[i][j] = GRID_UNKNOWN;
	//设置机器人的初始位置为左上方
	controller.robotX = 0;
	controller.robotY = 0;
	controller.room[0][0] = GRID_NORMAL;

	//test
	print_room_map(controller);

	//建图
	mapping(controller);
	goto_start_point(controller);
	step_over("建图完成！");

	//清扫
	scanning_sweep(controller);
	goto_start_point(controller);
	step_over("清扫完成！");

	//定点清扫（选做）
	//target_sweep(controller);
	//step_over("定点清扫完成！");

	//回到起点
	goto_start_point(controller);
	step_over("所有过程结束！");

	return 0;
}
