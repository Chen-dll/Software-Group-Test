//#This program is coded by Chen Sixiang#// 
//#Program Name：2048#//
//#Finish Date: 2024.10.3#//
//#Environment: Dev-C++ 5.11 with EasyX#// 
#include <graphics.h> // EasyX 图形库头文件
#include <conio.h>    // 控制台输入输出
#include <ctime>      // 用于随机数生成
#include <cstdlib>
#include <string.h>   // 处理字符串 
#include <stdio.h>
#include <windows.h>  // 调用WINAPI
#include <tchar.h> 	  // 获得管理员权限要用 
#include <imm.h> 	  // 切换输入法要用 

#pragma comment(lib, "Imm32.lib") //输入法要用 

#define SIZE 4 // 定义游戏板的大小
#define UNDO_LIMIT 2048 // 定义撤销操作的最大存储数量

int board[SIZE][SIZE]; // 当前游戏板
int previous_board[UNDO_LIMIT][SIZE][SIZE]; // 存储撤销的游戏状态
int previous_scores[UNDO_LIMIT];// 存储撤销的得分
int undo_count = 0; // 当前撤销数量
int score = 0; // 当前得分

// 显示按键功能介绍
void show_key_intro() 
{
    // 设置背景颜色
    setfillcolor(RGB(0, 0, 0)); // 黑色背景
    solidrectangle(0, 0, 400, 400); // 填充背景

    // 设置文本颜色
    settextcolor(RGB(255, 255, 255)); // 白色文本
    setbkmode(TRANSPARENT); // 透明背景

    // 显示标题
    settextstyle(40, 0, "Consolas");
    const char* title = "2048 Game Controls";
    int title_width = textwidth(title);
    outtextxy((400 - title_width) / 2, 25, title); // 居中标题
    
    // 显示按键功能说明 用中文怕出现乱码 
    settextstyle(24, 0, "Consolas"); // 字体格式大小 
    outtextxy(50, 75, "W - Move Up"); // 位置 
    outtextxy(50, 125, "A - Move Left");
    outtextxy(50, 170, "S - Move Down");
    outtextxy(50, 225, "D - Move Right");
    outtextxy(50, 275, "Z - Undo Move");
    outtextxy(50, 325, "P - Save Game");
    outtextxy(50, 375, "L - Load Game");

    // 等待用户按键
    _getch();
}

// 切换英文输入法 
void SwitchToEnglish() 
{
    // 获取当前输入法句柄
    HKL currentLayout = GetKeyboardLayout(0);

    // 英文输入法的句柄（US输入法）
    HKL englishLayout = LoadKeyboardLayout("00000409", KLF_SETFORPROCESS); // 00000409 是美国英语的标识

    // 切换输入法
    if (englishLayout) 
        ActivateKeyboardLayout(englishLayout, 0);
        
    Sleep(100); // 缓冲时间 
    
    SetForegroundWindow(GetConsoleWindow()); // 让焦点转到控制台以实现切换输入法 
    HWND easyxWindow = GetForegroundWindow(); // 获取EasyX窗口句柄 
    SetForegroundWindow(easyxWindow); // 再换回去EasyX的窗口 
}

// 获得管理员权限以修改控制台窗口 
void RunAsAdmin() 
{
    // 判断当前程序是否以管理员身份运行
    BOOL isAdmin = FALSE;
    HANDLE hToken = NULL;
    TOKEN_ELEVATION te;
    
    // 获取当前进程的令牌
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) 
    {
        DWORD dwSize;
        
        if (GetTokenInformation(hToken, TokenElevation, &te, sizeof(te), &dwSize)) 
            isAdmin = te.TokenIsElevated;
            
        CloseHandle(hToken);
    }

    // 如果没有管理员权限，重新启动程序以获得权限
    if (!isAdmin) 
    {
        // 构造程序路径
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);

        // 使用ShellExecute以管理员身份运行程序
        ShellExecuteA(NULL, "runas", path, NULL, NULL, SW_SHOWNORMAL);

        // 终止当前进程
        TerminateProcess(GetCurrentProcess(), 0);
    }
}

