# WINAPI 学习笔记
- 10月14日
- 也是本人对曾经和最近学习的 `WINAPI` 内容的总结

## 切换输入法
- ver.1 (EasyX界面与控制台窗口)
```cpp
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
```
- ver.2 (仅控制台窗口)
- 切换英文输入法
```cpp
void SwitchInputLanguageToEnglish()
{
    // 加载并激活指定的键盘布局
    HKL hkl = LoadKeyboardLayoutW(L"00000409", KLF_ACTIVATE); // 英语（美国）输入法布局标识符

    // 发送消息以刷新输入法
    HWND hwnd = GetForegroundWindow(); // 获取当前活动窗口句柄
    if (hwnd != NULL)
    {
        PostMessage(hwnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)hkl);
    }
}
```
- 切换中文输入法
```cpp
void SwitchInputLanguageToChinese()
{
    // 加载并激活指定的键盘布局
    HKL hkl = LoadKeyboardLayoutW(L"00000804", KLF_ACTIVATE); // 简体中文输入法布局标识符

    // 发送消息以刷新输入法
    HWND hwnd = GetForegroundWindow(); // 获取当前活动窗口句柄
    if (hwnd != NULL)
    {
        PostMessage(hwnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)hkl);
    }
}
```

## 获得管理员权限
```cpp
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
```
## 模拟键盘事件

### 模拟键盘按键事件的函数
```cpp
    void simulateKey(WORD virtualKey, bool isPress)
    {
        INPUT input = {0};           // 初始化输入结构体
        input.type = INPUT_KEYBOARD; // 表示该输入事件是一个键盘事件
        input.ki.wVk = virtualKey;   // 设置虚拟按键码

        input.ki.dwFlags = isPress ? 0 : KEYEVENTF_KEYUP; // 判断是按下还是释放
        // 如果 isPress 为 true，则设置为 0，表示按下事件
        // 如果 isPress 为 false，则设置为 KEYEVENTF_KEYUP，表示释放事件

        SendInput(1, &input, sizeof(INPUT)); // 发送输入事件

        // 为了观察效果，添加一些延迟
        Sleep(100);
    }
```
### 模拟文本输入，区分大小写字母
```cpp
    void simulateTextInput(const string &text, TextEditor &editor)
    {
        for (size_t i = 0; i < text.size(); ++i) // 遍历随机生成的字符串
        {
            char ch = text[i];
            bool isUpperCase = isupper(ch);    // 判断是否是大写字母
            SHORT vk = VkKeyScan(tolower(ch)); // 获取对应的小写字母虚拟按键码
            BYTE vkCode = LOBYTE(vk);          // 提取虚拟按键码

            // 如果是大写字母，先按住 Shift 键
            if (isUpperCase)
            {
                simulateKey(VK_SHIFT, true); // 按下 Shift 键
            }

            showOperationWithDelay("模拟按键输入: " + string(1, ch), 500); // 显示操作信息
            appendLog("模拟按键输入: " + string(1, ch));                   // 将按键输入记录到日志
            simulateKey(vkCode, true);                                     // 按下按键
            Sleep(100);                                                    // 延迟，观察输入效果
            simulateKey(vkCode, false);                                    // 释放按键

            // 如果是大写字母，释放 Shift 键
            if (isUpperCase)
            {
                simulateKey(VK_SHIFT, false); // 释放Shift键
            }
            Sleep(100); // 在每个字符之间添加延迟
        }
    }
```
### 模拟Backspace键
```cpp
    void simulateBackspaceKey(TextEditor &editor)
    {
        showOperationWithDelay("模拟按下Backspace键", 1000); // 提示信息
        appendLog("按下Backspace键");
        simulateKey(VK_BACK, true);  // 按下Backspace键
        Sleep(100);                  // 延迟
        simulateKey(VK_BACK, false); // 释放Backspace键
        Sleep(100);                  // 延迟
    }
```
### 模拟 Ctrl + 指定按键 (例如 Ctrl + Z, Ctrl + C)
```cpp
 void simulateCtrlKey(WORD key, TextEditor &editor)
    {
        showOperationWithDelay("模拟按下Ctrl + " + string(1, key), 1000); // 提示信息
        appendLog("按下Ctrl + " + string(1, key));                        // 写进操作日志

        // 按下Ctrl键
        simulateKey(VK_CONTROL, true);
        // 按下指定键
        simulateKey(key, true);
        Sleep(100);              // 延迟
        simulateKey(key, false); // 释放指定键
        Sleep(100);              // 延迟
        // 释放Ctrl键
        simulateKey(VK_CONTROL, false);
    }
```
### 其它
- 类似的还有 `GetKeyState‎()` 获得键盘状态，`keybd_event()`、`mouse_event()`，分别模拟键盘事件和鼠标事件，可用 `SendIuput()` 来代替
#### 模拟按下和释放字母 A 键
```cpp
#include <windows.h>
int main()
{
    INPUT input = {0};

    // 设置键盘输入类型
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = 'A';  // 虚拟键码：A键

    // 模拟按下 'A' 键
    SendInput(1, &input, sizeof(INPUT));

    // 模拟释放 'A' 键
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));

    return 0;
}
```
#### 模拟鼠标移动和点击
```cpp
#include <windows.h>

int main()
{
    INPUT inputs[3] = {0};
    
    // 模拟鼠标移动到屏幕中央
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = 960 * 65535 / 1920;  // X 轴位置 (假设屏幕分辨率为 1920x1080)
    inputs[0].mi.dy = 540 * 65535 / 1080;  // Y 轴位置
    inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

    // 模拟鼠标左键按下
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // 模拟鼠标左键释放
    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    // 发送输入事件
    SendInput(3, inputs, sizeof(INPUT));

    return 0;
}
```
  
