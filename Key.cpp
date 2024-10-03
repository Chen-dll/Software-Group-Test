#include "Key.h"
#include <string>
Key::Key()
{
	this->time=0; //默认没有延迟 
	cout<<this->time;
}

Key::~Key()
{
	
}
void state()
{
	/*
	// 判断键盘CapsLock键是否开启状态,开启状态则为大写,否则为小写
     if (GetKeyState(VK_CAPITAL))
     {
         // 如果当前键盘状态为大写,要求改小写,则模拟按键CapsLock切换状态
         if (!big)
         {
             keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
             keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
        }
     }
     else
     {
         // 如果当前键盘状态为小写,要求改大写,则模拟按键CapsLock切换状态He 
         if (big)
         {
             keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
             keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
         }
     }*/
     //if(!GetKeyStaye(VK_CAPITAL))
    // {
     	//press(VK_CAPITAL); 
     	
	// }
	 //如果是小写改成大写 
}

Key& Key::down(int vk_code)
{
	keybd_event(vk_code,0,0,0);
	return *this;

}
Key& Key::up(int vk_code)
{
	keybd_event(vk_code,0,KEYEVENTF_KEYUP,0);
	return *this;

}
Key& Key::press(int vk_code)
{
	if(this->time)
	{
		this->sleep(this->time);
	}
	
	if(islower(vk_code))//小写字母 
	{
		vk_code-=32; 
	}
	down(vk_code);
	up(vk_code);
	return *this; 
}
Key& Key::combination(int vk_code)
{
	press(vk_code);
	return *this;
}
Key& Key::combination(int vk_code_1,int vk_code_2)
{
	down(vk_code_1);
	press(vk_code_2);
	up(vk_code_1);
	return *this;
}
Key& Key::combination(int vk_code_1,int vk_code_2,int vk_code_3)
{
	down(vk_code_1);
	down(vk_code_2);
	press(vk_code_3);
	up(vk_code_2);
	up(vk_code_1);
	return *this;
}
Key& Key::combination(int vk_code_1,int vk_code_2,int vk_code_3,int vk_code_4)
{
	down(vk_code_1);
	down(vk_code_2);
	down(vk_code_3);
	press(vk_code_4);
	up(vk_code_3);
	up(vk_code_2);
	up(vk_code_1);
	return *this;
}
Key& Key::sleep(int _time)
{
	Sleep(_time);
	return *this;
}
Key& Key::caps()
{
	if (GetKeyState(VK_CAPITAL))
    {
         // 如果当前键盘状态为大写,要求改小写,则模拟按键CapsLock切换状态

             keybd_event(VK_CAPITAL, 0, 0, 0);
             keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
    return *this;
}
Key& Key::Caps()
{
	if (!GetKeyState(VK_CAPITAL))
    {
         // 如果当前键盘状态为小写,要求改大写,则模拟按键CapsLock切换状态

             keybd_event(VK_CAPITAL, 0, 0, 0);
             keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
    return *this;
}
Key& Key::setSleepTime(int _time)
{
	this->time=_time;
	return *this;
}
Key& Key::period()
{
	keybd_event(VK_DECIMAL, 0, 0, 0);
    keybd_event(VK_DECIMAL, 0, KEYEVENTF_KEYUP, 0);
	return *this;
}
Key& Key::comma()
{
	keybd_event(188, 0, 0, 0);
    keybd_event(188, 0, KEYEVENTF_KEYUP, 0);
	return *this;
}
int Key::getTime()
{
	return this->time;
}
Key& Key::bearStr(string str)
{
    for(int cout=0;cout<str.length();cout++)
    {
    	if(isupper(str[cout]))//如果是大写字母
		{
			this->Caps();
			press(str[cout]);
			this->caps(); 
		}
		else if(str[cout]=='.')
		{
			period();
		}
		else if(str[cout]==',')
		{
			comma();
		}
		else
		{
			press(str[cout]);
		}
	}
	return *this;
}

