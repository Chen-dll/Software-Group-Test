#ifndef SET_REG_H
#define SET_REG_H
#include <windows.h>  
#include <iostream>   
#include <string> 
#include <bits/stdc++.h>

void SET_REG()
{
    // TODO: 在此添加控件通知处理程序代码    
    HKEY hKey;  //可以理解为操作注册表的句柄 
    //找到系统的启动项  ，长指针类型是const char * 类型
    LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";  
    //打开启动项Key  
    long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);  
    if(lRet == ERROR_SUCCESS)  //打开成功
  	{  
       char pFileName[MAX_PATH] = {0};  //定义数组存放路径
       //得到程序自身的全路径  
       DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);  
       //添加一个子Key,并设置值 // 第二项是应用程序名字（不加后缀.exe） 
       lRet = RegSetValueEx(hKey, "返校倒计时", 0, REG_SZ, (BYTE *)pFileName, dwRet);  
       //关闭注册表  
       RegCloseKey(hKey);  
  }  
}
//int main()
//{
//	SET_REG();
//	return 0;
//}
#endif
