#include <windows.h>
#include <bits/stdc++.h>
void __fastcall TForm1::Button1Click(TObject *Sender)
{
　　HANDLE DestHandle;
　　DestHandle = FindWindow(NULL,"test.txt - 芝並云");
　　if (DestHandle != 0) PostMessage(DestHandle, WM_CLOSE, 0, 0);
}
int main()
{
	return 0;
}
 
