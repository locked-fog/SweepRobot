#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include "LaserDetector.h"
#include "PathPlanner.h"
using namespace std;

//��������ߴ�
const int ROOM_WIDTH = 12;
const int ROOM_LENGTH = 20;


//ö������RoomGrid�г��˷���ÿ�������״̬
enum RoomGrid {
	GRID_UNKNOWN = 0,	//��ͼ֮ǰδ֪
	GRID_NORMAL = 1,		//û���ϰ���
	GRID_CLEANED = 2,	//�Ѿ�����ɨ
	GRID_STATIC_OBSTACLE = -1,	//�о�̬�ϰ����ͼ���̷��ֵ��ϰ��
	GRID_DYNAMIC_OBSTACLE = -2,	//�ж�̬�ϰ����ɨ���̷��ֵ��ϰ��
	GRID_ROBOT = 3	//������
};

//��ɨ�߼����ƽṹ��
struct LogicController {
	int room[ROOM_WIDTH][ROOM_LENGTH]; //��ά��������ʾ���������״̬���������������ʼ��Ϊδ֪״̬GRID_UNKNOWN
	int robotX;	//������Ŀǰ��x����
	int robotY;	//������Ŀǰ��y����
};

// ���巿�����鲻ͬ״̬��Ӧ��ANSI��ɫ����
const string GRID_UNKNOWN_COLOR = "\033[47m  \033[0m";	//��ɫ����ʾδ֪����
const string GRID_NORMAL_COLOR = "\033[46m  \033[0m";	//�� ɫ����ʾ�ѽ�ͼ��Ŀ���ɨ����
const string GRID_STATIC_OBSTACLE_COLOR = "\033[41m  \033[0m";	//��ɫ����ʾ�ѽ�ͼ��ľ�̬�ϰ�������
const string GRID_DYNAMIC_OBSTACLE_COLOR = "\033[45m  \033[0m";	//Ʒ�죬��ʾ��ɨʱ���ֵĶ�̬�ϰ�������
const string GRID_CLEANED_COLOR = "\033[42m  \033[0m";	//��ɫ����ʾ�Ѿ���ɨ������
const string GRID_ROBOT_COLOR = "\033[43m  \033[0m";	//��ɫ����ʾɨ�ػ����˵�ǰ��������

unordered_map<int,string> colorMap;	//��¼ö�ٶ�Ӧ��ɫ

/***
 * ���ÿ���̨Ϊ�����ն�����ģʽ��ʹ���ܹ�֧�ֿɿ��ƵĹ���ƶ�����ɫ�ı��ȹ���
 * [1] https://learn.microsoft.com/zh-cn/windows/console/setconsolemode
 * [2] https://learn.microsoft.com/zh-cn/windows/console/console-virtual-terminal-sequences
 */
void config_screen() {
	// ��ȡ����̨���
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		cout << "Error:" << GetLastError() << endl;
		return;
	}

	// ��ȡ����̨��ǰģʽ
	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) {
		cout << "Error:" << GetLastError() << endl;
		return;
	}

	// ����̨��ǰģʽ�����������ն�����ģʽ
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) {
		cout << "Error:" << GetLastError() << endl;
		return;
	}
}

/***
 * ������ƶ���ָ��λ��
 * @param x ָ���� x ����
 * @param y ָ���� y ����
 */
