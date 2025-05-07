# 清扫机器人

> 2025春 C++ 小组作业

#### 功能

- [x] print_room_map

- [ ] mapping

- [ ] scanning_sweep

- [ ] target_sweep(choseable)

#### 配置（使用VS Code进行配置）

##### step 0

克隆此仓库

```git bash
git clone https://github.com/locked-fog/SweepRobot.git
```

在仓库中右键 -> 使用 VS Code 打开

##### step 1

注意，本项目采用 `GBK2312` 编辑，所以所有项目文件需要以 `GBK2312` 打开：

- 打开一个文件，在右下角找到并点击编码选择器（`UTF-8`），在弹出的对话框中选择 `通过编码重新打开(Reopen with Encoding)` 

##### step 2

配置开发环境

打开 `SweepRobot.cpp`，按下 `Ctrl+Shift+P` ，在弹出的对话框中输入 `Tasks: Configure Default Build Task` ，选择 `任务：配置默认生成任务(Tasks: Configure Default Build Task)` 。

选择 `C/C++: g++.exe 生成活动文件` 。

修改 `args` 属性：

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

保存并关闭文件。

##### step 3

回到 `SweepRobot.cpp` ，按下 `F5`，此时应当弹出错误窗口 `launch: program ... does not exist` ，按下 `打开"launch.json"`。

修改 `launch.json` 中的属性：

- 添加属性 `"preLaunchTask": "C/C++: g++.exe 生成活动文件"`

- 修改属性 `"cwd": "${workspaceFolder}"`

- 修改属性 `"program": "${fileDirname}\\${fileBasenameNoExtension}.exe"`

保存并关闭文件

##### step 4

回到 `SweepRobot.cpp` ，按下 `F5`，此时应当编译通过，并在新弹出的命令行窗口中运行程序。

#### Standard

代码开发需要遵循一定规范。

- 尽可能采用驼峰命名法（变量第一个词首字母小写，类型第一个词首字母大写，各词第一个字母使用大写字母）
- - 例如：`int robotPos` `class SweepRobot`

- 代码块的第一个大括号归并到上一行（参考已有代码），不能省略代码块的大括号（即便只有一行代码）

- 尽可能为自己的代码注释，并在每次写完后标记自己修改的部分

- 每次开始写代码前先同步项目，避免代码冲突

#### Author

Locked_Fog, levely, sooorry
