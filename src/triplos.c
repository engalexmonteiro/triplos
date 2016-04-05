/*
 * triploS.c
 *
 *  Created on: 05/03/2013
 *      Author: Alex Monteiro
 */

/*Dependences
 * libmysqlclient-dev:	-lmysqlclient
 * libiw-dev: 		  	-liw
 * libbluetooth-dev:	-lbluetooth
 * libconfuse-dev: 		-confuse
 * ligsl0-dev			-lgsl
 * gnuplot_i.h
 */


#include <iwlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>

//Internal libraries
#include "multinterfaces.h"  	// Structures multi-radio
#include "modules/mod80211.h"		 	// Structures and functions IEEE 802.11 Wi-fi
#include "modules/mod802151.h"			// Structures and functions IEEE 802.15.1 Bluetooth
#include "database/database.h"			// Functions of store in database MySQL
#include "charts/charts.h"
#include "bandwith/bwinterface.h"

//Samples Files
char *OVERLAPFACTOR = "fscanais.txt"; // this file store the overlap factor of the channels
char *WIFINETWORKS  = "varredura.txt"; // this file store the wifi network detected

char *IFACE;

char CONFIG_FILE[100] = "/etc/triplos/triplos.conf";

char BWIFACE[100] = "bwifaces.dat";
char COVERLAP_DATA[100] = "channeloverlap.dat";
char BEST_DATA[100] =  "bestchannel.dat";


char *CHAR_DIR;
char *CONF_DIR;
char *DATA_DIR;
char *GNU_DIR;

char TRIPLOS_DIR[100];
char GNU_FILE_BW[100];
char GNU_FILE_FS[100];
char GNU_FILE_SPEC[100];
char GNU_FILE_OLF[100];
char GNU_FILE_QUAL[100];
char GNU_FILE_SIGN[100];

char CHART_SPECTRE[100] = "spectreISM.png";
char CHART_FSCHANNELS[100] = "fschannels.png";
char CHART_BWIFACE[100] = "bwinterface.png";

//Global variables
static int amostras=0;
static int sample_time = 1;
static int saveindb = 0;
static int saveinfile =0;
static int show_charts =0;

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}