void gotoxy(int x, int y) {
	//Ϊ�˽��������򲻶�����ʾ������ͳһ����ƫ����
	x += 2;
	y += 2;
	x *= 2;	//��Ϊʹ��2���ո��ʾһ��������������Ҫ����2
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


/***
 * ��ӡĳ��ʱ�̷������������״̬
 * @param controller Ŀ��ɨ�ػ����˿�����
 */
void print_room_map(const LogicController controller) {
	//Locked_Fog 25/5/6
	/***
	 * ˼·���ƶ���굽��ʼλ�ã�Ȼ��controller�ڵ����ݴ�ӡͼ��
	 */
	for(int i=0;i<ROOM_WIDTH;i++){
		for(int j=0;j<ROOM_LENGTH;j++){
			gotoxy(j,i);		//ע�⣺���������Ĺ�ϵӦ��Ϊcontroller.room[y][x]
			if(i==controller.robotX && j==controller.robotY){
				cout<<colorMap[GRID_ROBOT];
			}else{
				cout<<colorMap[controller.room[i][j]];
			}
		}
	}
	return;
}

/***
 * ��ͼ
 * @param controller Ŀ��ɨ�ػ����˿�����
 */
void mapping(LogicController &controller) {
	srand(time(0));
    
    // �Ƚ�����������Ϊδ֪
    for (int i = 0; i < ROOM_WIDTH; i++) {
        for (int j = 0; j < ROOM_LENGTH; j++) {
            controller.room[i][j] = GRID_UNKNOWN;
        }
    }
    
    // ����һЩ��̬�ϰ���
    int obstacleCount = (ROOM_WIDTH * ROOM_LENGTH) / 8; // ��Լ1/8���������ϰ���
    for (int i = 0; i < obstacleCount; i++) {
        int x = rand() % ROOM_WIDTH;
        int y = rand() % ROOM_LENGTH;
        
        // ȷ�����������Ϊ�ϰ���
        if (x != 0 || y != 0) {
            controller.room[x][y] = GRID_STATIC_OBSTACLE;
        }
    }
    
    // ģ�⽨ͼ���̣������˴���㿪ʼ̽����������
    vector<vector<bool>> visited(ROOM_WIDTH, vector<bool>(ROOM_LENGTH, false));
    queue<pair<int, int>> q;
    
    // �����Ϊ�ѷ��ʲ���Ϊ��ͨ��
    controller.room[0][0] = GRID_NORMAL;
    q.push({0, 0});
    visited[0][0] = true;
    
    // �����ĸ������ϡ��ҡ��¡���
    vector<pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        int x = current.first;
        int y = current.second;
        
        // ���»�����λ�ò���ʾ
        controller.robotX = x;
        controller.robotY = y;
        print_room_map(controller);
        Sleep(100); // �����ٶ��Ա�۲�
        
        // �����ĸ�����
        for (auto dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            
            // ����Ƿ��ڱ߽�����δ���ʹ�
            if (newX >= 0 && newX < ROOM_WIDTH && newY >= 0 && newY < ROOM_LENGTH && 
                !visited[newX][newY]) {
                
                visited[newX][newY] = true;
                
                // ������ϰ�����Ϊ��̬�ϰ���
                if (controller.room[newX][newY] == GRID_STATIC_OBSTACLE) {
                    // ����Ҫ�ı䣬�Ѿ��Ǿ�̬�ϰ���
                } else {
                    // ������Ϊ����ɨ����
                    controller.room[newX][newY] = GRID_NORMAL;
                    q.push({newX, newY});
                }
            }
        }
    }
}
	//TODO


//��ɨ
/***
 * ��ɨ
 * @param controller Ŀ��ɨ�ػ����˿�����
 */
void scanning_sweep(LogicController &controller) {
	//TODO
}

/***
 * ������ɨ
 * @param x Ŀ��ص�� x ����
 * @param y Ŀ��ص�� y ����
 * @param controller Ŀ��ɨ�ػ����˿�����
 */
// void target_sweep(int x,int y,LogicController &controller) {
	//TODO
// }

/***
 * ���ƻ����˻ص����
 * @param controller Ŀ��ɨ�ػ����˿�����
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
 * ��ӡ��ʾ��Ϣ����ɴ�ӡ����ʾ���������������...��
 * @param msg Ԥ�ƽ�Ҫ��ӡ����Ϣ
 */
void step_over(string msg) {
	cout << endl << msg << endl;
	cout << "�������������..." << endl;
	getch();	//�ȴ�������������£�����Ҫ���س���ȷ������
	system("cls");	//�������̨���������е��ַ�����������ƶ������Ϸ�
}

int main() {
	//������Ļ��ʹ��֧�ֹ��λ�ÿ��ƺͲ�ɫ����
	config_screen();

	//��ʼ��colorMap
	colorMap[GRID_UNKNOWN] = GRID_UNKNOWN_COLOR;
	colorMap[GRID_NORMAL] = GRID_NORMAL_COLOR;
	colorMap[GRID_CLEANED] = GRID_CLEANED_COLOR;
	colorMap[GRID_STATIC_OBSTACLE] = GRID_STATIC_OBSTACLE_COLOR;
	colorMap[GRID_DYNAMIC_OBSTACLE] = GRID_DYNAMIC_OBSTACLE_COLOR;
	colorMap[GRID_ROBOT] = GRID_ROBOT_COLOR;

	//�����߼�������
	LogicController controller;
	//����������������ʼ��Ϊδ֪״̬
	for (int i = 0; i < ROOM_WIDTH; i++)
		for (int j = 0; j < ROOM_LENGTH; j++)
			controller.room[i][j] = GRID_UNKNOWN;
	//���û����˵ĳ�ʼλ��Ϊ���Ϸ�
	controller.robotX = 0;
	controller.robotY = 0;
	controller.room[0][0] = GRID_NORMAL;

	//test
	print_room_map(controller);

	//��ͼ
	mapping(controller);
	goto_start_point(controller);
	step_over("��ͼ��ɣ�");

	//��ɨ
	scanning_sweep(controller);
	goto_start_point(controller);
	step_over("��ɨ��ɣ�");

	//������ɨ��ѡ����
	//target_sweep(controller);
	//step_over("������ɨ��ɣ�");

	//�ص����
	goto_start_point(controller);
	step_over("���й��̽�����");

	return 0;
}