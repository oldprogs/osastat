#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <regex.h>

#include "process.h"

int process_bforce(char *desc, char *file){
    FILE *file_log;
    char buff[1024], ckl;
    unsigned short int      zone, net, node, point, len;
    unsigned int       timebeg, timelen, bytesrcv, bytessnt;
    unsigned char typ, pn=0, pole[12][80], *pnt, force[4], name_line[80];
					    
    if (!(file_log=fopen(file, "r"))){
	printf("error open log file: %s\n", file);
	return 1;
    }
    while(!feof(file_log)){
	bzero(buff, sizeof(buff));
	fgets(buff, sizeof(buff), file_log);
//	printf("%s", buff);
	pn=0;
	len=strlen(buff);
	for (ckl=len; ckl>=0; ckl--){
	    if (buff[ckl]==*","){
		strcpy(pole[pn], &buff[ckl+1]);
		buff[ckl]=0;
		pn++;
	    }
	}
	strcpy(pole[pn], buff);
	pn++;
//	printf("", );
//	for (ckl=12; ckl>0; ckl--){
//	    printf("(%d)%s,", ckl-1, pole[ckl-1]);
//	}
	if (pn<12){
	    pn++;
	    strcpy(pole[12], pole[11]);
	    strcpy(pole[11], pole[10]);
	    strcpy(pole[10], pole[9]);
	    strcpy(pole[9], pole[8]);
	    strcpy(pole[8], pole[7]);
	    strcpy(pole[7], pole[6]);
	    strcpy(pole[6], "0");
	}
//	for (ckl=12; ckl>0; ckl--){
//	    printf("(%d)%s,", ckl-1, pole[ckl-1]);
//	}

	if (split_ftn_addr(pole[10], &zone, &net, &node, &point)==-1) continue;

	bytesrcv=atoi(pole[0])+atoi(pole[1])+atoi(pole[2]);
	bytessnt=atoi(pole[3])+atoi(pole[4])+atoi(pole[5]);
	timelen=atoi(pole[8]);
	timebeg=atoi(pole[9]);
	strcpy(name_line, pole[11]);
	if (strstr(pole[7],"O")){typ=*"O";}else{typ=*"I";}
	if (strstr(pole[7],"I")){typ=*"I";}else{typ=*"O";}

	

	statistics_filter(desc, name_line, zone, net, node, point, typ, timebeg, timelen, bytesrcv, bytessnt);
    }
    fclose(file_log);
}

int process_qico(char *desc, char *file, char type){
    FILE *file_log;
    char buff[1024], ckl;
    unsigned short int      zone, net, node, point, len;
    unsigned int       timebeg, timelen, bytesrcv, bytessnt;
    unsigned char typ, pn=0, pole[7][80], *pnt, force[4], name_line[80];
					    
    if (!(file_log=fopen(file, "r"))){
	printf("error open log file: %s\n", file);
	return 1;
    }
    while(!feof(file_log)){
	bzero(buff, sizeof(buff));
	fgets(buff, sizeof(buff), file_log);
	pn=0;
	len=strlen(buff);
	for (ckl=len; ckl>=0; ckl--){
	    if (buff[ckl]==*","){
		strcpy(pole[pn], &buff[ckl+1]);
		buff[ckl]=0;
		pn++;
	    }
	}
	strcpy(pole[pn], buff);
	pn++;
	if (type==0){
	    if (split_ftn_addr(pole[3], &zone, &net, &node, &point)==-1) continue;
	    if (strstr(pole[2],"O")){typ=*"O";}else{typ=*"I";}
	    if (strstr(pole[2],"I")){typ=*"I";}else{typ=*"O";}
	}else{
	    if (split_ftn_addr(pole[2], &zone, &net, &node, &point)==-1) continue;
	    if (strstr(pole[3],"O")){typ=*"O";}else{typ=*"I";}
	    if (strstr(pole[3],"I")){typ=*"I";}else{typ=*"O";}
	}

	bytesrcv=atoi(pole[0]);
	bytessnt=atoi(pole[1]);
	timelen=atoi(pole[4]);
	timebeg=atoi(pole[5]);
	strcpy(name_line, pole[6]);

	statistics_filter(desc, name_line, zone, net, node, point, typ, timebeg, timelen, bytesrcv, bytessnt);
    }
    fclose(file_log);
}