// 绘制当前游戏板
void draw_board(int should_clear) 
{
    // 状态改变才清屏 缓解闪屏问题 双缓冲还没学会orz 
    if (should_clear) 
    {
        setfillcolor(RGB(187, 173, 160)); // 设置背景颜色
        solidrectangle(0, 0, 400, 400); // 填充背景
    }

    for (int i = 0; i < SIZE; i++) 
    {
        for (int j = 0; j < SIZE; j++) 
        {
            int number = board[i][j];
            int x = j * 100 + 10; // 每个格子的 x 坐标
            int y = i * 100 + 10; // 每个格子的 y 坐标

            // 根据数字设置不同的颜色
            if (number == 0) {
                setfillcolor(RGB(205, 193, 180)); // 空格子的颜色
            } else if (number == 2) {             // 2 的 颜色 
                setfillcolor(RGB(238, 228, 218));  
            } else if (number == 4) {             // 4 的 颜色 
                setfillcolor(RGB(237, 224, 200));
            } else if (number == 8) {			  // 8 的 颜色 
                setfillcolor(RGB(242, 177, 121));
            } else if (number == 16) {			  // 16 的 颜色 
                setfillcolor(RGB(245, 149, 99));
            } else if (number == 32) {			  // 32 的 颜色 
                setfillcolor(RGB(246, 124, 95));
            } else if (number == 64) {			  // 64 的 颜色 
                setfillcolor(RGB(246, 94, 59));
            } else if (number == 128) {	          // 128 的 颜色 
                setfillcolor(RGB(237, 207, 114));
            } else if (number == 256) {           // 256 的 颜色 
                setfillcolor(RGB(237, 204, 97));
            } else if (number == 512) {           // 512 的 颜色 
                setfillcolor(RGB(237, 200, 80));
            } else if (number == 1024) {		  // 1024 的 颜色 
                setfillcolor(RGB(237, 197, 63));
            } else if (number == 2048) {          // 2048 的 颜色 
                setfillcolor(RGB(237, 194, 46));
            }

            // 绘制每个格子
            solidrectangle(x, y, x + 80, y + 80);

            // 绘制数字
            if (number != 0) 
            {
                char num_str[5];
                snprintf(num_str, sizeof(num_str), "%d", number); // 使用 snprintf 更加安全 
                setbkmode(TRANSPARENT); // 设置透明背景
                settextstyle(30, 0, "Consolas"); // 设置字体
                settextcolor(RGB(119, 110, 101)); // 设置文本颜色
                int text_width = textwidth(num_str);
                int text_height = textheight(num_str);
                outtextxy(x + (80 - text_width) / 2, y + (80 - text_height) / 2, num_str); // 居中 
            }
        }
    }

    // 绘制得分
    char score_str[20];
    snprintf(score_str, sizeof(score_str), "Score: %d", score);
    settextcolor(BLACK);
    outtextxy(10, 10, score_str); // 在左上角显示得分
}

// 显示提示信息的函数
void show_message(const char *msg) 
{
    setfillcolor(RGB(0, 0, 0)); // 设置黑色背景
    solidrectangle(50, 150, 350, 250); // 填充消息框
    settextcolor(RGB(255, 255, 255)); // 设置文本颜色为白色
    int msg_width = textwidth(msg); // 获取文本宽度
    outtextxy(200 - msg_width / 2, 200, msg); // 显示消息，居中
    Sleep(1500);   // 显示 1.5 秒
    draw_board(1); // 重新打印 
}

// 初始化游戏板
void initialize_board() 
{
    srand((unsigned)time(NULL)); // 随机数种子
    for (int i = 0; i < SIZE; i++) 
        for (int j = 0; j < SIZE; j++) 
            board[i][j] = 0; // 初始化时所有格子为空
}

// 在空位置随机生成2或4
void add_random_number() 
{
    int x, y, num;
    do {
        x = rand() % SIZE;
        y = rand() % SIZE;
    } while (board[x][y] != 0); // 直到找到空位置

    num = (rand() % 2 + 1) * 2; // 生成2或4
    board[x][y] = num;
}

// 存储当前状态用于撤销
void save_to_undo() 
{
    if (undo_count < UNDO_LIMIT) 
    {
        for (int i = 0; i < SIZE; i++) 
            for (int j = 0; j < SIZE; j++) 
                previous_board[undo_count][i][j] = board[i][j]; // 用数组存起来 
                
        previous_scores[undo_count] = score; // 储存当前状态成绩 
        undo_count++;
    }
}

// 撤销操作
void undo() 
{
    if (undo_count > 0) 
    {
        undo_count--; // 减少撤销计数
        
        for (int i = 0; i < SIZE; i++) 
            for (int j = 0; j < SIZE; j++) 
                board[i][j] = previous_board[undo_count][i][j]; // 恢复到上一次状态

        score = previous_scores[undo_count]; // 显示撤销后对应的成绩 
        draw_board(1); // 打印 
    } 
    else 
        show_message("Cannot be revoked!"); // 提示用户没有可撤销的操作
}