## 多线程
- 实现主循环函数 (处理键盘输入和文本操作) 和测试函数同时进行，测试函数监测按下 F1 按键的动作，按下后进入测试内容。
- 多线程进行使得测试的每步操作文本编辑器都能实时处理显示。
- 接收到 Esc 按键后，结束两个线程，退出程序。
### 示例伪代码
```cpp
#include <iostream>   // 标准输入输出
#include <conio.h>    // 用于 _getch() 函数，捕获键盘输入
#include <string>     // 用于字符串操作
#include <windows.h>  // 用于清除控制台输出和睡眠函数
#include <stdexcept>  // 用于 std::out_of_range
#include <time.h>     // 用于随机数
#include <thread>     // 用于多线程
#include <fstream>    // 用于操作文件
#include <atomic>     // 用于原子变量

// 操作记录结构体
struct Operation
{
    ```
};

// 自定义栈类
template <typename T>
class Stack
{
private:
    ```

public:
    ```
};

// 文本编辑器类
class TextEditor
{
private:
    ```

public:
    ```
};

// 获得管理员权限以修改控制台窗口
void RunAsAdmin()
{
    ```
}

// 处理 Ctrl + C 事件，避免程序退出
BOOL WINAPI ConsoleHandler(DWORD signal)
{
    ```
}

// 切换英文输入法
void SwitchInputLanguageToEnglish()
{
    ```
}

// 切换中文输入法
void SwitchInputLanguageToChinese()
{
    ```
}

// 线程控制的全局变量 多线程环境
atomic<bool> running(true); // 控制主循环是否运行

