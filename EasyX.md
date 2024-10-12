
# EasyX 图形库学习笔记

## 一、简介
EasyX 是一个专为 C++ 设计的简单的图形库。

### 环境搭建
- 使用 Visual Studio 或者 Dev-C++
- 在 EasyX 图形库官网下载
- 添加 EasyX 图形库路径到 include 和 lib 目录中
- VS 项目配置属性字符集要使用多字节字符集

---

## 二、常用的函数

### 1. 初始化与关闭窗口

#### `initgraph`
```cpp
void initgraph(int width, int height);
```
- **功能**：创建一个宽为 `width`，高为 `height` 的图形窗口，单位为像素。
- **例子**：
```cpp
initgraph(640, 480);
```

#### `closegraph`
```cpp
void closegraph();
```
- **功能**：关闭图形窗口，释放图形资源。

---

### 2. 绘图函数

#### `setfillcolor()`
```cpp
void setfillcolor(COLORREF color);
```
- **功能**：设置填充颜色，用于之后的填充图形操作。
- **参数**：
  - `color`: 填充颜色，使用 RGB 颜色值。
- **例子**：
```cpp
setfillcolor(RED);
```

#### `solidrectangle()`
```cpp
void solidrectangle(int left, int top, int right, int bottom);
```
- **功能**：绘制一个实心矩形，矩形的左上角坐标为 `(left, top)`，右下角坐标为 `(right, bottom)`，使用当前的填充颜色进行填充。
- **例子**：
```cpp
solidrectangle(100, 100, 200, 200);
```

#### `setbkmode()`
```cpp
void setbkmode(int mode);
```
- **功能**：设置背景模式，可以选择将背景设置为透明或使用当前的背景色填充背景。
- **参数**：
  - `mode`: 取值 `TRANSPARENT`（透明背景）或 `OPAQUE`（不透明背景）。
- **例子**：
```cpp
setbkmode(TRANSPARENT);
```

#### `settextcolor()`
```cpp
void settextcolor(COLORREF color);
```
- **功能**：设置文本颜色。
- **参数**：
  - `color`: 文本颜色，使用 RGB 颜色值。
- **例子**：
```cpp
settextcolor(BLUE);
```

#### `settextstyle()`
```cpp
void settextstyle(int height, int width, const TCHAR* face);
```
- **功能**：设置字体样式，包括字体高度、宽度和字体名称。
- **参数**：
  - `height`: 字体的高度。
  - `width`: 字体的宽度（如果宽度设置为 0，系统将根据高度自动计算宽度）。
  - `face`: 字体名称。
- **例子**：
```cpp
settextstyle(20, 0, _T("CSX"));
```
#### `outtextxy()`
```cpp
void outtextxy(int x, int y, const TCHAR* str);
```
- **功能**：在坐标 `(x, y)` 处输出字符串 `str`，使用当前的文本样式和颜色。
- **参数**：
  - `x`: 文本输出的起始横坐标。
  - `y`: 文本输出的起始纵坐标。
  - `str`: 要输出的文本字符串。
- **例子**：
```cpp
outtextxy(100, 100, _T("Hello World"));
```
#### `cleardevice()`
```cpp
void cleardevice();
```
- **功能**：清除当前窗口的所有图像内容，使用背景色进行填充, 用于刷新屏幕。
- **例子**：
```cpp
cleardevice();
```

---

### 3. 输入与交互

#### `getch`
```cpp
char _getch();
```
- **功能**：等待并获取键盘输入字符。
- **例子**：
```cpp
char key = _getch();
```

#### `mousemsg`
```cpp
MOUSEMSG getmouse();
```
- **功能**：获取鼠标消息，如鼠标点击、移动等。
- **例子**：
```cpp
MOUSEMSG msg = getmouse();
```

---

### 3. 示例代码

```cpp
#include <graphics.h>
#include <conio.h>

int main() {
    initgraph(640, 480);   // 初始化图形窗口

    setfillcolor(RED);     // 设置填充颜色
    solidrectangle(100, 100, 300, 200); // 绘制一个实心矩形

    settextcolor(BLUE);    // 设置文本颜色
    settextstyle(30, 0, _T("Arial")); // 设置字体样式
    outtextxy(150, 150, _T("Hello EasyX")); // 输出文本

    _getch();               // 等待用户输入
    closegraph();          // 关闭图形窗口
    return 0;
}
```

---
