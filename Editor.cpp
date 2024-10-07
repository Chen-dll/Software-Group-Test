//#This program is coded by Chen Sixiang#//
//#Program Name：文本编辑器#//
//#Finish Date: 2024.10.7#//
//#Environment: Dev-C++ 5.11#//
//#详细设计思路请查看Markdown文档#//
#include <iostream>   // 标准输入输出
#include <conio.h>    // 用于 _getch() 函数，捕获键盘输入
#include <string>     // 用于字符串操作
#include <windows.h>  // 用于清除控制台输出和睡眠函数
#include <stdexcept>  // 用于 std::out_of_range
#include <time.h>     // 用于随机数
#include <thread>     // 用于多线程
#include <fstream>    // 用于操作文件
#include <atomic>     // 用于原子变量
#include <graphics.h> // EasyX 图形库头文件

#define SIZE 1024 // SIZE

using namespace std;

const int MAX_LOG_SIZE = 1024; // 定义最大日志容量

char operationLogs[MAX_LOG_SIZE]; // 全局操作日志数组 用来存储操作日志
int logIndex = 0;                 // 当前日志的写入位置
int IsTest = 0;                   // 记录测试状态 用于判断清屏

// 操作记录结构体
struct Operation
{
    int type;          // 操作类型（1 表示插入，0 表示删除）
    char character;    // 被插入或删除的字符
    string pastedText; // 被粘贴的文本（仅在粘贴操作中使用）

    // 默认构造函数，初始化 character 为 '\0'（空字符）
    Operation() : type(0), character('\0'), pastedText("") {}
    // 带参数的构造函数
    Operation(int t, char c, const string &text = "") : type(t), character(c), pastedText(text) {}
};

// 自定义栈类
template <typename T>
class Stack
{
private:
    T *data;      // 动态数组存储栈元素
    int capacity; // 栈的容量
    int top;      // 栈顶索引

    // 动态扩展栈的容量
    void resize()
    {
        int newCapacity = capacity * 2;  // 扩展为原来的两倍容量
        T *newData = new T[newCapacity]; // 分配新空间

        // 复制旧数据到新数组
        for (int i = 0; i <= top; ++i)
        {
            newData[i] = data[i];
        }

        // 删除旧数组，切换到新数组
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // 构造函数
    Stack(int size = SIZE) : capacity(size), top(-1)
    {
        data = new T[capacity]; // 创建动态数组
    }

    // 析构函数
    ~Stack()
    {
        delete[] data; // 释放动态数组
    }

    // 推入元素到栈
    void push(const T &value)
    {
        // 检查是否需要扩展容量
        if (top >= capacity - 1)
        {
            resize(); // 动态扩展容量
        }
        data[++top] = value; // 增加栈顶索引并赋值
    }

    // 从栈中弹出元素
    T pop()
    {
        if (isEmpty())
        {
            cerr << "栈为空，无法弹出元素！" << endl;
            throw out_of_range("栈为空"); // 抛出异常信息
        }
        return data[top--]; // 返回栈顶元素并减少栈顶索引
    }

    // 查看栈顶元素
    T peek() const
    {
        if (isEmpty())
        {
            cerr << "栈为空，无法查看栈顶元素！" << endl;
            throw out_of_range("栈为空"); // 抛出异常信息
        }
        return data[top]; // 返回栈顶元素
    }

    // 检查栈是否为空
    bool isEmpty() const
    {
        return top == -1; // 栈顶索引为-1时栈为空
    }

    // 获取栈的大小
    int size() const
    {
        return top + 1; // 栈的大小为栈顶索引加1
    }

    // 复制栈内容到字符串
    string toString() const
    {
        string result;
        // 通过遍历操作将当前栈的内容存储到临时栈
        for (int i = 0; i <= top; ++i)
        {
            result += data[i]; // 逐个构建字符串
        }
        return result; // 返回字符串
    }
};

// 文本编辑器类
class TextEditor
{
private:
    Stack<char> textStack;      // 文本栈，存储文本内容
    Stack<Operation> undoStack; // 操作栈，存储撤销操作
    Stack<Operation> redoStack; // 重做操作栈
    string clipboard;           // 剪贴板字符串

public:
    // 插入字符函数
    void insertChar(char c)
    {
        textStack.push(c);
        undoStack.push({1, c}); // 1 表示插入操作
        while (!redoStack.isEmpty())
            redoStack.pop(); // 清空重做栈
    }