int process_ifcico(char *desc, char *file){
    FILE *file_log;
    char buff[1024], mnth[255], ckl, tmp_buff[1024], tmp_len;
    char *string_pat[6]={	".*:.*:.*if.*:.*start.*bound.*session",
				".*:.*:.*if.*:.*remote  address:",
				".*:.*:.*if.*:.*sent.*bytes in.*seconds",
				".*:.*:.*if.*:.*received.*bytes in.*seconds",
				".*:.*:.*if.*:.* of .* calls, ",
				".*:.*:.*if.*:.*call to.*successful" };
    regex_t f_conf_string_pattern[5];
    regmatch_t result[1024];
    unsigned short int	zone, net, node, point, len;
    unsigned int	timebeg, timelen, bytesrcv=0, bytessnt=0;
    unsigned long	curr_pos, curr_sess;
    unsigned char typ;//, pn=0, pole[7][80], *pnt, force[4], name_line[80];

    for (ckl=0; ckl<6; ckl++){
	if (regcomp(&f_conf_string_pattern[ckl], string_pat[ckl], 0)){
	    printf("regexp error!\n");
	    return 2;
	}
    }
					    
    if (!(file_log=fopen(file, "r"))){
	printf("error open log file: %s\n", file);
	return 1;
    }
    while(!feof(file_log)){
	bzero(buff, sizeof(buff));
	fgets(buff, sizeof(buff), file_log);
//	buff[strlen(date_get)]=0;
//	printf("-(%s)-%s-%d\n", date_get, buff, memcmp(buff, date_get, date_get_len));
	if(!memcmp(date_get, buff, strlen(date_get))){
//	    printf("%s", buff);
	    if(!regexec(&f_conf_string_pattern[0], buff, 20, result, 0)){
//		printf("%s", buff);
    	        if (strstr(buff," outbound ")){typ=*"O";}else{typ=*"I";}
		if (strstr(buff," inbound ")) {typ=*"I";}else{typ=*"O";}
		curr_sess=get_sess_text_num(buff);
		timebeg=get_sess_text_time(buff);
	    	curr_pos=ftell(file_log);
		net=0;
		bytessnt=0;
		bytesrcv=0;
		while(!feof(file_log)){
		    bzero(buff, sizeof(buff));
		    fgets(buff, sizeof(buff), file_log);
		    if((!regexec(&f_conf_string_pattern[1], buff, 20, result, 0))&&(curr_sess==get_sess_text_num(buff))&&(net==0)){
			*(strstr(buff, "@")+1)=0;
			*strstr(buff, "@")=*"\n";
			split_ftn_addr(strstr(buff, "remote  address:")+17, &zone, &net, &node, &point);
//		 	printf("(%d:%d/%d.%d)" , zone, net, node, point);
			continue;
		    }
		    if((!regexec(&f_conf_string_pattern[2], buff, 20, result, 0))&&(curr_sess==get_sess_text_num(buff))){
			strcpy(tmp_buff, buff);
			*strstr(tmp_buff, " bytes in ")=0;
			tmp_len=strlen(tmp_buff)-1;
			for (ckl=tmp_len; ckl>0; ckl--){
			    if (tmp_buff[ckl]==*" "){
				bytessnt+=atoi(&tmp_buff[ckl+1]);
				break;
			    }
			}
//		    	printf("(%d)%s", bytessnt, buff);
			continue;
		    }
		    if((!regexec(&f_conf_string_pattern[3], buff, 20, result, 0))&&(curr_sess==get_sess_text_num(buff))){
			strcpy(tmp_buff, buff);
			*strstr(tmp_buff, " bytes in ")=0;
			tmp_len=strlen(tmp_buff)-1;
			for (ckl=tmp_len; ckl>0; ckl--){
			    if (tmp_buff[ckl]==*" "){
				bytesrcv+=atoi(&tmp_buff[ckl+1]);
				break;
			    }
			}
//		    	printf("(%d)%s", bytesrcv, buff);
			continue;
		    }
		    if(((!regexec(&f_conf_string_pattern[4], buff, 20, result, 0))||(!regexec(&f_conf_string_pattern[5], buff, 20, result, 0)))&&(curr_sess==get_sess_text_num(buff))){
			timelen=get_sess_text_time(buff)-timebeg;
//		 	printf("%s/ifcico|%d:%d/%d.%d - %c - (beg:%d/len:%d) - rx:%d/tx:%d\n" , desc, zone, net, node, point, typ ,timebeg ,timelen , bytesrcv, bytessnt);
			if (net!=0){
			    statistics_filter(desc, "ifcico", zone, net, node, point, typ, timebeg, timelen, bytesrcv, bytessnt);
			}
			break;
		    }
/*		    if((!regexec(&f_conf_string_pattern[3], buff, 20, result, 0))&&(curr_sess==get_sess_text_num(buff))){
			timelen=get_sess_text_time(buff)-timebeg;
//		 	printf("%s/ifcico|%d:%d/%d.%d - %c - (beg:%d/len:%d) - rx:%d/tx:%d\n" , desc, zone, net, node, point, typ ,timebeg ,timelen , bytesrcv, bytessnt);
			if (net!=0){
			    statistics_filter(desc, "ifcico", zone, net, node, point, typ, timebeg, timelen, bytesrcv, bytessnt);
			}
			bytessnt=0;bytesrcv=0;
			break;
		    }*/
		}
		fseek(file_log, curr_pos, SEEK_SET);
	    }
	}
	
/*	if(!regexec(&f_conf_string_pattern[1], buff, 20, result, 0)){
	    printf("%s", buff);
	}
	if(!regexec(&f_conf_string_pattern[2], buff, 20, result, 0)){
	    printf("%s", buff);
	}
	if(!regexec(&f_conf_string_pattern[3], buff, 20, result, 0)){
	    printf("%s", buff);
	}*/
/*	pn=0;
	len=strlen(buff);
	for (ckl=len; ckl>=0; ckl--){
	    if (buff[ckl]==*","){
		strcpy(pole[pn], &buff[ckl+1]);
		buff[ckl]=0;
		pn++;
	    }
	}
	strcpy(pole[pn], buff);
	pn++;
	if (split_ftn_addr(pole[2], &zone, &net, &node, &point)==-1) continue;

	bytesrcv=atoi(pole[0]);
	bytessnt=atoi(pole[1]);
	timelen=atoi(pole[4]);
	timebeg=atoi(pole[5]);
	strcpy(name_line, pole[6]);
	if (strstr(pole[3],"O")){typ=*"O";}else{typ=*"I";}
	if (strstr(pole[3],"I")){typ=*"I";}else{typ=*"O";}

	statistics_filter(desc, name_line, zone, net, node, point, typ, timebeg, timelen, bytesrcv, bytessnt);
*/
    }
    fclose(file_log);
}

