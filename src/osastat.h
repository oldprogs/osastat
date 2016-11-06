#ifndef _OSASTAT_H_
#define _OSASTAT_H_

#define NUM_Z	1000
#define NUM_Y	100

char log_types[5][255]={"bforce","binary","qico","ifcico","qicoX"};

extern struct configuration_struct{
    char conf_name[255];
    char conf_type[255];
    char conf_file[1024];
}configuration_key[100];

extern struct stat_screen_base{
    unsigned short int	zone, net, node, point;
    unsigned long int	bytesrcv, bytessnt;
    unsigned int	sess;
    unsigned char	field[97];
}*stat_screen;

extern struct line_load_struct{
    unsigned char fullname[255], desc[255], name_line[255], field[97];
    unsigned long int sess_in, sess_out;
    unsigned long int bytes_recv, bytes_sent;
    unsigned long int time_in, time_out;
}*line_load;

extern int configure_num;

extern unsigned int stat_screen_acc;
//extern unsigned int line_load_acc;
extern unsigned long int sess_in, sess_out;//, sess_all;
extern unsigned long int bytes_recv, bytes_sent;//, bytes_all;
extern unsigned long int time_in, time_out;//, time_all;

#endif

