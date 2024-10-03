#ifndef KEY_H
#define KEY_H

#include <windows.h> 
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

#define A VK_A
#define a VK_A
#define B VK_B
#define b VK_B
#define C VK_C
#define c VK_C
#define D VK_D
#define d VK_D
#define E 69
#define e 69
#define F VK_F
#define f VK_F
#define G VK_G
#define g VK_G
#define H 72
#define h 72
#define I VK_I
#define i VK_I
#define J VK_J
#define j VK_J
#define K VK_K
#define k VK_K
#define L VK_L
#define l VK_L
#define M VK_M
#define m VK_M
#define N VK_N
#define n VK_N
#define O VK_O
#define o VK_O
#define P VK_P
#define p VK_P
#define Q VK_Q
#define q VK_Q
#define R VK_R
#define r VK_R
#define S VK_S
#define s VK_S
#define T VK_T
#define t VK_T
#define U VK_U
#define u VK_U
#define V VK_V
#define v VK_V
#define W VK_W
#define w VK_W
#define X VK_X
#define x VK_X
#define Y VK_Y
#define y VK_Y
#define Z VK_Z
#define z VK_Z
#define _0 VK_NUMPAD0
#define _1 VK_NUMPAD1
#define _2 VK_NUMPAD2
#define _3 VK_NUMPAD3
#define _4 VK_NUMPAD4
#define _5 VK_NUMPAD5
#define _6 VK_NUMPAD6
#define _7 VK_NUMPAD7
#define _8 VK_NUMPAD8
#define _9 VK_NUMPAD9
#define _ VK_SPACE //¿Õ¸ñ 


//#define ; 186
//#define Caps_Lock 20
class Key
{
	public:
		Key();
		~Key();
		Key& down(int vk_code);
		Key& up(int vk_code);
		Key& press(int vk_code);
		Key& combination(int vk_code);
		Key& combination(int vk_code_1,int vk_code_2);
		Key& combination(int vk_code_1,int vk_code_2,int vk_code_3);
		Key& combination(int vk_code_1,int vk_code_2,int vk_code_3,int vk_code_4);
		Key& sleep(int _time);
		Key& caps(); 
		Key& Caps();
		Key& setSleepTime(int _time);
		Key& bearStr(string str);
		Key& period();
		Key& comma();
		int  getTime();
		 
	private:
		
		int time;
		
};

#endif