// 测试编辑器的功能
void runTest(TextEditor &editor)
{

    ```
}

// 主循环函数，用于处理键盘输入和文本操作
void mainLoop(TextEditor &editor)
{
    ```
        if (ch == 27)
        {                    // 按下 ESC 键退出
            running = false; // 停止所有线程
            break;
        }
    ```    
}

// 主程序
int main()
{
    RunAsAdmin();                                // 以管理员身份运行
    SwitchInputLanguageToEnglish();              // 切换英文输入法
    SetConsoleCtrlHandler(ConsoleHandler, TRUE); // 设置控制台控制处理程序

    TextEditor editor; // 创建文本编辑器实例

    thread testThread(runTest, ref(editor));  // 启动测试功能线程
    thread mainThread(mainLoop, ref(editor)); // 启动主进程

    testThread.join(); // 等待测试线程结束
    mainThread.join(); // 等待主进程结束

    return 0;
}
```
## 调整控制台窗口和 EasyX 窗口并列显示
```cpp
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
```
## 处理 Ctrl + C 事件，避免程序退出
```cpp
BOOL WINAPI ConsoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT) // 获取到 Ctrl + C 事件
    {
        return TRUE; // 返回 TRUE 表示已处理信号
    }
    return FALSE; // 返回 FALSE 表示未处理信号
}
```
## 获取当前控制台窗口的句柄
```cpp
HWND GetConsoleHwnd(void)
{
    #define MY_BUFSIZE 1024  // 定义缓冲区大小为 1024 字节
    HWND hwndFound;          // 用于存储找到的窗口句柄
    char pszNewWindowTitle[MY_BUFSIZE];  // 用于存储新生成的唯一窗口标题
    char pszOldWindowTitle[MY_BUFSIZE];  // 用于存储当前窗口的原始标题

    // 获取当前控制台窗口的标题，并存储在 pszOldWindowTitle 缓冲区中
    GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

    // 创建一个独特的新标题，使用系统运行时间和当前进程 ID 进行格式化，保证其唯一性
    wsprintf(pszNewWindowTitle, "%d/%d", GetTickCount(), GetCurrentProcessId());

    // 将控制台窗口的标题设置为刚才生成的唯一标题
    SetConsoleTitle(pszNewWindowTitle);

    // 程序暂停 40 毫秒，确保系统有足够时间更新窗口的标题
    Sleep(40);

    // 查找具有该唯一标题的窗口，并将其句柄存储在 hwndFound 中
    hwndFound = FindWindow(NULL, pszNewWindowTitle);

    // 将窗口标题恢复为原始的标题
    SetConsoleTitle(pszOldWindowTitle);

    // 返回找到的窗口句柄
    return(hwndFound);
}
```
## 禁用窗口最小化和关闭按钮
```cpp
void Disable_Button(HWND hwnd)
{ 
    // 获取窗口的系统菜单，传递 false 表示不重置系统菜单
    // 系统菜单包含关闭、最大化、最小化等选项
    HMENU hmenu = GetSystemMenu(hwnd, false);

    // 从系统菜单中移除关闭按钮，SC_CLOSE 表示关闭选项，MF_BYCOMMAND 表示通过命令删除
    RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);

    // 获取窗口的当前样式（GWL_STYLE 获取窗口的长窗口样式）
    LONG style = GetWindowLong(hwnd, GWL_STYLE);

    // 修改窗口样式，屏蔽最小化按钮，~(WS_MINIMIZEBOX) 是取反操作
    // WS_MINIMIZEBOX 是窗口样式标志，用于启用/禁用最小化按钮
    style &= ~(WS_MINIMIZEBOX); // 屏蔽关闭和最小化按钮，使它们无法使用

    // 获取窗口的扩展样式，GWL_EXSTYLE 用于获取扩展样式
    DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

    // 修改窗口的扩展样式，WS_EX_TOOLWINDOW 表示工具窗口样式
    // 工具窗口通常不会在 ALT+TAB 窗口切换列表中显示
    dwExStyle |= WS_EX_TOOLWINDOW; // 使窗口在 ALT+TAB 列表中不可见

    // 获取窗口的扩展样式，用于后续的进一步修改
    LONG styleValue = GetWindowLong(hwnd, GWL_EXSTYLE);

    // 启用分层窗口样式，WS_EX_LAYERED 允许窗口进行透明度和颜色键等操作
    styleValue |= WS_EX_LAYERED;

    // 启用透明窗口样式，WS_EX_TRANSPARENT 使窗口可以被鼠标穿透
    // 这样，可以点击窗口背后的内容，而不会阻挡鼠标事件
    dwExStyle |= WS_EX_TRANSPARENT;

    // 应用修改后的扩展样式，更新窗口的扩展样式设置
    SetWindowLong(hwnd, GWL_EXSTYLE, styleValue);

    // 设置窗口的分层属性，RGB(r, g, b) 表示颜色，180 是透明度（0 完全透明，255 完全不透明）
    // LWA_ALPHA 标志表示设置的是透明度
    SetLayeredWindowAttributes(hwnd, RGB(r,g,b), 180, LWA_ALPHA); // 设置窗口为半透明

    // 更新窗口的扩展样式，去掉 WS_EX_APPWINDOW 标志，使窗口从任务栏中隐藏
    SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle & ~WS_EX_APPWINDOW);

    // 调整窗口的 Z 顺序，保持在最前面（HWND_TOP），并且不改变窗口的位置和大小
    SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // 应用之前修改后的窗口样式（无最小化按钮等），更新窗口样式
    SetWindowLong(hwnd, GWL_STYLE, style);

    // 以正常状态显示窗口，SW_SHOWNORMAL 表示显示窗口并激活它
    ShowWindow(hwnd, SW_SHOWNORMAL);

    // 销毁系统菜单，防止后续对菜单的访问
    DestroyMenu(hmenu);

    // 释放与窗口相关的设备上下文（DC），NULL 表示不返回任何特定的上下文
    ReleaseDC(hwnd, NULL);
}
```
## 程序关闭自动打开
```cpp
BOOL WINAPI HandlerRoutine(DWORD dwCtrlType) 
// 定义控制处理程序，处理控制台接收到的控制信号
// dwCtrlType：接收到的控制信号类型，表示触发该处理程序的事件
{
    if(CTRL_CLOSE_EVENT == dwCtrlType) 
    // 检查控制信号类型是否为关闭事件，即用户试图关闭控制台窗口
    {
        ShellExecuteA(NULL, "open", "返校倒计时.exe", 0, 0, 0); 
        // 调用ShellExecuteA函数来重新启动“返校倒计时.exe”程序
        // NULL：表示没有父窗口
        // "open"：指定要执行的操作，这里是打开程序
        // "返校倒计时.exe"：要打开的程序的文件名
        // 0：表示不传递任何参数
        // 0：表示使用默认工作目录
        // 0：表示窗口的显示方式，此处为默认
    }
}
```
## 防止程序多开
```cpp
HANDLE hMutex = CreateMutex(NULL, TRUE, tc); 
// 创建一个命名互斥量来保证程序的单实例运行
// 参数说明：
// NULL：默认的安全属性
// TRUE：当前线程在创建时获得互斥量的所有权，防止其他实例运行
// tc：命名互斥量，用于唯一标识该程序实例

if(GetLastError() == ERROR_ALREADY_EXISTS) 
// 检查是否已经存在同名互斥量，即判断是否已有程序实例正在运行
{
    if(hMutex) 
    {
        CloseHandle(hMutex); 
        // 如果互斥量有效，则关闭该互斥量的句柄以释放资源

        MessageBox(NULL, TC, WAR, 0); 
        // 弹出消息框，通知用户程序已经在运行
        // TC：消息框的内容，通常是错误或警告信息
        // WAR：消息框标题，可能为预定义的字符串“警告”等
        // 0：默认消息框样式

        return 0; 
        // 结束当前程序，避免重复运行多个实例
    }
}
```

## 把程序写进注册表启动项
```cpp
void SET_REG()
{
    GetPrivilege(); 
    // 调用 GetPrivilege 函数以确保程序拥有足够的权限进行注册表操作
    // 该函数通常用于提升程序权限，确保能够访问 HKEY_LOCAL_MACHINE 下的注册表
    HKEY hKey;  // 声明一个 HKEY 类型的变量，用于存储注册表键的句柄
    // 定义一个字符串常量，表示要访问的注册表路径
    LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";  
    // 此路径用于 Windows 启动项的注册表位置，程序会在用户登录时自动运行

    // 尝试打开指定的注册表键，使用 HKEY_LOCAL_MACHINE 访问本地计算机的注册表
    // lpRun 是要打开的键的路径，0 表示从该键的顶层开始搜索
    // KEY_WRITE 表示打开该键以进行写操作
    long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);  

    // 检查打开注册表键是否成功
    if(lRet == ERROR_SUCCESS)  
    {  
        char pFileName[MAX_PATH] = {0};  // 定义字符数组，用于存放程序的完整路径，最大长度为 MAX_PATH
        // 使用 GetModuleFileName 函数获取当前程序的完整路径
        // 第一个参数为 NULL，表示获取当前进程的模块路径
        DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);  

        // 使用 RegSetValueEx 函数在注册表中添加一个子键
        // 第一个参数是打开的注册表句柄 hKey
        // 第二个参数是要设置的值的名称，此处为 "返校倒计时"（程序名，不加后缀）
        // 第三个参数为保留参数，通常设置为 0
        // 第四个参数是数据类型，这里为 REG_SZ 表示字符串类型
        // 第五个参数是指向要存储的数据的指针，这里是 pFileName，表示程序路径
        // 第六个参数是数据大小，这里使用 dwRet，表示程序路径的长度
        lRet = RegSetValueEx(hKey, "返校倒计时", 0, REG_SZ, (BYTE *)pFileName, dwRet);  

        // 关闭注册表键，释放句柄
        RegCloseKey(hKey);  
    }  
}
```
## 把写进注册表的启动项删除
```cpp
void DELETE_REG()
{
    GetPrivilege(); 
    // 调用 GetPrivilege 函数以确保程序拥有足够的权限进行注册表操作
    // 该函数通常用于提升程序权限，确保能够访问 HKEY_LOCAL_MACHINE 下的注册表

    HKEY hKEY; // 声明一个 HKEY 类型的变量，用于存储打开的注册表键的句柄

    // 尝试打开指定的注册表键，访问路径为 "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
    // 0 表示从该键的顶层开始搜索，KEY_SET_VALUE 表示以写权限打开该键
    if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKEY))
    {
        // 如果成功打开注册表键，则尝试删除指定的键值
        // 使用 RegDeleteValue 函数删除键值 "返校倒计时"
        if(ERROR_SUCCESS == RegDeleteValue(hKEY, "返校倒计时"))
        {
            // 如果删除成功，打印成功信息到控制台
            printf("删除键值 '返校倒计时' 成功.\n");
        }
        // 关闭打开的注册表键，释放句柄
        RegCloseKey(hKEY);
    } 
    return; // 返回，结束函数执行
}
```
## 根据给定的进程名称获取相应进程的进程 ID
```cpp
DWORD GetProcessidFromName(LPCTSTR name)
{
    // 定义一个 PROCESSENTRY32 结构，用于存储进程信息
    PROCESSENTRY32 pe;
    DWORD id = 0; // 初始化进程 ID 为 0，表示未找到
    // 创建一个快照，获取当前系统中所有进程的信息
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    // 设置结构的大小，确保结构体正确
    pe.dwSize = sizeof(PROCESSENTRY32);
    
    // 获取第一个进程的信息，如果失败则返回 0
    if (!Process32First(hSnapshot, &pe)) return 0;

    // 循环遍历进程快照中的所有进程
    while (1)
    {
        // 更新结构的大小，以确保在遍历过程中使用的结构体始终有效
        pe.dwSize = sizeof(PROCESSENTRY32);
        
        // 获取下一个进程的信息，如果失败则跳出循环
        if (Process32Next(hSnapshot, &pe) == FALSE)
            break;

        // 比较当前进程的可执行文件名与传入的名称
        if (strcmp(pe.szExeFile, name) == 0)
        {
            // 如果找到匹配的进程，获取其进程 ID
            id = pe.th32ProcessID;
            break; // 找到后跳出循环
        }
    }
    
    // 关闭进程快照句柄，释放资源
    CloseHandle(hSnapshot);
    
    // 返回找到的进程 ID，如果未找到则返回 0
    return id;
}
```
## 生成包含命令的字符串 
- `instsrv GameManagerServer` + 程序的目录路径，也就是生成把程序写进 `GameManagerServer` 服务的指令字符串。

### 获取程序的完整路径
```cpp
string GetProgramDir()  // 完整路径
{   
    char exeFullPath[MAX_PATH]; // 用于存储完整路径的字符数组
    string strPath = "";        // 初始化一个空的字符串，用于保存路径
 
    // 获取当前模块（即运行程序）的完整文件路径
    GetModuleFileName(NULL, exeFullPath, MAX_PATH);
    
    // 将字符数组转换为字符串
    strPath = (string)exeFullPath;    
   
    // 返回包含文件名的完整路径
    return strPath;  
    // return strPath.substr(0, pos);  // 如果需要返回不包含文件名的路径，可以使用这行代码
}   
```
### 获取程序的名称
```cpp
string GetProgramName()  // 程序名称
{
    char exename[MAX_PATH]; // 用于存储程序名称的字符数组
    string strname = "";    // 初始化一个空的字符串，用于保存程序名称

    // 获取当前模块（即运行程序）的完整文件路径
    GetModuleFileName(NULL, exename, MAX_PATH);
    
    // 将字符数组转换为字符串
    strname += (string)exename; 
    
    // 查找路径中最后一个反斜杠的位置
    int pos = strname.find_last_of('\\', strname.length());
    
    // 创建一个新的字符串以保存真实的程序名称
    string REAL;
    REAL += "\""; // 添加开始的双引号
    REAL += strname.substr(pos + 1, strname.length()); // 从反斜杠后获取文件名
    REAL += "\""; // 添加结束的双引号
    
    // 返回程序名称，包含双引号
    return REAL;	
}
```
### 获取程序的目录路径（不包括文件名）
```cpp
string GetProgramFuck()  // 目录路径不包括文件名
{   
    char exeFullPath[MAX_PATH]; // 用于存储完整路径的字符数组
    string strPath = "";        // 初始化一个空的字符串，用于保存路径
 
    // 获取当前模块（即运行程序）的完整文件路径
    GetModuleFileName(NULL, exeFullPath, MAX_PATH);
    
    // 将字符数组转换为字符串
    strPath = (string)exeFullPath; 
    
    // 查找路径中最后一个反斜杠的位置
    int pos = strPath.find_last_of('\\', strPath.length());
    
    // 返回不包含文件名的目录路径
    return strPath.substr(0, pos);  
}   
```
### 生成一个包含程序目录的命令字符串
```cpp
string path()
{
    string str;  // 初始化一个空的字符串

    // 拼接字符串，生成一个命令
    str += "\"instsrv GameManagerServer "; // 命令的开头
    str += GetProgramDir();                // 获取程序目录并添加到命令字符串
    str += "\"";                           // 添加命令字符串的结束引号
    
    // 返回生成的命令字符串
    return str;
}
```