    // 删除字符函数
    bool deleteChar() // 返回是否成功删除
    {
        if (!textStack.isEmpty()) // 判断文本栈是否为空
        {
            char deletedChar = textStack.pop(); // 删除字符
            undoStack.push({0, deletedChar});   // 0 表示删除操作
            while (!redoStack.isEmpty())        // 清空重做栈
                redoStack.pop();
            return true; // 删除成功
        }
        else
        {
            return false; // 删除失败
        }
    }

    // 撤销操作函数
    void undo()
    {
        if (!undoStack.isEmpty()) // 判断撤销栈是否为空
        {
            Operation lastOperation = undoStack.pop(); // 获取最后一项操作
            if (lastOperation.type == 1)               // 1 表示插入操作，
            {
                textStack.pop(); // 撤销时删除字符
            }
            else if (lastOperation.type == 0) // 0 表示删除操作
            {
                textStack.push(lastOperation.character); // 撤销时恢复字符
            }
            else if (lastOperation.type == 2) // 2 表示粘贴操作，撤销时删除粘贴的文本
            {
                // 直接删除粘贴的文本，而不是逐个字符删除
                size_t lengthToDelete = lastOperation.pastedText.size();
                for (size_t i = 0; i < lengthToDelete; ++i)
                {
                    if (!textStack.isEmpty()) // 确保不超出文本栈的范围
                        textStack.pop();      // 删除字符
                }
            }
            redoStack.push(lastOperation); // 将撤销操作压入重做栈
        }
        else
        {
            showErrorWithDelay("没有可以撤销的操作！", 1000); // 提示用户没有可撤销操作
        }
    }

    // 重做操作函数
    void redo()
    {
        if (!redoStack.isEmpty()) // 判断重做栈是否为空
        {
            Operation lastOperation = redoStack.pop(); // 获取最后一项重做操作
            if (lastOperation.type == 1)               // 1 表示插入操作
            {
                textStack.push(lastOperation.character);
            }
            else if (lastOperation.type == 0) // 0 表示删除操作
            {
                textStack.pop();
            }
            else if (lastOperation.type == 2) // 2 表示粘贴操作，重做时添加粘贴的文本
            {
                const string &textToRedo = lastOperation.pastedText; // 读取粘贴的文本
                // 反向插入粘贴的文本，确保顺序正确
                for (size_t i = lastOperation.pastedText.size(); i > 0; --i)
                {
                    textStack.push(textToRedo[i - 1]); // 从后向前插入字符
                }
            }
            undoStack.push(lastOperation); // 将重做的操作压回撤销栈
        }
        else
        {
            showErrorWithDelay("没有可以重做的操作！", 1000); // 提示用户没有可重做操作
        }
    }

    // 复制到剪贴板
    void copy()
    {
        clipboard.clear();     // 清空剪贴板
        Stack<char> tempStack; // 临时栈，保持文本栈完整

        // 将文本内容复制到剪贴板
        // 遍历文本栈，将内容逐个取出并添加到剪贴板
        while (!textStack.isEmpty())
        {
            char c = textStack.pop();
            clipboard += c;    // 添加到剪贴板
            tempStack.push(c); // 将内容保存到临时栈
        }

        // 将内容放回原栈，确保文本栈的完整
        while (!tempStack.isEmpty())
        {
            textStack.push(tempStack.pop());
        }

        showErrorWithDelay("文本已复制到剪贴板！", 1000); // 提示信息
    }

    // 从剪贴板粘贴
    void paste()
    {
        if (clipboard.empty()) // 判断剪贴板是否为空
        {
            showErrorWithDelay("剪贴板为空，无法粘贴！", 1000);
            return;
        }

        // 先保存当前文本内容的快照，便于撤销和重做操作
        size_t originalSize = textStack.size();

        // 将剪贴板内容逐个插入到文本堆栈中 反向遍历保证粘贴的顺序正确
        for (size_t i = clipboard.size(); i > 0; --i)
        {
            textStack.push(clipboard[i - 1]);
        }

        // 将整个粘贴操作记录在撤销栈中
        Operation pasteOperation;
        pasteOperation.type = 2;               // 2 表示粘贴操作
        pasteOperation.pastedText = clipboard; // 记录粘贴的文本

        undoStack.push(pasteOperation); // 将粘贴操作添加到撤销栈

        // 清空重做栈
        while (!redoStack.isEmpty())
            redoStack.pop();
    }

