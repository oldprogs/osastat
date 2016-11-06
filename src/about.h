#ifndef _ABOUT_H_
#define _ABOUT_H_

extern unsigned long int sess_in, sess_out;//, sess_all;
extern unsigned long int bytes_recv, bytes_sent;//, bytes_all;
extern unsigned long int time_in, time_out;//, time_all;

unsigned short int lines_num;

void about();
void intro();
void epilog();

#endif