int process_binary(char *desc, char *file){
    FILE *file_log;
    struct{
	unsigned short int      zone, net, node, point;
	unsigned int       timebeg, timelen, bytesrcv, bytessnt;
	unsigned char           filesrcv, filessnt;
	unsigned short int      typ;
    }thist;
    unsigned char typ;

    if (!(file_log=fopen(file, "r"))){
	printf("error open log file: %s\n", file);
	return 1;
    }
    while(!feof(file_log)){
	bzero(&thist, sizeof(thist));
	fread(&thist, 1, sizeof(thist), file_log);
	if ((thist.zone==0) && (thist.net==0) && (thist.node==0) && (thist.point==0) && (thist.timebeg==0) && (thist.timelen==0)){
	    continue;
	}
	if (thist.typ & 2){typ=*"I";}else{typ=*"O";}
	statistics_filter(desc, "binary", thist.zone, thist.net, thist.node, thist.point, typ, thist.timebeg, thist.timelen, thist.bytesrcv, thist.bytessnt);
    }
    fclose(file_log);
}

int statistics_filter(unsigned char *desc, unsigned char *name_line, unsigned short int zone, unsigned short int net, unsigned short int node, unsigned short int point, unsigned char typ, unsigned int timebeg, unsigned int timelen, unsigned int bytesrcv, unsigned int bytessnt){
    if ((timebeg>=time_start)&&(timebeg<=time_stop)){
//	printf("%s/%s = %d:%d/%d.%d - %c - %d(%d/%d), %d: rx:%d/tx:%d\n", desc, name_line, zone, net, node, point, typ, timebeg, timebeg-time_start, timebeg-time_stop, timelen, bytesrcv, bytessnt);
	statistics_insert(desc, name_line, zone, net, node, point, typ, timebeg, timelen, bytesrcv, bytessnt);
//    }else{
//	printf("%d<%d<%d\n", time_start, timebeg, time_stop);
//	printf("%s/%s = %d:%d/%d.%d - %c - (%d)%d(%d/%d), %d: rx:%d/tx:%d\n", desc, name_line, zone, net, node, point, typ, ctime(&timebeg), timebeg, timebeg-time_start, timebeg-time_stop, timelen, bytesrcv, bytessnt);
    }
}

int statistics_find_position(unsigned short int zone, unsigned short int net, unsigned short int node, unsigned short int point){
    unsigned int ckl;
    for (ckl=0; ckl<stat_screen_acc; ckl++){
	if ((stat_screen[ckl].zone==zone)&&(stat_screen[ckl].net==net)&&(stat_screen[ckl].node==node)&&(stat_screen[ckl].point==point)){
//	    printf("-%d=%d-\n", stat_screen[ckl].zone, zone);
//	    printf("-%d=%d-\n", stat_screen[ckl].net, net);
//	    printf("-%d=%d-\n", stat_screen[ckl].node, node);
//	    printf("-%d=%d-\n", stat_screen[ckl].point, point);
//	    printf("%d Ok\n", ckl);
	    return ckl;
//	}else{
//	    printf("-%d=%d-\n", stat_screen[ckl].zone, zone);
//	    printf("-%d=%d-\n", stat_screen[ckl].net, net);
//	    printf("-%d=%d-\n", stat_screen[ckl].node, node);
//	    printf("-%d=%d-\n", stat_screen[ckl].point, point);
	}
    }
    return -1;
}

int load_find_position(unsigned char *desc, unsigned char *name_line){
    unsigned int ckl;
    for (ckl=0; ckl<line_load_acc; ckl++){
//	printf("\n%s/%s-%s/%s\n", line_load[ckl].desc, desc, line_load[ckl].name_line, name_line);
	if ((!strcmp(line_load[ckl].desc, desc))&&(!strcmp(line_load[ckl].name_line, name_line))){
//	    printf("\nOk\n");
//	    printf("-%d=%d-\n", stat_screen[ckl].zone, zone);
//	    printf("-%d=%d-\n", stat_screen[ckl].net, net);
//	    printf("-%d=%d-\n", stat_screen[ckl].node, node);
//	    printf("-%d=%d-\n", stat_screen[ckl].point, point);
//	    printf("%d Ok\n", ckl);
	    return ckl;
	}//else{
//	    printf("\nNo\n");
//	    printf("-%d=%d-\n", stat_screen[ckl].zone, zone);
//	    printf("-%d=%d-\n", stat_screen[ckl].net, net);
//	    printf("-%d=%d-\n", stat_screen[ckl].node, node);
//	    printf("-%d=%d-\n", stat_screen[ckl].point, point);
//	}
    }
    return -1;
}

