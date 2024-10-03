#ifndef PATH_H
#define PATH_H
#include <windows.h>  
#include <iostream>   
#include <string> 
using namespace std;    
     
string GetProgramDir()  //full
{   
    char exeFullPath[MAX_PATH]; // Full path
    string strPath = "";
 
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
    strPath=(string)exeFullPath;    // Get full path of the file
    //int pos = strPath.find_last_of('\\', strPath.length());
    return strPath;  // Return the directory with the full file name
    //return strPath.substr(0, pos);  // Return the directory without the file name 
}   

string GetProgramName()  //program name
{
	char exename[MAX_PATH];
	string strname = "";
    GetModuleFileName(NULL, exename, MAX_PATH);
    strname += (string)exename; 
    int pos = strname.find_last_of('\\', strname.length());
    string REAL;
	REAL += "\"";
	REAL += strname.substr(pos+1,strname.length());
	REAL += "\""; 
    return REAL;	
}

string GetProgramFuck()  //part
{   
    char exeFullPath[MAX_PATH];
    string strPath = "";
 
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
    strPath=(string)exeFullPath; 
    int pos = strPath.find_last_of('\\', strPath.length());
    return strPath.substr(0, pos);  // Return the directory without the file name 
}   

string path()
{
    string str;
    str += "\"instsrv GameManagerServer ";
    str += GetProgramDir();
    str += "\""; 
    return str;
}
#endif
