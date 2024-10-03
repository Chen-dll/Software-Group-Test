#include <bits/stdc++.h>
using namespace std;
time_t convert_string_to_time_t(const std::string & time_string)  
{  
         struct tm tm1;  
         time_t time1;  
         sscanf(time_string.c_str(), "%d-%d-%d %d:%d:%d" ,&(tm1.tm_year), &(tm1.tm_mon),&(tm1.tm_mday),&(tm1.tm_hour), &(tm1.tm_min),&(tm1.tm_sec));
                  
         tm1.tm_year -= 1900;  
        tm1.tm_mon --;  
        tm1.tm_isdst=-1;  
         time1 = mktime(&tm1);  
       
         return time1;  
     }  
     
 int main()  
 {  
    string date_string("2010-11-20 18:08:01");  
     time_t tmp_time;  
     tmp_time = convert_string_to_time_t(date_string);  
     cout<<tmp_time<<endl;
    