int statistics_insert(unsigned char *desc, unsigned char *name_line, unsigned short int zone, unsigned short int net, unsigned short int node, unsigned short int point, unsigned char typ, unsigned int timebeg, unsigned int timelen, unsigned int bytesrcv, unsigned int bytessnt){
    int position, ll_position, t_start, t_stop, ckl;
    unsigned char buff[1024];
    position=statistics_find_position(zone, net, node, point);
    ll_position=load_find_position(desc, name_line);
//    printf("pos: %d --", ll_position);
    if (position==-1){
	position=stat_screen_acc;
	stat_screen_acc++;
    }
    if (ll_position==-1){
	ll_position=line_load_acc;
	line_load_acc++;
	bzero(&line_load[ll_position], sizeof(struct line_load_struct));
    }
    stat_screen[position].zone=zone;
    stat_screen[position].net=net;
    stat_screen[position].node=node;
    stat_screen[position].point=point;
    stat_screen[position].bytesrcv+=bytesrcv;
    stat_screen[position].bytessnt+=bytessnt;
    stat_screen[position].sess++;
    bytes_recv+=bytesrcv;
    bytes_sent+=bytessnt;
    line_load[ll_position].bytes_recv+=bytesrcv;
    line_load[ll_position].bytes_sent+=bytessnt;
//    bytes_all+=bytesrcv;
//    bytes_all+=bytessnt;
    if (typ==*"I"){
	sess_in++;
	time_in+=timelen;
	line_load[ll_position].sess_in++;
	line_load[ll_position].time_in+=timelen;
    }else{
	sess_out++;
	time_out+=timelen;
	line_load[ll_position].sess_out++;
	line_load[ll_position].time_out+=timelen;
    }
//    printf("<(%d/%d/%d)>\n", ll_position, line_load[ll_position].sess_in, line_load[ll_position].sess_out);

    t_start=(timebeg-time_start)/900;
    t_stop=timelen/900;

    for (ckl=0; ckl<=t_stop; ckl++){
	if (t_start+ckl<97){
	    stat_screen[position].field[t_start+ckl]=1;
	}
//	printf("X");
    }
    strcpy(line_load[ll_position].desc, desc);
    strcpy(line_load[ll_position].name_line, name_line);
    find_name(line_load[ll_position].fullname, line_load[ll_position].desc ,line_load[ll_position].name_line);
//    printf("<(%d/%d/%d)>\n", ll_position, line_load[ll_position].sess_in, line_load[ll_position].sess_out);
    for (ckl=0; ckl<=t_stop; ckl++){
	if (t_start+ckl<97){
	    line_load[ll_position].field[t_start+ckl]++;
	}
//	printf("X");
    }
//    printf("\n%d| %d:%d/%d.%d sess:%d/rx:%d/tx:%d\n", position, stat_screen[position].zone, stat_screen[position].net, stat_screen[position].node, stat_screen[position].point, stat_screen[position].sess, stat_screen[position].bytesrcv, stat_screen[position].bytessnt);
//    printf("sess in:%d out:%d all:%d\n", sess_in, sess_out, sess_all);
//    printf("traff in:%d out:%d all:%d\n", bytes_recv, bytes_sent, bytes_all);
//    for (ckl=0; ckl<96; ckl++){
//	printf("%d", line_load[ll_position].field[ckl]);
//    }
//    printf("\n");
//    printf("<(%d/%d/%d)>\n", ll_position, line_load[ll_position].sess_in, line_load[ll_position].sess_out);
}

int split_ftn_addr(unsigned char *addr, unsigned short int *zone, unsigned short int *net, unsigned short int *node, unsigned short int *point){
    char	force[4]=":/.", *pnt, ckl;
    for (ckl=2; ckl>=0; ckl--){
	force[ckl+1]=0;
	pnt=strstr(addr, &force[ckl]);
	if (pnt==NULL){
	    switch (ckl){
        	case 2: *point=0;
            		break;
                case 1: *node=0;
            		break;
                case 0: *net=0;
            		break;
            }
        }else{
    	    switch (ckl){
        	case 2: *point=atoi(pnt+1);
            		break;
                case 1: *node=atoi(pnt+1);
            		break;
                case 0: *net=atoi(pnt+1);
                        break;
            }
	    *pnt=0;
        }
    }
    *zone=atoi(addr);
    if (*net==0) return -1;
    return 0;
}

int sort_order_prepare(){
    unsigned short int	zone, net, node, point, ckl, pr_pos=0, len;
    len=strlen(sort_order);
    for (ckl=0; ckl<len; ckl++){
	if (sort_order[ckl]==*","){
	    sort_order[ckl]=0;
	    strcpy(sort_order_sep[sort_order_num], &sort_order[pr_pos]);
//	    split_ftn_addr(sort_order_sep[sort_order_num], &zone, &net, &node, &point);
	    sort_order_num++;
	    pr_pos=ckl+1;
//	    printf("%d:%d/%d.%d\n", zone, net, node, point);
	}
    }
    strcpy(sort_order_sep[sort_order_num], &sort_order[pr_pos]);
//    split_ftn_addr(sort_order_sep[sort_order_num], &zone, &net, &node, &point);
    sort_order_num++;
//    printf("%d:%d/%d.%d\n", zone, net, node, point);
}

