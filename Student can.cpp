//#This program is coded by Chen Sixiang#// 
//#Program Name：学生信息管理系统 #//
//#Finish Date: 2024.10.6#//
//#Environment: Dev-C++ 5.11 with EasyX#// 
#include <stdio.h>    // 标准输入输出库
#include <stdlib.h>   // 标准库
#include <string.h>   // 字符串处理
#include <conio.h>    // 控制台输入输出
#include <windows.h>  // Windows API
#include <graphics.h> // EasyX 图形库头文件

#define MAX_STUDENTS 100     // 最大存储学生数量
#define STUDENTS_PER_PAGE 10 // 每页显示学生的数量
#define WIDTH 800   // 窗口宽度 
#define HEIGHT 600  // 窗口高度 

// 学生结构体定义
typedef struct
{
    long long id;       // id   使用long long是考虑到id比较大的情况 比如3124000xxx 
    char name[50];      // 姓名
    long long age;      // 年龄 
    char className[20]; // 班级
    double grade;       // 成绩
} Student;

Student students[MAX_STUDENTS]; // 声明学生结构体数组
int studentCount = 0;           // 记录学生数量
int currentStartIndex = 0; // 当前显示的起始学生索引
int isQuerying = 0;        // 查询状态标记

// 绘制所有学生信息到屏幕
void draw_Students()
{
    settextstyle(24, 0, "Consolas"); // 设置字体样式

    if (!isQuerying) // 如果没在查询操作 
    {
        cleardevice(); // 清除屏幕
        // 绘制当前页的学生信息
        for (int i = 0; i < STUDENTS_PER_PAGE && currentStartIndex + i < studentCount; i++)
        {
            int y = 50 + i * 40; // 每条记录在屏幕上的 y 坐标
            char info[100];
            // // 将学生信息格式化为字符串
            snprintf(info, sizeof(info), "ID: %lld Name: %s Age: %lld Class: %s Grade: %.2f",
                     students[currentStartIndex + i].id, students[currentStartIndex + i].name,
                     students[currentStartIndex + i].age, students[currentStartIndex + i].className,
                     students[currentStartIndex + i].grade);
            outtextxy(20, y, info); // 显示信息
        }

        // 确保总学生数大于可见学生数 否则不需要滑动条 
        if (studentCount > STUDENTS_PER_PAGE)
        {
            float sliderRatio = (float)STUDENTS_PER_PAGE / studentCount; // 计算比例
            int sliderLength = (int)(sliderRatio * HEIGHT);              // 滑动条的长度

            // 计算滑动条的位置
            int sliderPosition = (int)((float)currentStartIndex / (studentCount - STUDENTS_PER_PAGE) * (HEIGHT - sliderLength));

            // 绘制滑动条
            setfillstyle(SOLID_FILL, DARKGRAY);                                               // 滑动条颜色
            solidrectangle(WIDTH - 20, sliderPosition, WIDTH, sliderPosition + sliderLength); // 具体位置和大小
        }
    }
}

// 在控制台操作
// 添加学生信息
void add_Student()
{
    if (studentCount >= MAX_STUDENTS) // 判断还能不能添加学生
    {
        printf("Student list is full!\n");
        return;
    }
    printf("Enter student ID: ");
    scanf("%lld", &students[studentCount].id);       // 输入id
    printf("Enter student name: ");
    scanf("%s", students[studentCount].name);        // 输入名字
    printf("Enter student age: ");
    scanf("%lld", &students[studentCount].age);      // 输入年龄
    printf("Enter student class: ");
    scanf("%s", students[studentCount].className);   // 输入班级
    printf("Enter student grade: ");
    scanf("%lf", &students[studentCount].grade);     // 输入成绩

    studentCount++; // 学生数量+1
}

