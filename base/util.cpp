/*************************************************************************
  > File Name: util.cpp
  > Created Time: Mon 11 Apr 2016 06:18:03 PM CST
  > Author: 
  > description: 
 ************************************************************************/

#include "util.h"

//转化15:30:00到当天的秒数
uint32_t t2seconds(string t, string format)                 
{                                                                      
    struct tm tmp = {0};                                               
    strptime(t.c_str() , format.c_str(), &tmp);                        
    return tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec;          
}                                                                      

//转化时间戳到当天的秒数
uint32_t st2seconds(time_t  t)              
{                                                      
    struct tm tmp = {0};                               
    localtime_r((time_t*)&t, &tmp);                    
	//cout << "time=" << t << " tm_hour=" << tmp.tm_hour << " tm_min=" << tmp.tm_min << " tm_sec="<<tmp.tm_sec << endl;
    return tmp.tm_hour * 3600 + tmp.tm_min * 60 + tmp.tm_sec;
}

string second2ts(uint32_t second)                                
{                                                                           
    int h = second / 3600;                                                  
    int m = (second % 3600) / 60;                                           
    int s = (second % 3600) % 60;                                           
    char tmp[32] = {0};                                                     
    snprintf(tmp, 32, "%02d%02d%02d", h, m, s);                             
    return tmp;                                                             
}                                                                           

//字符串格式转化时间戳
time_t strtostamp(const char*  date, const char* format)
{
    struct tm tmp = {0};
    strptime(date , format, &tmp);
    return mktime(&tmp); 
}

string stamptostr(time_t stamp, const char* format)
{
    char strtime[32] = {0};
    struct tm tmp = {0};
    localtime_r(&stamp, &tmp);
    strftime(strtime, sizeof(strtime), format, &tmp);  
    return strtime;  
}

//日期转化tm格式
struct tm strtotm(const char*  date, const char* format)
{
    struct tm tmp = {0};
    strptime(date , format, &tmp); 
    return tmp; 
}

uint32_t get_hour(time_t stamp)
{
    struct tm tmp = {0};      
    localtime_r(&stamp, &tmp);
    return  tmp.tm_hour;
}

uint32_t get_day_of_week(time_t stamp)
{
    struct tm tmp = {0};
    localtime_r(&stamp, &tmp);
    return tmp.tm_wday;
}

uint32_t get_ymd(time_t stamp)
{
    struct tm tmp = {0};
    localtime_r(&stamp, &tmp);
    tmp.tm_hour = 0;
    tmp.tm_min  = 0;
    tmp.tm_sec  = 0;
    return mktime(&tmp);
}

//返回以微秒记
uint64_t get_usec_timestamp()
{
    struct  timeval    tv;
    gettimeofday(&tv, NULL);
    return ((uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec);
}

string ctos(char c)
{
    if((uint8_t)c == 0 || c == '\0') return "";
    else return string(1, c);
}

int set_limit_core(uint32_t size)
{
    rlimit rlim, rlim_new;
    if(getrlimit(RLIMIT_CORE, &rlim) == 0)
    {
        rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
        if(setrlimit(RLIMIT_CORE, &rlim_new) == 0) 
            return 0;

        if(size > rlim.rlim_max)
            rlim_new.rlim_cur = rlim_new.rlim_max = size;
        else
            rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;

        if(setrlimit(RLIMIT_CORE, &rlim_new) != 0)
            return errno;
    }
    return errno;
}


int set_daemon()
{
    if(fork() != 0)
    {
        printf("pid:%d ppid:%d(exit)\n", getpid(), getppid());
        exit(-1);
    }

    printf("pid:%d ppid:%d\n", getpid(), getppid());
    if(setsid() <0)
    {
        printf("set group leader fail. errno:%d\n", errno);
        exit(-1);
    }

    if(fork() != 0)
    {
        printf("pid:%d ppid:%d(exit)\n", getpid(), getppid());
        exit(-1);
    }

    printf("pid:%d ppid:%d\n", getpid(), getppid());

    umask(0);  
    //chdir("/");  环境变量找不到会

    //close(STDIN_FILENO);  
    //close(STDOUT_FILENO);  
    //close(STDERR_FILENO);   

    signal(SIGCHLD,SIG_IGN);

    return 0;
}


uint64_t stoint64(const char* s)
{
    uint64_t v = 0;
    memset(&v, 0, sizeof(v));
    char* p = (char*)s;
    int32_t size = sizeof(uint64_t)/sizeof(char);
    int32_t i = size-1;
    while(*p != 0 && i >= 0)
    {
        *(((char*)&v)+i) = *p;
        //printf("%c---%d ", *(((char*)&v)+i), *(((char*)&v)+i));
        i--;
        p++;
    }
    //printf("\n %lu \n", v);

    return v; 
}

string int64tos(uint64_t v)
{
    string s;
    int32_t size = sizeof(uint64_t)/sizeof(char);
    for(int32_t i=size-1; i>=0; i--)
    {
        char* c = ((char*)&v)+i;
        s.append (c, 1); 
    }
    return s;
}