int sort_aka(){
    unsigned short int	tempbuffer[100], ckl, ckl1, ckl2, num;
    int	pos;
    unsigned char buff[32];
//    printf("----------\n");
//    for(ckl=0; ckl<stat_screen_acc; ckl++){
//	printf("%d| %d:%d/%d.%d sess:%d/rx:%d/tx:%d\n", ckl, stat_screen[ckl].zone, stat_screen[ckl].net, stat_screen[ckl].node, stat_screen[ckl].point, stat_screen[ckl].sess, stat_screen[ckl].bytesrcv, stat_screen[ckl].bytessnt);
//	for (ckl1=0; ckl1<96; ckl1++){
//	    printf("%d", stat_screen[ckl].field[ckl1]);
//	}
//    }
//    printf("----------\n");
    for(ckl1=0; ckl1<stat_screen_acc-1; ckl1++){
	for(ckl=ckl1+1; ckl<stat_screen_acc; ckl++){
	    if (stat_screen[ckl].zone<stat_screen[ckl1].zone){
//		printf("zone: cmp %d < %d\n", stat_screen[ckl].zone, stat_screen[ckl1].zone);
		exchange_stat_struct(&stat_screen[ckl], &stat_screen[ckl1]);
	    }else{
		if ((stat_screen[ckl].zone==stat_screen[ckl1].zone)&&(stat_screen[ckl].net<stat_screen[ckl1].net)){
//		    printf("net: cmp %d < %d\n", stat_screen[ckl].net, stat_screen[ckl1].net);
		    exchange_stat_struct(&stat_screen[ckl], &stat_screen[ckl1]);
		}else{
		    if ((stat_screen[ckl].zone==stat_screen[ckl1].zone)&&(stat_screen[ckl].net==stat_screen[ckl1].net)&&(stat_screen[ckl].node<stat_screen[ckl1].node)){
//			printf("ndl: cmp %d < %d\n", stat_screen[ckl].node, stat_screen[ckl1].node);
			exchange_stat_struct(&stat_screen[ckl], &stat_screen[ckl1]);
		    }else{
			if ((stat_screen[ckl].zone==stat_screen[ckl1].zone)&&(stat_screen[ckl].net==stat_screen[ckl1].net)&&(stat_screen[ckl].node==stat_screen[ckl1].node)&&(stat_screen[ckl].point<stat_screen[ckl1].point)){
//			    printf("pnt: cmp %d < %d\n", stat_screen[ckl].point, stat_screen[ckl1].point);
			    exchange_stat_struct(&stat_screen[ckl], &stat_screen[ckl1]);
			}
		    }
		}
	    }
	}
//    for(ckl=0; ckl<stat_screen_acc; ckl++){
//	printf("%d| %d:%d/%d.%d sess:%d/rx:%d/tx:%d\n", ckl, stat_screen[ckl].zone, stat_screen[ckl].net, stat_screen[ckl].node, stat_screen[ckl].point, stat_screen[ckl].sess, stat_screen[ckl].bytesrcv, stat_screen[ckl].bytessnt);
//	for (ckl1=0; ckl1<96; ckl1++){
//	    printf("%d", stat_screen[ckl].field[ckl1]);
//	}
//    }
    }
//    printf("\n----------\n");
//    for(ckl=0; ckl<stat_screen_acc; ckl++){
//	printf("%d| %d:%d/%d.%d sess:%d/rx:%d/tx:%d\n", ckl, stat_screen[ckl].zone, stat_screen[ckl].net, stat_screen[ckl].node, stat_screen[ckl].point, stat_screen[ckl].sess, stat_screen[ckl].bytesrcv, stat_screen[ckl].bytessnt);
//	for (ckl1=0; ckl1<96; ckl1++){
//	    printf("%d", stat_screen[ckl].field[ckl1]);
//	}
//    }
//    printf("\n----------\n");
    for(ckl=sort_order_num; ckl>0; ckl--){
//	printf("ckl:%d\n", ckl-1);
	pos=-1;num=0;
	for(ckl1=0; ckl1<stat_screen_acc; ckl1++){
//	    printf("%d test...\n", ckl1);
	    sprintf(buff,"%d:%d/%d.%d", stat_screen[ckl1].zone, stat_screen[ckl1].net, stat_screen[ckl1].node, stat_screen[ckl1].point );
//	    printf("%s->%s\n", buff, sort_order_sep[ckl-1]);
	    if (strstr(buff, sort_order_sep[ckl-1])){
//		printf("%d Ok\n", ckl1);
		if (pos==-1){
		    pos=ckl1;
		    num++;
		}else{
		    num++;
		}
	    }
	}
	if (pos!=-1){
//	    printf("%d->%d\n", pos, num);
	    up_struct_block(pos, num);
	}
//    printf("----------\n");
//    for(ckl2=0; ckl2<stat_screen_acc; ckl2++){
//	printf("%d| %d:%d/%d.%d sess:%d/rx:%d/tx:%d\n", ckl2, stat_screen[ckl2].zone, stat_screen[ckl2].net, stat_screen[ckl2].node, stat_screen[ckl2].point, stat_screen[ckl2].sess, stat_screen[ckl2].bytesrcv, stat_screen[ckl2].bytessnt);
//	for (ckl1=0; ckl1<96; ckl1++){
//	    printf("%d", stat_screen[ckl].field[ckl1]);
//	}
//    }
//    printf("----------\n");
    }
//    printf("----------\n");
//    for(ckl2=0; ckl2<stat_screen_acc; ckl2++){
//	printf("%2d| %d:%d/%d.%d sess:%d/rx:%d/tx:%d\n", ckl2, stat_screen[ckl2].zone, stat_screen[ckl2].net, stat_screen[ckl2].node, stat_screen[ckl2].point, stat_screen[ckl2].sess, stat_screen[ckl2].bytesrcv, stat_screen[ckl2].bytessnt);
//	for (ckl1=0; ckl1<96; ckl1++){
//	    printf("%d", stat_screen[ckl].field[ckl1]);
//	}
//    }
//    printf("\n----------\n");
}

