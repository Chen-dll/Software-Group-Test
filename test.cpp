#include <iostream>
#include <conio.h>  // 用于 _getch() 函数

int main() {
    std::cout << "Press arrow keys (left or right) or ESC to exit:\n";

    while (true) {
        int ch = _getch();  // 捕获第一个字符
        if (ch == 224) {  // 特殊键（方向键）的第一个值是 224
            ch = _getch();  // 捕获实际的方向键值
            switch (ch) {
                case 75:  // 左箭头
                    std::cout << "Left arrow key pressed.\n";
                    break;
                case 77:  // 右箭头
                    std::cout << "Right arrow key pressed.\n";
                    break;
                default:
                    std::cout << "Another special key pressed.\n";
                    break;
            }
        } else if (ch == 27) {  // ESC 键退出
            break;
        }
    }

    return 0;
}

