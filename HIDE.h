#ifndef HIDE_H
#define HIDE_H
#include <iostream>
#include <windows.h>          //左上 0 0 右上 300 0 左下 0 145 右下 300 145 
#include <cstdio>
 
using namespace std;
 
void HIDE(HWND hwnd)
{
	POINT p; 
	while(1)
	{
		GetCursorPos(&p);//获取鼠标坐标 
		if((p.x >= 0 && p.x <= 300) && (p.y >= 0 && p.y <= 145)) ShowWindow(hwnd, SW_HIDE);//判断鼠标是否在窗口上 
		else ShowWindow(hwnd, SW_SHOW);//显示窗口
	}
	return;
}
#endif 
