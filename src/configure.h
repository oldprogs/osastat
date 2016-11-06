#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

struct configuration_struct{
    char conf_name[255];
    char conf_type[255];
    char conf_file[1024];
}configuration_key[100];

struct configuration_lines_aliases{
    char alias[255];
    char lines[1024];
}conf_aliases[100];

char *conf_string_pattern="^conf_.*=";
char config_key[7][255]={"conf_name","conf_type","conf_file","conf_sort","conf_lines","conf_alias","conf_group"};

extern unsigned char sort_order[10000];
extern unsigned short int lines_num;

int configure_num=0;
int aliases_num=0;
int group_aliases=0;

int configure(char *file_config_name);

#endif
