//#ifndef DO_NOT_TRY_TO_CLOSE_H
//#define DO_NOT_TRY_TO_CLOSE_H
#include<iostream>
#include<cstdlib>
#include<windows.h>
#include<Tlhelp32.h>
 
using namespace std;
  
void OPEN() 
{ 
	ShellExecuteA(NULL,"open","返校倒计时.exe",0,0,0);
}

DWORD GetProcessidFromName(LPCTSTR name)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe)) return 0;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp(pe.szExeFile, name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return id;
}
int main()
{ 
    HWND hWnd= GetForegroundWindow(); 
    ShowWindow(hWnd, SW_HIDE);
    while(1)
    {
	    if(GetProcessidFromName("返校倒计时.exe")==0) OPEN();
        Sleep(8000); 
    }
	return 0;
}
//void DO_NOT_TRY_TO_CLOSE()
//{ 
//    while(1)
//	{
// 	    if(GetProcessidFromName("返校倒计时.exe")==0) OPEN();
//		Sleep(5000);
//	} 
//	return;
//}
//#endif