    // 显示当前文本内容
    void displayText()
    {
        static int lastLineCount = 0;                                     // 跟踪上一次显示的行数
        COORD coord = {0, 0};                                             // 坐标 (0, 0)
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // 设置光标位置

        string currentText = textStack.toString(); // 获取栈内容并转换为字符串

        // 计算输出行数 第一行特例分开算
        int totalLines;
        if (currentText.length() < 111)
            totalLines = 1;
        else
            totalLines = ((currentText.length() - 111) / 120) + 2; // 以本人调试窗口大小为准

        // 如果当前行数与上次行数不同，则进行清屏操作，同样是为了缓解闪屏
        if (totalLines != lastLineCount)
        {
            system("cls");              // 清屏操作
            lastLineCount = totalLines; // 更新上一次的行数
        }

        // 测试操作显示完操作日志后清屏
        if (IsTest)
        {
            system("cls"); // 清屏
            IsTest = 0;    // 恢复，以便多次测试
        }

        cout << string(100, ' ') << "\r当前文本: "; // 清除当前行
        cout << currentText;                        // 输出文本

        COORD newCoord = {0, static_cast<SHORT>(totalLines + 1)};            // 在文本下方输出
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newCoord); // 设置光标位置

        // 输出按键介绍和报错提示
        cout << "删除(Backspace), 复制(Ctrl+C), 粘贴(Ctrl+V), 撤销操作(Ctrl+Z), 重做操作(Ctrl+Y), 测试(F1), 退出(Esc)" << endl;

        // 设置光标位置到文本末尾
        short newCoord_x, newCoord_y = totalLines;
        if (totalLines == 1)
            newCoord_x = currentText.length() + 9; // 因为第一行有 "当前文本："
        else
            newCoord_x = (currentText.length() - 111) % 120;                 // 后面正常计算
        newCoord = {newCoord_x, newCoord_y};                                 // 计算末尾坐标
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newCoord); // 设置光标位置
    }

    // 显示错误信息
    void displayError(const string &message)
    {
        string currentText = textStack.toString(); // 获取栈内容并转换为字符串
        int totalLines;                            // 同上操作
        if (currentText.length() < 111)
            totalLines = 1;
        else
            totalLines = ((currentText.length() - 111) / 120) + 2;           // 以本人调试窗口大小为准
        COORD newCoord = {0, static_cast<SHORT>(totalLines + 1)};            // 在文本下方输出
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newCoord); // 设置光标位置
        cout << string(100, ' ');                                            // 清除当前行内容
        cout << "\r" << message;                                             // 输出错误信息
        cout.flush();                                                        // 刷新输出流
    }

    // 延长错误信息显示时间
    void showErrorWithDelay(const string &message, int delay)
    {
        displayError(message); // 显示错误信息
        Sleep(delay);          // 延迟指定时间
        displayError("");      // 清除错误信息
        displayText();         // 在文本下面重新输出按键介绍
    }

    // 设置输出测试信息时光标位置
    void setTestCursor()
    {
        // 获取栈内容并转换为字符串
        string currentText = textStack.toString();

        // 计算输出行数 同上操作
        int totalLines;
        if (currentText.length() < 111)
            totalLines = 1;
        else
            totalLines = ((currentText.length() - 111) / 120) + 2; // 以本人调试窗口大小为准

        COORD newCoord = {0, static_cast<SHORT>(totalLines + 2)};            // 在文本下方输出
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newCoord); // 设置光标位置
    }

    // 清空文本栈
    void clearTextStack()
    {
        while (!textStack.isEmpty())
        {
            textStack.pop(); // 删除字符
        }
    }

    // 延长操作信息显示时间
    void showOperationWithDelay(const string &message, int delay)
    {
        setTestCursor(); // 把光标移到文本下方以输出操作信息
        cout << "\r" << message;
        cout.flush();
        Sleep(delay); // 延时
        cout << "\r" << string(100, ' ') << "\r";
        displayText(); // 重新显示文本
    }

    // 追加日志到日志数组
    void appendLog(const string &log)
    {
        if (logIndex + log.size() >= MAX_LOG_SIZE) // 检验日志容量
        {
            cout << "日志已满，无法继续写入。" << endl; // 日志已满就提示
            return;
        }
        for (size_t i = 0; i < log.size(); ++i) // 将日志字符串中的每个字符逐个存储到数组中
        {
            operationLogs[logIndex++] = log[i];
        }
        operationLogs[logIndex++] = '\n'; // 每条日志后添加换行符
    }

    // 将日志写入文件
    void saveLogToFile(const string &filename, TextEditor &editor)
    {
        ofstream outFile(filename.c_str(), ios::app); // 创建一个输出文件流对象
        if (!outFile)                                 // 检查文件是否成功打开
        {
            editor.setTestCursor();                       // 移动光标到适当位置
            cerr << "无法打开文件: " << filename << endl; // 提示信息
            return;
        }

        outFile.write(operationLogs, logIndex);           // 将日志内容写入文件
        outFile.close();                                  // 关闭文件
        editor.setTestCursor();                           // 移动光标到适当位置
        cout << "日志已保存到文件: " << filename << endl; // 提示信息
    }

    // 模拟键盘按键事件的函数
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

    // 模拟文本输入，区分大小写字母
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

    // 模拟Backspace键
    void simulateBackspaceKey(TextEditor &editor)
    {
        showOperationWithDelay("模拟按下Backspace键", 1000); // 提示信息
        appendLog("按下Backspace键");
        simulateKey(VK_BACK, true);  // 按下Backspace键
        Sleep(100);                  // 延迟
        simulateKey(VK_BACK, false); // 释放Backspace键
        Sleep(100);                  // 延迟
    }

    // 模拟 Ctrl + 指定按键 (例如 Ctrl + Z, Ctrl + C)
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

    // 生成随机测试文本
    string generateRandomText(int length)
    {
        // 设置用来随机生成的字符
        const string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        string randomText; // 存储生成的字符串
        for (int i = 0; i < length; ++i)
        {
            randomText += characters[rand() % characters.size()]; // 使用rand生成随机字符
        }
        return randomText; // 返回随机生成的字符串
    }
};

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