// 根据 ID 或姓名删除学生信息
void delete_Student()
{
    char option[10];
    printf("Delete by (id/name): "); // 选用 ID 或 姓名 进行修改
    scanf("%s", option);             // 输入选项

    if (strcmp(option, "id") == 0) // strcmp() 比较两个字符串 相等返回 0
    {
        long long id;
        printf("Enter student ID to delete: ");
        scanf("%lld", &id); // 输入 ID

        int found = 0;                         // 记录找没找到
        for (int i = 0; i < studentCount; i++) // 遍历查找
        {
            if (students[i].id == id)
            {
                found = 1;
                // 将数组后面的元素往前移，覆盖要删除的学生
                for (int j = i; j < studentCount - 1; j++)
                {
                    students[j] = students[j + 1];
                }
                studentCount--; // 学生数量-1
                printf("Student with ID %lld deleted.\n", id);
                break;
            }
        }

        if (!found) // 没找到
        {
            printf("Student with ID %lld not found.\n", id);
        }
    }
    else if (strcmp(option, "name") == 0) // 选了按姓名删除
    {
        char name[50];
        printf("Enter student name to delete: ");
        scanf("%s", name); // 输入姓名
        // 和上面类似操作
        int found = 0;
        for (int i = 0; i < studentCount; i++)
        {
            if (strcmp(students[i].name, name) == 0)
            {
                found = 1;
                // 将数组后面的元素往前移，覆盖要删除的学生
                for (int j = i; j < studentCount - 1; j++)
                {
                    students[j] = students[j + 1];
                }
                studentCount--; // 学生数量-1 
                printf("Student with name %s deleted.\n", name);
                break;
            }
        }

        if (!found) // 没找到
        {
            printf("Student with name %s not found.\n", name);
        }
    }
    else // 处理无效输入
    {
        printf("Invalid option! Please enter 'id' or 'name'.\n");
    }
}

// 根据 ID 或姓名修改学生信息
void modify_Student()
{
    char option[10];
    printf("Modify by (id/name): "); // 选用 ID 或 姓名进行修改
    scanf("%s", option);             // 输入选项
    if (strcmp(option, "id") == 0)   // 按 ID 查找
    {
        long long id;
        printf("Enter student ID to modify: ");
        scanf("%lld", &id);

        int found = 0; // 记录找没找到
        for (int i = 0; i < studentCount; i++)
        {
            if (students[i].id == id)
            {
                found = 1;
                // 找到后重新输入对应学生数据
                printf("Enter new name: ");
                scanf("%s", students[i].name);         // 输入姓名
                printf("Enter new age: ");
                scanf("%lld", &students[i].age);       // 输入年龄
                printf("Enter new class: ");
                scanf("%s", students[i].className);    // 输入班级
                printf("Enter new grade: ");
                scanf("%lf", &students[i].grade);      // 输入成绩
                printf("Student with ID %lld modified.\n", id);
                break;
            }
        }

        if (!found) // 没找到
        {
            printf("Student with ID %lld not found.\n", id);
        }
    }
    else if (strcmp(option, "name") == 0) // 按姓名查找
    {
        char name[50];
        printf("Enter student name to modify: ");
        scanf("%s", name); // 输入名字

        int found = 0;
        for (int i = 0; i < studentCount; i++)
        {
            if (strcmp(students[i].name, name) == 0)
            {
                found = 1;
                // 找到后重新输入对应学生数据
                printf("Enter new ID: ");
                scanf("%lld", &students[i].id);      // 输入id
                printf("Enter new age: ");
                scanf("%lld", &students[i].age);     // 输入年龄
                printf("Enter new class: ");
                scanf("%s", students[i].className);  // 输入班级
                printf("Enter new grade: ");
                scanf("%lf", &students[i].grade);    // 输入成绩
                printf("Student with name %s modified.\n", name);
                break;
            }
        }

        if (!found) // 没找到
        {
            printf("Student with name %s not found.\n", name);
        }
    }
    else // 处理无效输入
    {
        printf("Invalid option! Please enter 'id' or 'name'.\n");
    }
}

