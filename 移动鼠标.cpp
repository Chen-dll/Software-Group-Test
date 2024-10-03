#include <iostream>
#include <windows.h>
#include <cstdio>
 
using namespace std;
 
int main(){
	POINT p; 
	while(1){
		GetCursorPos(&p);//获取鼠标坐标 
		printf("%d %d", p.x,p.y);
        system("pause");
        // SetCursorPos(p.x+3,p.y);//更改鼠标坐标 
		// Sleep(10);//控制移动时间间隔 
	}
	mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
	/*鼠标的当前位置单击，mouse_event(MOUSEEVENTF_XXXXXXX,0,0,0,0)，前两个是0是x，y坐标
    ，如果没有使用MOUSEEVENTF_ABSOLUTE，函数默认的是相对于鼠标当前位置的点，如果x，和y，用
    0，0表示，这函数认为是当前鼠标所在的点*/
    mouse_event (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, 500, 500, 0, 0 )
    //绝对坐标，直接设定绝对坐标并单击
    // mouse_event(MOUSEEVENTF_LEFTDOWN, X * 65536 / 1024, Y * 65536 / 768, 0, 0);
    // mouse_event(MOUSEEVENTF_LEFTUP, X * 65536 / 1024, Y * 65536 / 768, 0, 0);
    //其中X，Y分别是你要点击的点的横坐标和纵坐标
    return 0;
}