// 处理 Ctrl + C 事件，避免程序退出
BOOL WINAPI ConsoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT)
    {
        return TRUE; // 返回 TRUE 表示已处理信号
    }
    return FALSE; // 返回 FALSE 表示未处理信号
}

// 切换英文输入法
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

// 切换中文输入法
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

// 线程控制的全局变量 多线程环境
atomic<bool> running(true); // 控制主循环是否运行

// 测试编辑器的功能
void runTest(TextEditor &editor)
{

    // 监测键盘事件 按下 F1就进入测试操作
    while (running)
    {
        if (_kbhit() && _getch() == 59) // F1 键的 ASCII 值是 59
        {

            srand(time(0));                                                          // 初始化随机数种子
            string testText = editor.generateRandomText(6);                          // 生成6个字符的随机文本
            editor.clearTextStack();                                                 // 清空文本栈
            editor.setTestCursor();                                                  // 设置光标位置
            system("cls");                                                           // 清屏
            editor.displayText();                                                    // 重新显示文本
            editor.showOperationWithDelay("F1 键被按下，开始运行测试功能...", 1000);   // 提示信息

            editor.simulateTextInput(testText, editor); // 模拟输入文本
            SwitchInputLanguageToChinese();             // 切换中文输入法使得日志文件正常显示
            editor.appendLog("输入文本: " + testText);  // 记录到日志
            Sleep(1000);                                // 每次操作之间暂停1秒

            // 先四个操作按顺序模拟一次 再进行随机的五个操作
            // 模拟复制操作 Ctrl + C
            editor.simulateCtrlKey('C', editor);
            editor.appendLog("执行复制操作"); // 记录到日志
            Sleep(1000);                      // 每次操作之间暂停1秒

            // 模拟粘贴操作 Ctrl + V
            editor.simulateCtrlKey('V', editor);
            editor.appendLog("执行粘贴操作"); // 记录到日志
            Sleep(1000);                      // 每次操作之间暂停1秒

            // 模拟撤销操作 Ctrl + Z
            editor.simulateCtrlKey('Z', editor);
            editor.appendLog("执行撤销操作"); // 记录到日志
            Sleep(1000);                      // 每次操作之间暂停1秒

            // 模拟重做操作 Ctrl + Y
            editor.simulateCtrlKey('Y', editor);
            editor.appendLog("执行重做操作"); // 记录到日志
            Sleep(1000);                      // 每次操作之间暂停1秒

            int executedCount = 0;                                            // 记录操作次数
            bool operationsExecuted[5] = {false, false, false, false, false}; // 记录5个操作是否已执行

            while (executedCount < 5) // 确保 5个操作都执行过一次
            {
                int randomOperation = rand() % 5; // 生成 0-4 之间的随机数

                if (!operationsExecuted[randomOperation]) // 如果该操作尚未执行
                {
                    switch (randomOperation)
                    {
                    case 0:
                        editor.simulateBackspaceKey(editor);
                        editor.appendLog("执行删除操作");
                        break;
                    case 1:
                        editor.simulateCtrlKey('C', editor);
                        editor.appendLog("执行复制操作");
                        break;
                    case 2:
                        editor.simulateCtrlKey('V', editor);
                        editor.appendLog("执行粘贴操作");
                        break;
                    case 3:
                        editor.simulateCtrlKey('Z', editor);
                        editor.appendLog("执行撤销操作");
                        break;
                    case 4:
                        editor.simulateCtrlKey('Y', editor);
                        editor.appendLog("执行重做操作");
                        break;
                    }
                    operationsExecuted[randomOperation] = true; // 标记该操作为已执行
                    executedCount++;                            // 操作次数+1
                    Sleep(1000);                                // 每次操作之间暂停1秒
                }
            }

            IsTest = 1;             // 完成测试操作，清屏重新显示
            editor.setTestCursor(); // 移动光标到适当位置
            // 输出操作日志
            cout << "操作日志:" << endl;
            cout.write(operationLogs, logIndex);               // 打印操作日志内容
            editor.saveLogToFile("operation_log.txt", editor); // 保存日志内容到文件
            memset(operationLogs, 0, sizeof(operationLogs));   // 减少不必要的显示错误
            editor.clearTextStack();                           // 清空文本栈
            SwitchInputLanguageToEnglish();                    // 换回英文输入法
            this_thread::sleep_for(chrono::milliseconds(500)); // 暂停测试，防止连续触发
        }
        Sleep(100); // 稍作延迟以减少 CPU 使用
    }
}