// 保存学生信息到文件 类似2048项目对应功能 写成 txt文件
void save_To_File(const char *filename)
{
    FILE *file = fopen(filename, "w"); // 以写入模式打开文件
    if (!file)                         // 保存失败
    {
        printf("Error opening file for saving!\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) // 逐个写入
    {
        fprintf(file, "%lld %s %lld %s %.2f\n", students[i].id, students[i].name, 
		        students[i].age, students[i].className, students[i].grade);
    }
    fclose(file); // 关闭文件
}

// 从文件加载学生信息，如果文件不存在则创建一个新的文件
void load_From_File(const char *filename)
{
    FILE *file = fopen(filename, "r"); // 以读取模式打开文件

    // 如果文件不存在，则创建文件并返回
    if (!file)
    {
        printf("File not found, creating new file: %s\n", filename);
        file = fopen(filename, "w"); // 创建新文件
        if (!file)                   // 创建失败
        {
            printf("Error creating new file!\n");
            return;
        }
        fclose(file); // 关闭文件
        return;
    }

    studentCount = 0; // 根据文件重新统计学生数量
    while (fscanf(file, "%lld %s %lld %s %lf", &students[studentCount].id, students[studentCount].name, &students[studentCount].age, students[studentCount].className, &students[studentCount].grade) == 5)
    {
        studentCount++; // 学生数量+1
    }
    fclose(file); // 关闭文件
}

// 条件查询学生信息（姓名、年龄、成绩）
void query_Students_By_Condition()
{
    char condition[50];
    printf("Enter condition (name, age, grade): "); // 选用姓名或年龄或成绩进行查询
    scanf("%s", condition);                         // 输入选项

    isQuerying = 1; // 设置状态为查询中
    cleardevice();  // 清除屏幕

    settextstyle(24, 0, "Consolas"); // 设置字体
    int y = 50;

    if (strcmp(condition, "name") == 0) // 按姓名查询
    {
        char name[50];
        printf("Enter name to search: ");
        scanf("%s", name);                     // 输入姓名
        for (int i = 0; i < studentCount; i++) // 遍历
        {
            if (strcmp(students[i].name, name) == 0)
            {
                char info[200];
                // 将学生信息格式化为字符串
                snprintf(info, sizeof(info), "ID: %lld Name: %s Age: %lld Class: %s Grade: %.2f",
                         students[i].id, students[i].name, students[i].age, students[i].className, students[i].grade);
                outtextxy(50, y, info); // 显示信息
                y += 50;                // 设置显示间隔
            }
        }
    }
    else if (strcmp(condition, "age") == 0) // 按年龄查询
    {
        long long age;
        printf("Enter age to search: ");
        scanf("%lld", &age);                   // 输入年龄
        for (int i = 0; i < studentCount; i++) // 遍历
        {
            if (students[i].age == age)
            {
                char info[200];
                // 将学生信息格式化为字符串
                snprintf(info, sizeof(info), "ID: %lld Name: %s Age: %lld Class: %s Grade: %.2f",
                         students[i].id, students[i].name, students[i].age, students[i].className, students[i].grade);
                outtextxy(50, y, info); // 显示信息
                y += 50;                // 设置显示间隔
            }
        }
    }
    else if (strcmp(condition, "grade") == 0) // 按成绩查询
    {
        double grade;
        printf("Enter grade to search: ");
        scanf("%lf", &grade);                  // 输入成绩
        for (int i = 0; i < studentCount; i++) // 遍历
        {
            if (students[i].grade == grade)
            {
                char info[200];
                // 将学生信息格式化为字符串
                snprintf(info, sizeof(info), "ID: %lld Name: %s Age: %lld Class: %s Grade: %.2f",
                         students[i].id, students[i].name, students[i].age, students[i].className, students[i].grade);
                outtextxy(50, y, info); // 显示信息
                y += 50;                // 设置显示间隔
            }
        }
    }
    else // 处理无效输入
    {
        printf("Invalid condition! Please enter 'name', 'age', or 'grade'.\n");
    }

    outtextxy(150, 535, "Press any key to continue..."); // 提示信息
    _getch();                                            // 按任意按键返回
    isQuerying = 0;                                      // 查询结束，恢复状态
    draw_Students();                                     // 绘制学生信息
}

// 条件排序学生信息（成绩、年龄、班级、姓名）
void sort_Students()
{
    char choice[10];
    printf("Sort by (grade/age/class/name): "); // 选用成绩或年龄或班级或姓名进行排序
    scanf("%s", choice);                        // 输入选项

    // 冒泡排序
    for (int i = 0; i < studentCount - 1; i++) // 遍历每一个学生
    {
        for (int j = i + 1; j < studentCount; j++) // 比较当前学生与后续学生之间的大小关系
        {
            int shouldSwap = 0; // 判断是否需要交换

            if (strcmp(choice, "grade") == 0) // 按成绩从高到低排序
            {
                shouldSwap = (students[i].grade < students[j].grade);
            }
            else if (strcmp(choice, "age") == 0) // 按年龄从小到大排序
            {
                shouldSwap = (students[i].age > students[j].age);
            }
            else if (strcmp(choice, "class") == 0) // 按班级排序
            {
                // 比较班级字符串字典序
                shouldSwap = (strcmp(students[i].className, students[j].className) > 0);
            }
            else if (strcmp(choice, "name") == 0) // 按姓名字母顺序排列
            {
                // 比较姓名字符串字典序
                shouldSwap = (strcmp(students[i].name, students[j].name) > 0);
            }
            else // 处理无效输入
            {
                printf("Invalid sorting option!\n");
                return;
            }

            if (shouldSwap) // 交换操作
            {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}

// 显示介绍界面
void show_Intro()
{
    cleardevice();                   // 清屏
    settextstyle(24, 0, "Consolas"); // 设置字体
    // 显示字体
    outtextxy(100, 100, "Welcome to Student Management System");
    outtextxy(100, 160, "F1: Add Student");
    outtextxy(100, 210, "F2: Query Students by Condition");
    outtextxy(100, 260, "F3: Sort Students (by Grade, Age, Class, or Name)");
    outtextxy(100, 310, "F4: Delete Student");
    outtextxy(100, 360, "F5: Modify Student");
    outtextxy(100, 410, "Esc: Exit");
    outtextxy(100, 460, "Press any key to continue...");
    _getch();
}

// 主循环
void main_loop()
{
    cleardevice();   // 清除屏幕
    draw_Students(); // 绘制学生信息
    while (1)
    {
        // 处理键盘输入
        if (_kbhit())
        {
            int key = _getch();
            if (key == 27) // 按下 ESC 退出
            {
                break;
            }
            else if (key == 0 || key == 224) // 检测功能键的前缀
            {
                key = _getch(); // 获取实际的功能键
                if (key == 59)  // 按 F1 添加学生信息
                {
                    add_Student();                // 添加学生信息
                    draw_Students();              // 绘制学生信息
                    save_To_File("students.txt"); // 保存到文件
                }
                else if (key == 60) // 按 F2 条件查询学生信息
                {
                    query_Students_By_Condition();
                }
                else if (key == 61) // 按 F3 排序学生信息
                {
                    sort_Students();              // 排序学生信息
                    draw_Students();              // 绘制学生信息
                    save_To_File("students.txt"); // 保存到文件
                }
                else if (key == 62) // 按 F4 删除学生信息
                {
                    delete_Student();             // 删除学生信息
                    draw_Students();              // 绘制学生信息
                    save_To_File("students.txt"); // 保存到文件
                }
                else if (key == 63) // 按 F5 修改学生信息
                {
                    modify_Student();             // 修改学生信息
                    draw_Students();              // 绘制学生信息
                    save_To_File("students.txt"); // 保存到文件
                }
            }
            else if (key == 72) // 上移 (向上箭头)
            {
                if (currentStartIndex > 0)
                {
                    currentStartIndex--;
                    draw_Students();
                }
            }
            else if (key == 80) // 下移 (向下箭头)
            {
                if (currentStartIndex < studentCount - STUDENTS_PER_PAGE)
                {
                    currentStartIndex++;
                    draw_Students();
                }
            }
        }

        // 处理鼠标滚轮输入
        ExMessage msg; // 存储鼠标事件的消息
        // 处理鼠标滚轮输入
        if (peekmessage(&msg, EM_MOUSE)) // 检查消息队列中是否有鼠标相关的消息
        {
            if (msg.message == WM_MOUSEWHEEL) // 检查是否是滚轮事件 
            {
                if (msg.wheel > 0 && currentStartIndex > 0) // 向上滚动，确保有更多的学生信息可以向上滚动 
                {
                    currentStartIndex--; // 显示的学生信息向上移动一个页面
                    draw_Students(); // 更新界面 
                }
                else if (msg.wheel < 0 && currentStartIndex < studentCount - STUDENTS_PER_PAGE) // 向下滚动
                {
                    currentStartIndex++; // 显示的学生信息向下移动一个页面
                    draw_Students(); // 更新界面 
                }
            }
        }
    }
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

// 调整控制台和 EasyX 窗口并排显示
void setWindowsPosition()
{
    // 获取当前控制台窗口句柄
    HWND consoleWindow = GetConsoleWindow();

    // 获取 EasyX 图形窗口句柄
    HWND easyxWindow = GetHWnd();

    // 定义窗口的宽度和高度
    int consoleWidth = 500;  // 控制台窗口宽度
    int consoleHeight = 500; // 控制台窗口高度
    int easyxWidth = 800;    // EasyX图形窗口宽度
    int easyxHeight = 600;   // EasyX图形窗口高度

    // 调整 EasyX 窗口的位置：放在屏幕左侧
    SetWindowPos(easyxWindow, NULL, 0, 100, easyxWidth, easyxHeight, SWP_NOZORDER | SWP_SHOWWINDOW);

    // 调整控制台窗口的位置：放在屏幕右侧（EasyX 窗口的右边）
    SetWindowPos(consoleWindow, NULL, easyxWidth + 50, 100, consoleWidth, consoleHeight, SWP_NOZORDER | SWP_SHOWWINDOW);

    // 让焦点转到控制台便于操作
    SetForegroundWindow(GetConsoleWindow());
}

// 主程序
int main()
{
    RunAsAdmin();                   // 获取管理员权限以修改控制台大小
    initgraph(WIDTH, HEIGHT);       // 初始化图形窗口
    setWindowsPosition();           // 调整控制台和 EasyX 窗口的位置
    show_Intro();                   // 显示介绍界面
    load_From_File("students.txt"); // 加载学生信息
    main_loop();                    // 开始循环
    closegraph();                   // 关闭图形窗口
    return 0;
}
//#This program is coded by Chen Sixiang#// 
