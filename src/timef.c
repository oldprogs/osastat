#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <regex.h>
#include <time.h>

#include "timef.h"

int set_time(int days_back){
    time_t date_now;
    struct tm date_now_s;
    long offset_utc;
    
    time(&date_now);
    date_now_s=*localtime(&date_now);
    offset_utc=date_now_s.tm_gmtoff;
//    tm_mday=date_now_s.tm_mday;
//    tm_mon=date_now_s.tm_mon+1;
    date_now_s.tm_sec=0;
    date_now_s.tm_min=0;
    date_now_s.tm_hour=0;
//    date_now=timegm(&date_now_s)-offset_utc;
    date_now=timegm(&date_now_s);
    
    time_start=date_now-days_back*day_sec;
    time_stop=date_now-days_back*day_sec+day_sec-1;

    date_now_s=*localtime(&time_start);
    tm_mday=date_now_s.tm_mday;
    tm_mon=date_now_s.tm_mon;

//    printf("%d: %s", time_start, ctime(&time_start));
//    printf("%d: %s", time_stop, ctime(&time_stop));
    
}

void print_time(){
    time_t date_now;
    struct tm date_now_s;
    char x[16], ln, ckl;
    date_now=time_stop;
    date_now_s=*localtime(&date_now);
    sprintf(x, "%2d.%2d.%4d", date_now_s.tm_mday, date_now_s.tm_mon+1, date_now_s.tm_year+1900);
    ln=strlen(x);
    for (ckl=0; ckl<ln; ckl++){
	if (x[ckl]==*" "){
	    x[ckl]=*"0";
	}
    }
    printf("                     From 00:00:00 to 23:59:59, %s\n", x);
    printf("\n");
}
