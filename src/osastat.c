#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

#include "osastat.h"

int main(int argc, char **argv)
{
    char file_config_name[1024], ckl, ckl1;

    if (argc>1){
	strcpy(file_config_name, argv[1]);
    }else{
	about();
	printf("osastat <configuration_file> [<number_of_back_days..>]\n");
	return 1;
    }
    if (argc>2){
	set_time(atoi(argv[2]));
    }else{
	set_time(0);
    }

    if (configure(file_config_name)!=0) return 1;

    stat_screen=(struct stat_screen_base *)malloc(NUM_Z*sizeof(struct stat_screen_base));
    line_load=(struct line_load_struct *)malloc(NUM_Y*sizeof(struct line_load_struct));
    bzero(stat_screen, NUM_Z*sizeof(struct stat_screen_base));
    bzero(line_load, NUM_Y*sizeof(struct line_load_struct));

    for (ckl=0; ckl<configure_num; ckl++){
//	printf("n - %s\nt - %s\nf - %s\nnum = %d\n", configuration_key[ckl].conf_name, configuration_key[ckl].conf_type, configuration_key[ckl].conf_file, ckl);
	if (!strcmp(configuration_key[ckl].conf_type,log_types[0])){
	    process_bforce(configuration_key[ckl].conf_name,configuration_key[ckl].conf_file);
	}
	if (!strcmp(configuration_key[ckl].conf_type,log_types[1])){
	    process_binary(configuration_key[ckl].conf_name,configuration_key[ckl].conf_file);
	}
	if (!strcmp(configuration_key[ckl].conf_type,log_types[2])){
	    process_qico(configuration_key[ckl].conf_name,configuration_key[ckl].conf_file);
	}
    }
    if (stat_screen_acc!=0){
	intro();
	print_time();
	sort_order_prepare();
	sort_aka();
	statistics_draw();
	line_load_group();
	line_load_draw();
	epilog();
	about();
    }
//    for (ckl=0; ckl<stat_screen_acc; ckl++){
//	printf("\n%d| %d:%d/%d.%d sess:%d/rx:%d/tx:%d\n", ckl, stat_screen[ckl].zone, stat_screen[ckl].net, stat_screen[ckl].node, stat_screen[ckl].point, stat_screen[ckl].sess, stat_screen[ckl].bytesrcv, stat_screen[ckl].bytessnt);
//	printf("sess in:%d out:%d all:%d\n", sess_in, sess_out, sess_all);
//	printf("traff in:%d out:%d all:%d\n", bytes_recv, bytes_sent, bytes_all);
//	for (ckl1=0; ckl1<96; ckl1++){
//	    printf("%d", stat_screen[ckl].field[ckl1]);
//	}
//    }
//  for (ckl=0; ckl<line_load_acc; ckl++){
//	printf("\n%s/%s/%d/%d\n", line_load[ckl].desc, line_load[ckl].name_line, line_load[ckl].sess_in, line_load[ckl].sess_out);
//	for (ckl1=0; ckl1<96; ckl1++){
//	    printf("%d", line_load[ckl].field[ckl1]);
//	}
//    }
  
//    printf("%d\n", configure_num);

//    for(ckl=0; ckl<stat_screen_acc; ckl++){
//	printf("%2d| %d:%d/%d.%d sess:%d/rx:%d/tx:%d\n", ckl, stat_screen[ckl].zone, stat_screen[ckl].net, stat_screen[ckl].node, stat_screen[ckl].point, stat_screen[ckl].sess, stat_screen[ckl].bytesrcv, stat_screen[ckl].bytessnt);
//	for (ckl1=0; ckl1<96; ckl1++){
//	    printf("%d", stat_screen[ckl].field[ckl1]);
//	}
//    }

    free(line_load);
    free(stat_screen);
    return 0;
}