// 游戏板左移
void move_left() 
{
    save_to_undo(); // 保存当前状态
    int moved = 0; // 标记是否发生了移动
    for (int i = 0; i < SIZE; i++) 
    {
        int current = 0; // 当前合并的列位置
        for (int j = 1; j < SIZE; j++) 
        {
            if (board[i][j] != 0) 
            {
                if (board[i][current] == 0) 
                {
                    board[i][current] = board[i][j]; // 将数字移动到当前列
                    board[i][j] = 0;
                    moved = 1; // 标记发生了移动
                } 
                else if (board[i][current] == board[i][j]) 
                {
                    board[i][current] *= 2; // 合并相同数字
                    score += board[i][current]; // 更新得分
                    board[i][j] = 0;
                    current++; // 合并后移动到下一个列
                    moved = 1; // 标记发生了移动
                } 
                else 
                {
                    current++;
                    if (current != j) 
                    {
                        board[i][current] = board[i][j]; // 将数字移动到下一个列
                        board[i][j] = 0;
                        moved = 1; // 标记发生了移动
                    }
                }
            }
        }
    }
    if (moved) 
    {
        add_random_number(); // 只有在有效移动后才添加新的随机数字
        draw_board(1); // 更新游戏板并清空背景
    } 
    else 
        draw_board(0); // 更新游戏板但不清空背景
}

// 游戏板右移 
void move_right() 
{
    save_to_undo(); // 保存当前状态
    int moved = 0; // 标记是否发生了移动
    for (int i = 0; i < SIZE; i++) 
    {
        int current = SIZE - 1; // 当前合并的列位置
        for (int j = SIZE - 2; j >= 0; j--) 
        {
            if (board[i][j] != 0) 
            {
                if (board[i][current] == 0) 
                {
                    board[i][current] = board[i][j]; // 将数字移动到当前列
                    board[i][j] = 0;
                    moved = 1; // 标记发生了移动
                } 
                else if (board[i][current] == board[i][j]) 
                {
                    board[i][current] *= 2; // 合并相同数字
                    score += board[i][current]; // 更新得分
                    board[i][j] = 0;
                    current--; // 合并后移动到下一个列
                    moved = 1; // 标记发生了移动
                } 
                else 
                {
                    current--;
                    if (current != j) 
                    {
                        board[i][current] = board[i][j]; // 将数字移动到下一个列
                        board[i][j] = 0;
                        moved = 1; // 标记发生了移动
                    }
                }
            }
        }
    }
    if (moved) 
    {
        add_random_number(); // 只有在有效移动后才添加新的随机数字
        draw_board(1); // 更新游戏板并清空背景
    } 
    else 
        draw_board(0); // 更新游戏板但不清空背景
}

// 游戏板上移 
void move_up() 
{
    save_to_undo(); // 保存当前状态
    int moved = 0; // 标记是否发生了移动
    for (int j = 0; j < SIZE; j++) 
    {
        int current = 0; // 当前合并的行位置
        for (int i = 1; i < SIZE; i++) 
        {
            if (board[i][j] != 0) 
            {
                if (board[current][j] == 0) 
                {
                    board[current][j] = board[i][j]; // 将数字移动到当前行
                    board[i][j] = 0;
                    moved = 1; // 标记发生了移动
                } 
                else if (board[current][j] == board[i][j]) 
                {
                    board[current][j] *= 2; // 合并相同数字
                    score += board[current][j]; // 更新得分
                    board[i][j] = 0;
                    current++; // 合并后移动到下一行
                    moved = 1; // 标记发生了移动
                } 
                else 
                {
                    current++;
                    if (current != i) 
                    {
                        board[current][j] = board[i][j]; // 将数字移动到下一行
                        board[i][j] = 0;
                        moved = 1; // 标记发生了移动
                    }
                }
            }
        }
    }
    if (moved) 
    {
        add_random_number(); // 只有在有效移动后才添加新的随机数字
        draw_board(1); // 更新游戏板并清空背景
    } 
    else 
        draw_board(0); // 更新游戏板但不清空背景
}

// 游戏板下移 
void move_down() 
{
    save_to_undo(); // 保存当前状态
    int moved = 0; // 标记是否发生了移动
    for (int j = 0; j < SIZE; j++) 
    {
        int current = SIZE - 1; // 当前合并的行位置
        for (int i = SIZE - 2; i >= 0; i--) 
        {
            if (board[i][j] != 0) 
            {
                if (board[current][j] == 0) 
                {
                    board[current][j] = board[i][j]; // 将数字移动到当前行
                    board[i][j] = 0;
                    moved = 1; // 标记发生了移动
                } 
                else if (board[current][j] == board[i][j]) 
                {
                    board[current][j] *= 2; // 合并相同数字
                    score += board[current][j]; // 更新得分
                    board[i][j] = 0;
                    current--; // 合并后移动到下一行
                    moved = 1; // 标记发生了移动
                } 
                else 
                {
                    current--;
                    if (current != i) 
                    {
                        board[current][j] = board[i][j]; // 将数字移动到下一行
                        board[i][j] = 0;
                        moved = 1; // 标记发生了移动
                    }
                }
            }
        }
    } 
    if (moved) 
    {
        add_random_number(); // 只有在有效移动后才添加新的随机数字
        draw_board(1); // 更新游戏板并清空背景
    } 
    else
        draw_board(0); // 更新游戏板但不清空背景
}