int init_triplos(char* fileconfig){

	   char dir_triplos[100];
	   sprintf(dir_triplos,"%s/%s",getenv("HOME"),"triplos");

	   char	 *TRIPLOS_DIR_TEMP = strdup(dir_triplos);

	   char *BWIFACE_TEMP = strdup("bwifaces.dat");
	   char *COVERLAP_TEMP = strdup("channeloverlap.dat");
	   char *BEST_TEMP = strdup("bestchannel.dat");

	   char *GNU_FILE_BW_TEMP = strdup("bwiface.gnu");
	   char *GNU_FILE_FS_TEMP  = strdup("fschart.gnu");
	   char *GNU_FILE_SPEC_TEMP  = strdup("spectre.gnu");
	   char *GNU_FILE_OLF_TEMP  = strdup("wifinetolf.gnu");
	   char *GNU_FILE_QUAL_TEMP  = strdup("wifinetquality.gnu");
	   char *GNU_FILE_SIGN_TEMP  = strdup("wifinetsignal.gnu");

	   char *CHART_SPECTRE_TEMP  = strdup("spectreISM.png");
	   char *CHART_FSCHANNELS_TEMP  = strdup("fschannels.png");
	   char *CHART_BWIFACE_TEMP = strdup("bwinterface.png");

	   IFACE = strdup("wlan0");
	   CHAR_DIR = strdup("charts");
	   CONF_DIR = strdup("conf");
	   DATA_DIR = strdup("data");
	   GNU_DIR = strdup("gnu");

	   cfg_t *cfg;
	   cfg_opt_t opts[] = {
			   	 CFG_SIMPLE_STR ("IFACE",&IFACE),
			   	 CFG_SIMPLE_STR ("TRIPLOS_DIR",&TRIPLOS_DIR_TEMP),
				 CFG_SIMPLE_STR ("BWIFACE",&BWIFACE_TEMP),
				 CFG_SIMPLE_STR ("OVERLAPFACTOR",&COVERLAP_TEMP), // this file store the overlap factor of the channels
				 CFG_SIMPLE_STR ("BEST_CHANNEL",&BEST_TEMP), // this file store the overlap factor of the channels
				 CFG_SIMPLE_STR ("GNU_FILE_BW",&GNU_FILE_BW_TEMP),
				 CFG_SIMPLE_STR ("GNU_FILE_FS",&GNU_FILE_FS_TEMP),
				 CFG_SIMPLE_STR ("GNU_FILE_SPEC",&GNU_FILE_SPEC_TEMP),
				 CFG_SIMPLE_STR ("GNU_FILE_OLF",&GNU_FILE_OLF_TEMP),
				 CFG_SIMPLE_STR ("GNU_FILE_QUAL",&GNU_FILE_QUAL_TEMP),
				 CFG_SIMPLE_STR ("GNU_FILE_SIGN",&GNU_FILE_SIGN_TEMP),
	             CFG_SIMPLE_STR ("CHART_SPECTRE", &CHART_SPECTRE_TEMP),
				 CFG_SIMPLE_STR ("CHART_FSCHANNELS", &CHART_FSCHANNELS_TEMP),
				 CFG_SIMPLE_STR ("CHART_BWIFACE", &CHART_BWIFACE_TEMP),
				 CFG_SIMPLE_STR ("WIFINETWORKS",&WIFINETWORKS), // this file store the wifi network detected
	             CFG_END()
	             };

	   cfg = cfg_init (opts, 0);

	   if(cfg_parse(cfg,fileconfig) == CFG_PARSE_ERROR)
		   syslog (LOG_INFO, "File not found, using default configuration");
	   else
		   syslog (LOG_INFO, "Config file found, using configuration defined");

	  sprintf(TRIPLOS_DIR,"%s/%s",TRIPLOS_DIR_TEMP,"triplos");
	  sprintf(COVERLAP_DATA,"%s/%s/%s",TRIPLOS_DIR,DATA_DIR,COVERLAP_TEMP);
	  sprintf(BEST_DATA,"%s/%s/%s",TRIPLOS_DIR,DATA_DIR,BEST_TEMP);
	  sprintf(BWIFACE,"%s/%s/%s",TRIPLOS_DIR,DATA_DIR,BWIFACE_TEMP);

	  sprintf(GNU_FILE_BW,"%s/%s/%s",TRIPLOS_DIR,GNU_DIR,GNU_FILE_BW_TEMP);
	  sprintf(GNU_FILE_FS,"%s/%s/%s",TRIPLOS_DIR,GNU_DIR,GNU_FILE_FS_TEMP);
	  sprintf(GNU_FILE_SPEC,"%s/%s/%s",TRIPLOS_DIR,GNU_DIR,GNU_FILE_SPEC_TEMP);
	  sprintf(GNU_FILE_OLF,"%s/%s/%s",TRIPLOS_DIR,GNU_DIR,GNU_FILE_OLF_TEMP);
	  sprintf(GNU_FILE_QUAL,"%s/%s/%s",TRIPLOS_DIR,GNU_DIR,GNU_FILE_QUAL_TEMP);
	  sprintf(GNU_FILE_SIGN,"%s/%s/%s",TRIPLOS_DIR,GNU_DIR,GNU_FILE_SIGN_TEMP);

	  sprintf(CHART_SPECTRE,"%s/%s/%s",TRIPLOS_DIR,CHAR_DIR,CHART_SPECTRE_TEMP);
	  sprintf(CHART_FSCHANNELS,"%s/%s/%s",TRIPLOS_DIR,CHAR_DIR,CHART_FSCHANNELS_TEMP);
	  sprintf(CHART_BWIFACE,"%s/%s/%s",TRIPLOS_DIR,CHAR_DIR,CHART_BWIFACE_TEMP);

	  cfg_free(cfg);       /* Libera a memória alocada por cfg_init */
	  return 0;
}