// 主循环函数，用于处理键盘输入和文本操作
void mainLoop(TextEditor &editor)
{
    cout << "简单文本编辑器\n\n";
    cout << "删除(Backspace), 复制(Ctrl+C), 粘贴(Ctrl+V), 撤销操作(Ctrl+Z), 重做操作(Ctrl+Y), 测试(F1), 退出(Esc)" << endl;
    cout << "按下任意键继续";
    _getch();
    // 清屏重新显示
    system("cls");
    editor.displayText();
    while (running)
    {
        char ch = _getch(); // 捕获用户输入的字符

        if (ch == 27)
        {                    // 按下 ESC 键退出
            running = false; // 停止所有线程
            break;
        }
        else if (ch == 8) // 按下 Backspace 键删除字符
        {
            if (!editor.deleteChar()) // 判断能否继续删除
            {
                editor.showErrorWithDelay("无法删除，文本为空！", 1000); // 显示错误信息并延迟
            }
            system("cls"); // 清屏
        }
        else if (ch == 26) // 按下 Ctrl+Z 撤销操作
        {
            editor.undo(); // 调用撤销函数
            system("cls"); // 清屏
        }
        else if (ch == 25) // 按下 Ctrl+Y 重做操作
        {
            editor.redo(); // 调用重做函数
            system("cls"); // 清屏
        }
        else if (ch == 3) // 按下 Ctrl+C 复制操作
        {
            editor.copy(); // 调用复制函数
            system("cls"); // 清屏
        }
        else if (ch == 22) // 按下 Ctrl+V 粘贴操作
        {
            editor.paste(); // 调用粘贴函数
            system("cls");  // 清屏
        }

        else if (isprint(ch)) // 如果是可打印字符
        {
            editor.insertChar(ch); // 调用插入字符函数
        }

        editor.displayText(); // 显示当前的文本内容
    }
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
// #This program is coded by Chen Sixiang#//

