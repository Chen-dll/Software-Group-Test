#ifndef HOOK_H
#define HOOK_H
#include <assert.h>
#include <windows.h> 
// 全局Hook ID
HHOOK g_HookHwnd = NULL ;

// WH_KEYBOARD_LL好像需要将实例句柄映射出去
// 具体自己看MSDN吧
HINSTANCE hInst;

LRESULT CALLBACK Wndproc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK MyHookFun(int nCode, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
  WNDCLASSEX ws;  // 定义结构 注意:这里用的是WNDCLASSEX

  ws.cbSize = sizeof(WNDCLASSEX);  // 返回结构大小 这个选项是Ex结构中新增的
  ws.hbrBackground = (HBRUSH)COLOR_BACKGROUND;  // 背景颜色
  ws.hCursor = LoadCursor(NULL,IDC_ARROW);  // 鼠标指针样式
  ws.hIcon = LoadIcon(NULL,IDI_APPLICATION);  // 图标样式
  ws.hIconSm = LoadIcon(NULL,IDI_APPLICATION);  // 小图标  同样是Ex新增的
  ws.hInstance = hInstance;  // 当前实例句柄
  ws.lpfnWndProc = Wndproc;  // 回调函数指针
  ws.lpszClassName = "MyWindows";  // 窗口类名
  ws.lpszMenuName = NULL;   // 菜单名
  ws.style = CS_DBLCLKS;  // 接受双击窗体消息  //CS_HREDRAW | CS_VREDRAW;
  ws.cbClsExtra = 0;  // 无额外类存储
  ws.cbWndExtra = 0;  // 无额外窗体存储

  if (!RegisterClassEx(&ws))  // Ex的结构必须要Ex的注册函数
  {
    return 0;  // 注册失败直接返回0
  } 

  hInst = hInstance;

  HWND hWnd;

  hWnd = CreateWindowEx(
    0,  // 扩展样式
    "MyWindows",  // 类名,就是注册时的类名
    "For Example",  // 标题栏文字
    WS_OVERLAPPEDWINDOW,  // 样式 各属性均用默认值
    CW_USEDEFAULT,  // 窗体X值
    CW_USEDEFAULT,  // 窗体Y值
    CW_USEDEFAULT,  // 窗体宽度
    CW_USEDEFAULT,  // 窗体高度
    NULL,  // 父窗口句柄
    NULL,  // 功能菜单句柄
    hInstance,  // 应用程序实例句柄
    NULL  // 建立参数
    );

  if (!hWnd)
  {
    return 0;  // 失败返回
  }
  
  ShowWindow(hWnd, SW_SHOWNORMAL);  // 显示窗体
  UpdateWindow(hWnd);  //更新窗体

  MSG msg;

  // 消息主循环
  while(GetMessage(&msg,NULL,0,0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int)msg.wParam;
}

// 主回调处理函数
LRESULT CALLBACK Wndproc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  // 以下两个仅用于测试Hook安装和卸载的返回值
  BOOL bL = NULL;
  BOOL bLresult = NULL;

  switch(uMsg)
  {
    case WM_DESTROY:
      // 可以卸载钩子了
      // 参数就是SetWindowsHookEx返回的ID
      bLresult = UnhookWindowsHookEx(g_HookHwnd);
      // ASSERT测试
      // 当exp为False,弹出错误框
      // 需要afx.h支持
      assert(bLresult);
      PostQuitMessage(0);
      break;

    case WM_CREATE:
      // 安装钩子
      g_HookHwnd = SetWindowsHookEx(WH_KEYBOARD_LL, MyHookFun, hInst, 0);
      // 测试.同上
      assert(g_HookHwnd);
      break;

    case WM_LBUTTONDBLCLK:
      MessageBox(hwnd,"Um...KingsamChen is a Fucker","Fuker",MB_OK);
      break;

    default:
      return DefWindowProc(hwnd,uMsg,wParam,lParam);
      break;
  }
  
  return 0;
}

// 钩子子程
LRESULT CALLBACK MyHookFun(int nCode, WPARAM wParam, LPARAM lParam)
{
  // 这个Structure包含了键盘的信息
  /*typedef struct {
      DWORD vkCode;
      DWORD scanCode;
      DWORD flags;
      DWORD time;
      ULONG_PTR dwExtraInfo;
  } KBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;*/
  // 我们只需要那个vkCode
  PKBDLLHOOKSTRUCT pVirKey = (PKBDLLHOOKSTRUCT)lParam;
  
  // MSDN说了,nCode < 0的时候别处理
  if (nCode >= 0)
  {
    // 按键消息
    switch(wParam) 
    {
      case WM_KEYDOWN:
      case WM_SYSKEYDOWN: 
      case WM_KEYUP:
      case WM_SYSKEYUP:
        switch(pVirKey->vkCode) 
        {
          case VK_LWIN:
          case VK_RWIN:
            return 1;  // 吃掉消息
            break;
        }
        break;
    }    
  }

  return CallNextHookEx(g_HookHwnd, nCode, wParam, lParam);
}
#endif
