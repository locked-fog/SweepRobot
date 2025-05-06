# ��ɨ������

> 2025�� C++ С����ҵ

#### ����

- [ ] print_room_map

- [ ] mapping

- [ ] scanning_sweep

- [ ] target_sweep(choseable)

#### ���ã�ʹ��VS Code�������ã�

##### step 0

��¡�˲ֿ�

```git bash
git clone https://github.com/locked-fog/SweepRobot.git
```

�ڲֿ����Ҽ� -> ʹ�� VS Code ��

##### step 1

ע�⣬����Ŀ���� `GBK2312` �༭������������Ŀ�ļ���Ҫ�� `GBK2312` �򿪣�

- ��һ���ļ��������½��ҵ����������ѡ������`UTF-8`�����ڵ����ĶԻ�����ѡ�� `ͨ���������´�(Reopen with Encoding)` 

##### step 2

���ÿ�������

�� `SweepRobot.cpp`������ `Ctrl+Shift+P` ���ڵ����ĶԻ��������� `Tasks: Configure Default Build Task` ��ѡ�� `��������Ĭ����������(Tasks: Configure Default Build Task)` ��

ѡ�� `C/C++: g++.exe ���ɻ�ļ�` ��

�޸� `args` ���ԣ�

```json
"args": [
    "-fdiagnostics-color=always",
    "-g",
    "${file}",
    "-L${workspaceFolder}",
    "-lLaserDetector",
    "-lPathPlanner",
    "-o",
    "${fileDirname}\\${fileBasenameNoExtension}.exe"
]
```

���沢�ر��ļ���

##### step 3

�ص� `SweepRobot.cpp` ������ `F5`����ʱӦ���������󴰿� `launch: program ... does not exist` ������ `��"launch.json"`��

�޸� `launch.json` �е����ԣ�

- ������� `"preLaunchTask": "C/C++: g++.exe ���ɻ�ļ�"`

- �޸����� `"cwd": "${workspaceFolder}"`

- �޸����� `"program": "${fileDirname}\\${fileBasenameNoExtension}.exe"`

���沢�ر��ļ�

##### step 4

�ص� `SweepRobot.cpp` ������ `F5`����ʱӦ������ͨ���������µ����������д��������г���

#### Standard

���뿪����Ҫ��ѭһ���淶��

- �����ܲ����շ���������������һ��������ĸСд�����͵�һ��������ĸ��д�����ʵ�һ����ĸʹ�ô�д��ĸ��
- - ���磺`int robotPos` `class SweepRobot`

- �����ĵ�һ�������Ź鲢����һ�У��ο����д��룩������ʡ�Դ����Ĵ����ţ�����ֻ��һ�д��룩

- ������Ϊ�Լ��Ĵ���ע�ͣ�����ÿ��д������Լ��޸ĵĲ���

- ÿ�ο�ʼд����ǰ��ͬ����Ŀ����������ͻ

#### Author

Locked_Fog, levely