int exchange_stat_struct(struct stat_screen_base *s1, struct stat_screen_base *s2){
    struct stat_screen_base *tmp_s;
    int b_size=sizeof(struct stat_screen_base);
    tmp_s=(struct stat_screen_base *)malloc(sizeof(struct stat_screen_base));
//    bzero(tmp_s, b_size);
    memcpy(tmp_s, s1, b_size);
//    bzero(s1, b_size);
    memcpy(s1, s2, b_size);
//    bzero(s2, b_size);
    memcpy(s2, tmp_s, b_size);
    free(tmp_s);
}

int up_struct_block(int first, int num){
    struct stat_screen_base *tmp_s;
    int b_size=first*sizeof(struct stat_screen_base), ckl;
//    for (ckl=first; ckl<=first+num-1; ckl++){
//	printf("(1|%d:%d/%d.%d{%d})", stat_screen[ckl].zone, stat_screen[ckl].net, stat_screen[ckl].node, stat_screen[ckl].point, ckl);
//    }
    tmp_s=(struct stat_screen_base *)malloc(b_size);
//    bzero(tmp_s, b_size);
    memcpy(tmp_s, stat_screen, b_size);
//    bzero(stat_screen, num*sizeof(struct stat_screen_base));
    memcpy(stat_screen, &stat_screen[first], num*sizeof(struct stat_screen_base));
//    printf("\n");
//    bzero(&stat_screen[num], b_size);
    memcpy(&stat_screen[num], tmp_s, b_size);
    free(tmp_s);
//    for (ckl=first; ckl<=first+num-1; ckl++){
//	printf("(2|%d:%d/%d.%d{%d})", stat_screen[ckl].zone, stat_screen[ckl].net, stat_screen[ckl].node, stat_screen[ckl].point, ckl);
//    }
//    printf("\n");
}

int statistics_draw(){
    int ckl, ckl1;
    unsigned char tpl_tmp[49], p1=*"", p2=*"", p3=*"", buff_addr[20], bytr[16], byts[16];
    otstup();printf("%s\n", tpl_numb);
    otstup();printf("%sses  recv  sent\n", tpl_start);
    for (ckl=0; ckl<stat_screen_acc; ckl++){
	strcpy(tpl_tmp, tpl_draw);
	if (stat_screen[ckl].field[0]==1){
	    tpl_tmp[0]=p2;
	}
	for (ckl1=1; ckl1<49; ckl1++){
	    if ((stat_screen[ckl].field[ckl1*2-1]==1)&&(stat_screen[ckl].field[ckl1*2]==0)){
		tpl_tmp[ckl1]=p1;
	    }
	    if ((stat_screen[ckl].field[ckl1*2-1]==0)&&(stat_screen[ckl].field[ckl1*2]==1)){
		tpl_tmp[ckl1]=p2;
	    }
	    if ((stat_screen[ckl].field[ckl1*2-1]==1)&&(stat_screen[ckl].field[ckl1*2]==1)){
		tpl_tmp[ckl1]=p3;
	    }
//	    printf("%d/%d", ckl1*2-1, ckl1*2);
	}
	if (stat_screen[ckl].field[96]==1){
	    tpl_tmp[48]=p1;
	}else{
	    tpl_tmp[48]=*"";
	}
	tpl_tmp[49]=0;
	bzero(buff_addr, sizeof(buff_addr));
	if (stat_screen[ckl].point!=0){
	    sprintf(buff_addr, "%d:%d/%d.%d", stat_screen[ckl].zone, stat_screen[ckl].net, stat_screen[ckl].node, stat_screen[ckl].point);
	}else{
	    sprintf(buff_addr, "%d:%d/%d", stat_screen[ckl].zone, stat_screen[ckl].net, stat_screen[ckl].node);
	}
	if (strlen(buff_addr)<otstup_num){
	    to_long(buff_addr, otstup_num+1);
	}
	if ((ckl>0)&&((stat_screen[ckl].zone!=stat_screen[ckl-1].zone)||(stat_screen[ckl].net!=stat_screen[ckl-1].net))){
	    otstup();printf("%s\n", tpl_split);
	}
	perevod_num(byts, stat_screen[ckl].bytessnt);
	perevod_num(bytr, stat_screen[ckl].bytesrcv);
	printf("%s%s%3d %5s %5s\n", buff_addr, tpl_tmp, stat_screen[ckl].sess, bytr, byts);
    }
    otstup();printf("%s\n", tpl_end);
    otstup();printf("%s\n", tpl_numb);
}