int help_info(){

	printf("\nTriploS - Smart Sense Spectre\n");
	 printf("midev-agent [OPTION] [ARGUMENT]\n");
	printf("OPTIONS:\n\t\t-n [value]\t: number of samples");
	printf("\n\t\t-t [time]\t: period of samples in seconds");
	printf("\n\t\t-a [config file]: configuration file");
	printf("\n\t\t-d\t\t: Store in database");
	printf("\n\t\t-f\t\t: Save in txt file");
	printf("\n\t\t-c\t\t: Show Charts with Gnuplot\n");
	return 0;
};

int save_scan_mi_file(wireless_scan_mi_list* list, time_t currentTime){

	struct tm *timeinfo = localtime(&currentTime);

	int i;
	int menor_fator;
	int melhor_canal;
	wireless_scan_mi* current_device;

	char string[SIZEQUERY];

	FILE *arquivo;


	current_device = list->head_list;

	if(current_device != NULL){

		arquivo = fopen(WIFINETWORKS,"a");

		if (arquivo != NULL)
		{

			while(current_device != NULL){

					snprintf(string,SIZEQUERY,"%d-%d-%d;%d:%d:%d;wifi;%s;%d;%d;%0.2f;%0.2f;%d;%d;%s;%0.2f;%d/%d;%s\n",
								timeinfo->tm_year+1900,timeinfo->tm_mon,timeinfo->tm_mday,
								timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
								current_device->mac_address,
								current_device->key,
								current_device->canal,
								current_device->frequencia,
								current_device->qualidade,
								current_device->nivel,
								current_device->maxbitrate,
								current_device->modo,
								current_device->factor_overlap,
								current_device->score_snr,
								list->size_list,
								current_device->essid);

								if(EOF == fputs(string,arquivo))
									printf("Erro ao gravar lista de redes detectadas");

								current_device = current_device->next;

				};

		}
		else
		{
			printf("Falha ao abrir arquivo varredura.txt\n");
		}

		fclose(arquivo);

		menor_fator = list->fator_sobreposicao[0];
		melhor_canal = 1;

		arquivo = fopen(OVERLAPFACTOR,"a");

		if (arquivo != NULL)
		{


			for(i = 0; i < CANAL-1 ; i++){

				if(menor_fator > list->fator_sobreposicao[i]){
					menor_fator = list->fator_sobreposicao[i];
					melhor_canal = i+1;
				}
			}

			snprintf(string,SIZEQUERY,"%d-%d-%d;%d:%d:%d;",
				timeinfo->tm_year+1900,timeinfo->tm_mon,timeinfo->tm_mday,
				timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

			if(EOF == fputs(string,arquivo))
				printf("Erro ao gravar lista de fatores de sobreposicao de canais");

			for(i = 0; i < CANAL-1 ; i++){
				snprintf(string,SIZEQUERY,"%0.2f;",list->fator_sobreposicao[i]);

				if(EOF == fputs(string,arquivo))
					printf("Erro ao gravar fator de sobreposicao do canal %d",i+1);
			}

			snprintf(string,SIZEQUERY,"%d;%d\n",melhor_canal,list->factor_diversity);

			if(EOF == fputs(string,arquivo))
				printf("Erro ao gravar lista de fatores de sobreposicao de canais");

			}else{
				printf("Erro ao abrir arquivo fscanais.txt");
			}

		fclose(arquivo);

	}

	return 0;
}

int varredura(char* iface, wireless_scan_mi_list* list){

	list->head_list = NULL;
	list->end_list = NULL;
	list->size_list = 0;
	list->factor_diversity = 0;

	time_t currentTime;
	struct tm *timeinfo;

	iw_scan_wifiv2(iface,list);

	score_net_snr(list);

	/* Pega a hora atual do sistema. */
	currentTime= time(NULL);

	//if(saveinfile)
		//save_scan_mi_file(list,currentTime);

	if(saveindb)

		save_scan_mi_BD(list,currentTime);

			/* Converte-o em uma estrutura tm. */
	timeinfo= localtime(&currentTime);

			 /* Apresenta a hora atual no formato horas:minutos:segundos */
	printf("\nData: %02d/%02d/%d Hora atual: %02d:%02d:%02d\n", timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year+1900,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

//	print_classi_network(list);

	print_scan_mi(list);

	return 0;
}



int main(int argc,char **argv){

	int i=0;
	int option;


	wireless_scan_mi_list list;
	host_interfaces_t host_ifaces;

	host_ifaces.first_pass = 1;

	gnuplot_ctrl *h1, *h2, *h3;

		h1 = gnuplot_init ( );
		h2 = gnuplot_init ( );
		h3 = gnuplot_init ( );

		openlog ("TriploS",LOG_PID,LOG_USER);

		if ( h1 == NULL || h2 == NULL || h3 == NULL )
		{
			    printf ( "\n" );
			    printf ( "ANIM - Fatal error!\n" );
			    printf ( "  The gnuplot command is not in your path.\n" );
			    exit ( 1 );
		}


	while( (option=getopt(argc, argv, "n:dft:a:hc")) != -1 ){
		switch(option){
			case 'n' : amostras = atoi(optarg);
				   	   break;
			case 'd' : saveindb = 1;
				   	   printf("\nThis parameter save in database. Please before set the triplos.conf \n");
				   	   break;
			case 'f' : saveinfile = 1;
	    		   	   printf("\nThis parameter save in .txt files \n");
	    		   	   break;
			case 't' : sample_time = atoi(optarg);
				   	   break;
			case 'a' : strcpy(CONFIG_FILE,optarg);
					   break;
			case 'h' : help_info();
					   return 0;
				   	   break;
			case 'c' : show_charts=1;
					   break;
			default: amostras = -1;
				 	 break;
		}
	}

	init_triplos(CONFIG_FILE);

	if(amostras < 1){
		while(!kbhit()){
				system("clear");

				varredura("wlan0",&list);

				file_for_bw(i,&host_ifaces, IFACE,BWIFACE);

				printf("\nPress Enter for exit\n");

				if(show_charts){
					gnuplot_resetplot ( h1 );
					gnuplot_cmd ( h1, "load 'spectre.gnu'");
					gnuplot_setstyle(h1, "lines");
					plot_net_channel(h1,&list,GNU_FILE_SPEC);
					plot_fs_channels(h2,&list,GNU_FILE_FS);

					if(i > 1)
						plot_bw_iface(h3,i,54,BWIFACE,GNU_FILE_BW);
				}

				if(saveinfile){
						fprint_scan_mi(i,&list,TRIPLOS_DIR);
						fprint_olf(i, &list, "channeloverlap.dat");
						fprint_best_channels(i, &list, "bestchannel.dat");

						if(i == amostras-1)
							fprint_gnuplot_files(&list);
				}

				i++;

				limpar_scan_mi(&list);
				sleep(sample_time);
		}


	}else{
		for(i = 0; i< amostras ; i++){

			system("clear");

			varredura(IFACE,&list);

			file_for_bw(i,&host_ifaces, IFACE,BWIFACE);

			if(show_charts){
				plot_net_channel(h1,&list,GNU_FILE_SPEC);
				plot_fs_channels(h2,&list,GNU_FILE_FS);

				if(i > 1)
				plot_bw_iface(h3,i,20,BWIFACE,GNU_FILE_BW);
			}

			if(saveinfile){
					fprint_scan_mi(i,&list,TRIPLOS_DIR);
					fprint_olf(i, &list,COVERLAP_DATA);
					fprint_best_channels(i, &list,BEST_DATA);

					if(i == amostras-1)
						fprint_gnuplot_files(&list);
			}

			sleep(sample_time);

			limpar_scan_mi(&list);

		}


	}

	if(show_charts){
		save_chart_ploted(h1, CHART_SPECTRE);
		save_chart_ploted(h2, CHART_FSCHANNELS);
		save_chart_ploted(h3, CHART_BWIFACE);
	}

	closelog ();

	printf("Powered by ETSS/ICOMP/Federal University of Amazonas\n");

	if(saveindb)
	    printf("\nThis samples was saved in database\n");

	if(saveinfile)
		printf("\nThis samples was saved in files varredura.txt, MCI.txt e fscanais.txt\n");

	return 0;
}
