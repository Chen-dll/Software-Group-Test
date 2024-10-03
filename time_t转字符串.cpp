#include <bits/stdc++.h>
using namespace std;
int main()
{
	time_t timer, ZK, GK;//声明变量 
	time(&timer); 
	timer = time(NULL);		
//	time_t time (time_t* timer);//获取当前时间,然后赋值给指针变量 timer
//							    //注意这里的时间是现在时间减去1900年1月1日0时0分0秒的时间
	char* ctime (const time_t * timer);//将time_t转换成字符串
	cout << ctime(&timer);// 字符串的格式为: Www Mmm dd hh:mm:ss yyyy
    //星期      月     日  时   分  秒  年
	return 0; 
 } 