// 判断游戏是否胜利（达到2048）
int check_win() 
{
    for (int i = 0; i < SIZE; i++) 
        for (int j = 0; j < SIZE; j++) 
            if (board[i][j] == 2048) 
                return 1; //遍历游戏板 
                
    return 0;
}

// 判断游戏是否失败（无可移动空间）
int check_loss() 
{
    for (int i = 0; i < SIZE; i++) 
    {
        for (int j = 0; j < SIZE; j++) 
        {
            if (board[i][j] == 0) return 0; // 有空位，未失败
            // 判断相邻相同数字
            if (i < SIZE - 1 && board[i][j] == board[i + 1][j]) return 0; // 下方可以合并
            if (j < SIZE - 1 && board[i][j] == board[i][j + 1]) return 0; // 右方可以合并
        }
    }
    return 1; // 没有可移动空间，游戏失败
}

// 保存游戏进度
void save_game(const char *filename) 
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL) // 保存失败 
    {
        show_message("Save Game failed!");
        return;
    }
    fwrite(board, sizeof(int), SIZE * SIZE, file); // 保存游戏板数据
    fwrite(&score, sizeof(int), 1, file); // 保存得分
    fclose(file);
    show_message("Game Saved!"); // 提示保存成功 
}

// 加载游戏进度
void load_game(const char *filename) 
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) // 加载失败 
    {
        show_message("Game loading failed!");
        return;
    }
    fread(board, sizeof(int), SIZE * SIZE, file); // 读取游戏板数据
    fread(&score, sizeof(int), 1, file); // 读取得分
    fclose(file);
    show_message("Game Loaded!"); // 提示加载成功 
}

// 游戏结束弹窗
void show_game_over() 
{
    setfillcolor(RGB(0, 0, 0)); // 设置黑色背景
    solidrectangle(50, 150, 350, 250); // 填充游戏结束框
    settextcolor(RGB(255, 0, 0)); // 设置文本颜色为红色
    const char *msg = "Game Over!";
    int msg_width = textwidth(msg); // 获取文本宽度
    outtextxy(200 - msg_width / 2, 200, msg); // 显示游戏结束信息，居中
    _getch(); // 等待用户按键
}

// 游戏胜利弹窗
void show_win() 
{
    setfillcolor(RGB(0, 0, 0)); // 设置黑色背景
    solidrectangle(50, 150, 350, 250); // 填充游戏胜利框
    settextcolor(RGB(0, 255, 0)); // 设置文本颜色为绿色
    const char *msg = "You Win!";
    int msg_width = textwidth(msg); // 获取文本宽度
    outtextxy(200 - msg_width / 2, 200, msg); // 显示游戏胜利信息，居中
    _getch(); // 等待用户按键
}

//主循环 
void game_loop() 
{
    initialize_board(); // 初始化游戏板
    //初始状态有两个数字 
    add_random_number(); // 第一个数字 
    add_random_number(); // 第二个数字
    
    draw_board(1); //打印 
    //进入循环 
    while (1) 
    {
        Sleep(100); // 缓冲时间 可以微调 
        if (check_win()) 
        {
            show_win(); // 显示游戏胜利信息
            break;
        }
        if (check_loss()) 
        {
            show_game_over(); // 显示游戏结束信息
            break;
        }
        if (_kbhit()) 
        {
            char input = _getch(); // 读取用户输入
            // 过滤输入，确保只处理有效的控制按键
            switch (input) 
            {
                case 'a': move_left(); break;  // 左移 
                case 'd': move_right(); break; // 右移 
                case 'w': move_up(); break;    // 上移 
                case 's': move_down(); break;  // 下移 
                case 'z': undo(); break; // 撤销上一步
                case 'p': save_game("2048_save.dat"); break; // 保存游戏
                case 'l': load_game("2048_save.dat"); break; // 加载游戏
                default: break; // 忽略无效输入
            }
        }
    }
}

// 主函数
int main() 
{
    RunAsAdmin();        // 获取管理员权限 
    initgraph(400, 400); // 初始化窗口   
    SwitchToEnglish();   // 输入法换成英文  	
    show_key_intro();    // 按键介绍 
    game_loop();         // 开始游戏循环
    closegraph();        // 关闭窗口
    return 0;
}
//#This program is coded by Chen Sixiang#// 
