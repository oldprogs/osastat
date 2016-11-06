#ifndef _TIMEF_H_
#define _TIMEF_H_

int tm_mday;
int tm_mon;

unsigned long int day_sec=24*60*60;

unsigned long int time_start;
unsigned long int time_stop;

int set_time(int days_back);
void print_time();

#endif
