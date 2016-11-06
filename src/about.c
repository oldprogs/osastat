#include "about.h"

void about(){
    printf(" €€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€\n");
    printf("  osastat 1.0beta-4 (c) Oleg Smirnov, 2:5022/128\n");
    printf("  (skin borrowed from T-Hist)\n");
    printf("\n");
}

void intro(){
    printf("\n");
    printf("     ‚€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€ƒ\n");
    printf("      ‘‘‘’’’  Summary sessions history for %2d lines  ’’’‘‘‘ \n", lines_num);
    printf("     „€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€…\n");
    printf("\n");
}

void epilog(){
    unsigned char x[16], y[16];
    printf("\n");
    printf (" ‚€€€€€€€€€€€€€€€€€€€€€€€€ˆ€€€€€€€€€€€€€€€€€€€€€€ˆ€€€€€€€€€€€€€€€€€€€€€€€€€€ƒ\n");
    perevod_num(x, bytes_recv);
    perevod_time(y, time_in);
    printf ("  Total Received : %5s  Total Ses In  : %4d  Time In      :  %8s \n", x, sess_in, y);
    perevod_num(x, bytes_sent);
    perevod_time(y, time_out);
    printf ("  Total Sent     : %5s  Total Ses Out : %4d  Time Out     :  %8s \n", x, sess_out, y);
    perevod_num(x, bytes_recv + bytes_sent);
    perevod_time(y, time_in + time_out);
    printf ("  Total Traffic  : %5s  Total Sessions: %4d  Time Online  :  %8s \n", x, sess_in+sess_out, y);
    printf (" „€€€€€€€€€€€€€€€€€€€€€€€€‰€€€€€€€€€€€€€€€€€€€€€€‰€€€€€€€€€€€€€€€€€€€€€€€€€€…\n");
}
