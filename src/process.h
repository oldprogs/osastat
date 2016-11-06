#ifndef _PROCESS_H_
#define _PROCESS_H_

extern unsigned int time_start;
extern unsigned int time_stop;
extern unsigned int day_sec;
extern int tm_mday;
extern int tm_mon;

extern struct configuration_lines_aliases{
    char alias[255];
    char lines[1024];
}conf_aliases[100];

extern int aliases_num;
extern int group_aliases;

unsigned char sort_order[10000];
unsigned char sort_order_sep[100][32];
unsigned char sort_order_num=0;

unsigned int stat_screen_acc=0;

unsigned int sess_in=0, sess_out=0;//, sess_all=0;
unsigned int bytes_recv=0, bytes_sent=0;//, bytes_all=0;
unsigned int time_in=0, time_out=0;//, time_all=0;

struct stat_screen_base{
    unsigned short int	zone, net, node, point;
    unsigned int	bytesrcv, bytessnt;
    unsigned int	sess;
    unsigned char	field[97];
}*stat_screen;

unsigned int line_load_acc=0;

struct line_load_struct{
    unsigned char fullname[255], desc[255], name_line[255], field[97];
    unsigned int sess_in, sess_out;//, sess_all=0;
    unsigned int bytes_recv, bytes_sent;//, bytes_all=0;
    unsigned int time_in, time_out;//, time_all=0;
}*line_load;

unsigned char	month[12][16];
unsigned char	date_get[32];
unsigned char	date_get_len;
unsigned char	*tpl_start="†€‰€Š€‰€Š€‰€Š€‰€Š€‰€Š€‰€Š€‰€Š€‰€Š€‰€Š€‰€Š€‰€Š€‰€‡";
unsigned char	*tpl_draw ="                                    ";
unsigned char	*tpl_split="†€€€Š€€€Š€€€Š€€€Š€€€Š€€€Š€€€Š€€€Š€€€Š€€€Š€€€Š€€€‡";
unsigned char	*tpl_end  ="†€ˆ€Š€ˆ€Š€ˆ€Š€ˆ€Š€ˆ€Š€ˆ€Š€ˆ€Š€ˆ€Š€ˆ€Š€ˆ€Š€ˆ€Š€ˆ€‡";
unsigned char	*tpl_numb ="0   2   4   6   8  10  12  14  16  18  20  22  24";
unsigned char	otstup_num=15;

int process_bforce(char *desc, char *file);
int process_qico(char *desc, char *file, char type);
int process_ifcico(char *desc, char *file);
int process_binary(char *desc, char *file);
int statistics_filter(unsigned char *desc, unsigned char *name_line, unsigned short int zone, unsigned short int net, unsigned short int node, unsigned short int point, unsigned char typ, unsigned int timebeg, unsigned int timelen, unsigned int bytesrcv, unsigned int bytessnt);
int statistics_find_position(unsigned short int zone, unsigned short int net, unsigned short int node, unsigned short int point);
int statistics_insert(unsigned char *desc, unsigned char *name_line, unsigned short int zone, unsigned short int net, unsigned short int node, unsigned short int point, unsigned char typ, unsigned int timebeg, unsigned int timelen, unsigned int bytesrcv, unsigned int bytessnt);
int split_ftn_addr(unsigned char *addr, unsigned short int *zone, unsigned short int *net, unsigned short int *node, unsigned short int *point);
int sort_order_prepare();
int sort_aka();
int exchange_stat_struct(struct stat_screen_base *s1, struct stat_screen_base *s2);
int up_struct_block(int first, int num);
int statistics_draw();
int line_load_draw();
void otstup();
void to_long(unsigned char *x, int len);
void perevod_num(char *adp, unsigned int bytes_i);
void perevod_time(char *adp, unsigned int second);
void find_name(unsigned char *fullname, unsigned char *desc, unsigned char *name_line);
void line_load_group();
unsigned int get_sess_text_num(char *buff);
unsigned int get_sess_text_time(char *buff);

#endif
