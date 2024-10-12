# WINAPI 学习笔记
- 10月13日

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
    if (signal == CTRL_C_EVENT)
    {
        return TRUE; // 返回 TRUE 表示已处理信号
    }
    return FALSE; // 返回 FALSE 表示未处理信号
}
```