int line_load_draw(){
    int ckl, ckl1, ckl2, tmp_calc;
    unsigned char tpl_tmp[11][49], p1=*"", p2=*"", p3=*"", buff[1024];
    double perc, perc1;
    bzero(tpl_tmp, sizeof(tpl_tmp));
    for (ckl=0; ckl<line_load_acc; ckl++){
//	for (cx=0; cx<96; cx++){
//	    printf("%d", line_load[ckl].field[cx]);
//	}
//	printf("<(%d/%d/%d)>\n", ckl, line_load[ckl].sess_in, line_load[ckl].sess_out);
	printf("\n");
	for (ckl1=0; ckl1<10; ckl1++){
	    tpl_tmp[ckl1][0]=*"";
	    if ((line_load[ckl].field[0]>0)||(line_load[ckl].field[1]>0)){
		    tpl_tmp[ckl1][0]=p2;
		    if (line_load[ckl].field[0]>0) line_load[ckl].field[0]--;
		    if (line_load[ckl].field[1]>0) line_load[ckl].field[1]--;
	    }
	    for (ckl2=1; ckl2<48; ckl2++){
		tpl_tmp[ckl1][ckl2]=*" ";
//		printf("%d%d", line_load[ckl].field[ckl2*2], line_load[ckl].field[ckl2*2+1]);
		if ((line_load[ckl].field[ckl2*2]>0)&&(line_load[ckl].field[ckl2*2+1]>0)){
		    tpl_tmp[ckl1][ckl2]=p3;
		    line_load[ckl].field[ckl2*2]--;
		    line_load[ckl].field[ckl2*2+1]--;
		}else{
		    if ((line_load[ckl].field[ckl2*2]==0)&&(line_load[ckl].field[ckl2*2+1]>0)){
			tpl_tmp[ckl1][ckl2]=p2;
			line_load[ckl].field[ckl2*2+1]--;
		    }else{
			if ((line_load[ckl].field[ckl2*2]>0)&&(line_load[ckl].field[ckl2*2+1]==0)){
			    tpl_tmp[ckl1][ckl2]=p1;
			    line_load[ckl].field[ckl2*2]--;
			    
			}
		    }
		}
//		printf("%c", tpl_tmp[ckl1][ckl2]);
	    }
	    tpl_tmp[ckl1][48]=0;
	}
	if ((line_load[ckl].sess_in+line_load[ckl].sess_out)>0){
	    if (line_load[ckl].fullname[0]){
		ckl2=(80-strlen(line_load[ckl].fullname))/2;
		for(ckl1=0; ckl1<ckl2; ckl1++){
		    printf(" ");
		}
		printf("%s\n", line_load[ckl].fullname);
	    }
	    if (!group_aliases){
		sprintf(buff, "(%s/%s)\0", line_load[ckl].desc ,line_load[ckl].name_line);
		ckl2=(80-strlen(buff))/2;
		for(ckl1=0; ckl1<ckl2; ckl1++){
		    printf(" ");
		}
		printf("%s\n", buff);
	    }
	    printf("\n");
	    perevod_num(buff, line_load[ckl].bytes_recv);
	    printf("           100%%%s  Rcvd:    %5s\n", tpl_tmp[9], buff);
	    perevod_num(buff, line_load[ckl].bytes_sent);
	    printf("               %s  Sent:    %5s\n", tpl_tmp[8], buff);
	    printf("               %s  €€€€€€€€€€€€€€\n", tpl_tmp[7]);
	    if ((line_load[ckl].time_in+line_load[ckl].time_out)>0){
		tmp_calc=(line_load[ckl].bytes_recv+line_load[ckl].bytes_sent)/(line_load[ckl].time_in+line_load[ckl].time_out);
	    }else{
		tmp_calc=0;
	    }
	    printf("               %s  CPS: %9d\n", tpl_tmp[6], tmp_calc);
	    printf("               %s\n", tpl_tmp[5]);
	    printf("   Sessions    %s   Time Online\n", tpl_tmp[4]);
	    printf(" €€€€€€€€€€€€  %s  €€€€€€€€€€€€€€\n", tpl_tmp[3]);
	    perevod_time(buff, line_load[ckl].time_in);
	    printf(" In:    %4d   %s  In:   %8s\n", line_load[ckl].sess_in, tpl_tmp[2], buff);
	    perevod_time(buff, line_load[ckl].time_out);
	    printf(" Out:   %4d   %s  Out:  %8s\n", line_load[ckl].sess_out, tpl_tmp[1], buff);
	    perevod_time(buff, line_load[ckl].time_in+line_load[ckl].time_out);
	    printf(" Total: %4d   %s  Total:%8s\n", line_load[ckl].sess_in+line_load[ckl].sess_out, tpl_tmp[0], buff);
	    printf("             0%%Š€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ˆ€ƒ €€€€€€€€€€€€€€\n");
	    perc1=day_sec/100;
	    perc=(line_load[ckl].time_in+line_load[ckl].time_out)/perc1;
	    otstup();printf("%s Usage: %6.2f%%\n", tpl_numb, perc);
	}
    }
}

void otstup(){
    int ckl;
    for (ckl=0; ckl<otstup_num; ckl++){
	printf(" ");
    }
}

void to_long(unsigned char *x, int len){
    int ckl, ln=strlen(x);
    if (ln<len){
        for (ckl=ln; ckl<len-1; ckl++){
	    *(x+ckl)=*" ";
	}
	*(x+len)=0;
    }
}

