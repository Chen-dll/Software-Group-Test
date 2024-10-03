#ifndef FUCKING_CRAZY_H
#define FUCKING_CRAZY_H
#include <windows.h>

COLORREF col;
UINT r=GetRValue(col), g=GetGValue(col),b=GetBValue(col);//RGB() 

void FUCKING_CRAZY(HWND hwnd)
{ 
    HMENU hmenu = GetSystemMenu(hwnd, false);
    RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~(WS_MINIMIZEBOX);//屏蔽关闭和最小化 
    DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);  
	dwExStyle |=  WS_EX_TOOLWINDOW;//ALT+TAB不显示 
	LONG styleValue = ::GetWindowLong(hwnd, GWL_EXSTYLE); //设置扩展窗口主题 
    styleValue |= WS_EX_LAYERED; 
    dwExStyle |= WS_EX_TRANSPARENT;//让鼠标穿透窗口点击 
    SetWindowLong(hwnd,GWL_EXSTYLE,styleValue);
    SetLayeredWindowAttributes(hwnd,RGB(r,g,b), 180, LWA_ALPHA);//设置窗口半透明									   
	SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle & ~WS_EX_APPWINDOW);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetWindowLong(hwnd, GWL_STYLE, style);//设置窗口主题 
    ShowWindow(hwnd, SW_SHOWNORMAL);//正常显示 
    DestroyMenu(hmenu);
    ReleaseDC(hwnd, NULL);
}
#endif
