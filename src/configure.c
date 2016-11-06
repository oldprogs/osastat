#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <regex.h>

#include "configure.h"

int configure(char *file_config_name){

    FILE *file_config;
    char buff[1024], param[255], *tmp_char;

    regmatch_t result[1024];
    regex_t f_conf_string_pattern;
    if (regcomp(&f_conf_string_pattern, conf_string_pattern, 0)){
	printf("regexp error!\n");
	return 2;
    }
    if (!(file_config=fopen(file_config_name, "r"))){
	printf("error open configuration file %s\n", file_config_name);
	return 1;
    }
    do{
	fgets(buff, sizeof(buff), file_config);
	if(regexec(&f_conf_string_pattern, buff, 20, result, 0)) continue;
	strcpy(param, strstr(buff,"\"")+1);
	*strstr(param, "\"")=0;
	if (strstr(buff, config_key[0])){ strcpy(configuration_key[configure_num].conf_name, param);}
	if (strstr(buff, config_key[1])){ strcpy(configuration_key[configure_num].conf_type, param);}
	if (strstr(buff, config_key[2])){
	    strcpy(configuration_key[configure_num].conf_file, param);
	    configure_num++;
	}
	if (strstr(buff, config_key[3])){strcpy(sort_order, param);}
	if (strstr(buff, config_key[4])){lines_num=atoi(param);}
	if (strstr(buff, config_key[5])){
	    tmp_char=strstr(param, "==");
	    *tmp_char=0;
	    strcpy(conf_aliases[aliases_num].alias, param);
	    strcpy(conf_aliases[aliases_num].lines, tmp_char+2);
//	    printf("%s %s\n", conf_aliases[aliases_num].alias, conf_aliases[aliases_num].lines);
	    aliases_num++;
	}
	if (strstr(buff, config_key[6])){group_aliases=atoi(param);}
	if (strstr(buff, config_key[7])){month_get(param);}
	if (strstr(buff, config_key[8])){date_get_conf(param);}
	bzero(buff, sizeof(buff));
    }while(!feof(file_config));
    fclose(file_config);
    return 0;
}

void month_get(char *param){
    char ckl, len=strlen(param), num=12;
//    printf("%s\n", param);
    for (ckl=len; ckl>0; ckl--){
	if (num<1){
	    printf("Неверно заданы месяцы в 'conf_text_month'...\n");
	    exit();
	}
	if (param[ckl]==*","){
	    num--;
	    param[ckl]=0;
	    strcpy(month[num], &param[ckl+1]);
	}
    }
    strcpy(month[0], param);
//    for (ckl=0; ckl<12; ckl++){
//	printf("-%s(%d)-", month[ckl], ckl);
//    }
}

void date_get_conf(char *param){
    char ckl, len=strlen(param);
    for (ckl=0; ckl<len; ckl++){
	if (param[ckl]==*"d"){
	    sprintf(date_get, "%s%d", date_get, tm_mday);
	    continue;
	}
	if (param[ckl]==*"m"){
	    sprintf(date_get, "%s%s", date_get, month[tm_mon]);
	    continue;
	}
	sprintf(date_get, "%s%c", date_get, param[ckl]);
    }
//    printf("%s\n", date_get);
}