void perevod_num(char *adp, unsigned int bytes_i){
    char x[128];
    double bytes = bytes_i;
    if (bytes>=1073741824){
	sprintf (x, "%0.1f", bytes / 1024 / 1024 / 1024);
	if (strlen(x)>4){
//	    printf("(%d)(%s)", strlen(x), x);
	    strcpy(strstr(x, "."), "G\0");
	}else{
	    strcpy(&x[strlen(x)], "G\0");
	}
    }else{
	if (bytes>=1048576){
	    sprintf (x, "%0.1f", bytes / 1024 / 1024);
	    if (strlen(x)>4){
//		printf("(%d)(%s)", strlen(x), x);
		strcpy(strstr(x, "."), "M\0");
	    }else{
		strcpy(&x[strlen(x)], "M\0");
	    }
	}else{
	    if (bytes>=1024){
		sprintf (x, "%0.1f", bytes / 1024);
		if (strlen(x)>4){
//		    printf("(%d)(%s)", strlen(x), x);
		    strcpy(strstr(x, "."), "K\0");
		}else{
		    strcpy(&x[strlen(x)], "K\0");
		}
	    }else{
		if ( bytes == 0 ){
		    x[0]=*"0";
		    x[1]=0;
		}else{
		    sprintf (x, "%0.1f", bytes);
		    if (strlen(x)>4){
//			printf("(%d)(%s)", strlen(x), x);
			strcpy(strstr(x, "."), "b\0");
		    }else{
			strcpy(&x[strlen(x)], "b\0");
		    }
		}
	    }
	}
    }
//    printf("%d, %s\n", bytes_i, x);
    strcpy(adp, x);
}

void perevod_time(char *adp, unsigned int second){
    char x[128], *y, ckl, ln;
    unsigned int h, m, s;
    h=second/60/60;
    m=(second-h*60*60)/60;
    s=(second-h*60*60-m*60);
    sprintf(x ,"%2d:%2d:%2d", h, m, s);
    ln=strlen(x);
    for (ckl=0; ckl<ln; ckl++){
	if (x[ckl]==*" "){
	    x[ckl]=*"0";
	}
    }
    strcpy(adp, x);
}

void find_name(unsigned char *fullname, unsigned char *desc, unsigned char *name_line){
    int ckl;
    char buff[1024];
    sprintf(buff, "%s/%s", desc, name_line);
    for (ckl=0; ckl<aliases_num; ckl++){
//	printf("@%s==%s@\n", conf_aliases[ckl].lines, buff);
	if (strstr(conf_aliases[ckl].lines, buff)){
	    strcpy(fullname, conf_aliases[ckl].alias);
//	    printf("Ok\n");
	    return;
	}
    }
    *fullname=0;
}

void line_load_group(){
    struct line_load_struct *tmp_line_load;
    int tmp_size=line_load_acc, ckl, ckl1, ckl2;
    if (!group_aliases){
	return;
    }
    tmp_line_load=(struct line_load_struct *)malloc(tmp_size*sizeof(struct line_load_struct));
    bzero(tmp_line_load, tmp_size*sizeof(struct line_load_struct));
    memcpy(tmp_line_load, line_load, tmp_size*sizeof(struct line_load_struct));
    bzero(line_load, tmp_size*sizeof(struct line_load_struct));
    line_load_acc=aliases_num;
    for (ckl=0; ckl<aliases_num; ckl++){
	strcpy(line_load[ckl].fullname, conf_aliases[ckl].alias);
	line_load[ckl].desc[0]=0;
	line_load[ckl].name_line[0]=0;
	for (ckl1=0; ckl1<tmp_size; ckl1++){
//	    printf("---%s---%s(%s/%s)---\n", line_load[ckl].fullname, tmp_line_load[ckl1].fullname, tmp_line_load[ckl1].desc, tmp_line_load[ckl1].name_line);
	    if (!strcmp(line_load[ckl].fullname, tmp_line_load[ckl1].fullname)){
//		printf("Ok\n");
		line_load[ckl].sess_in+=tmp_line_load[ckl1].sess_in;
		line_load[ckl].sess_out+=tmp_line_load[ckl1].sess_out;
		line_load[ckl].bytes_recv+=tmp_line_load[ckl1].bytes_recv;
		line_load[ckl].bytes_sent+=tmp_line_load[ckl1].bytes_sent;
		line_load[ckl].time_in+=tmp_line_load[ckl1].time_in;
		line_load[ckl].time_out+=tmp_line_load[ckl1].time_out;
		for (ckl2=0; ckl2<97; ckl2++){
		    line_load[ckl].field[ckl2]+=tmp_line_load[ckl1].field[ckl2];
		}
		line_load[ckl].field[96]=0;
	    }
	}
    }
    free(tmp_line_load);
}

unsigned int get_sess_text_num(char *buff){
    int ckl, len, stat=0;
    char tmp[1024];
    strcpy(tmp, buff);
    len=strlen(buff);
    for (ckl=len;ckl>0;ckl--){
	if (tmp[ckl]==*"]"){
	    tmp[ckl]=0;
	    stat=1;
	}
	if ((tmp[ckl]==*"[")&&(stat==1)){
	    return atoi(&tmp[ckl+1]);
	}
    }
    return 0;
}

unsigned int get_sess_text_time(char *buff){
    int ckl, len, stat=0;
    char tmp[1024], tmp_date[32];
    unsigned int x_sec=0, x_min=0, x_hour=0;
    strcpy(tmp, buff);
    sprintf(tmp_date, "%s %d", month[tm_mon], tm_mday);
    len=strlen(tmp_date);
    tmp[len+9]=0;
    x_sec=atoi(&tmp[len+7]);
    tmp[len+6]=0;
    x_min=atoi(&tmp[len+4]);
    tmp[len+3]=0;
    x_hour=atoi(&tmp[len+1]);
    return time_start+x_sec+x_min*60+x_hour*60*60;

}