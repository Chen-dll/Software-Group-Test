#include <iostream>
#include <windows.h>
using namespace std;
int main()
{
	POINT p; 
	while(1)
    {
		GetCursorPos(&p);//获取鼠标坐标 
		SetCursorPos(p.x + 5, p.y);//更改鼠标坐标 
		Sleep(10); 
		//控制移动时间间隔 
        cout << p.x << " " << p.y << endl;
        // 输出当前鼠标(x,y)坐标
	}
	return 0;
} 
