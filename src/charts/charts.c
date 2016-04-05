/*
 * matrix.c
 *
 *  Created on: Sep 12, 2013
 *      Author: alex
 */

#include "charts.h"

#define BANDWITH 20
#define TF_FSCHANNEL "/tmp/fs_canais.txt"

int get_fuction_plot(char* equation, double freq, double signal_level, double bandwith)
{
	// Define the dimension n of the matrix
	// and the signum s (for LU decomposition)
	int n = 3;
	int s;

	// Define all the used matrices
	gsl_matrix * m = gsl_matrix_alloc (n, n);
	gsl_matrix * inverse = gsl_matrix_alloc (n, n);
	gsl_permutation * perm = gsl_permutation_alloc (n);

	// Fill the matrix m

	gsl_matrix_set(m, 0,0,pow(freq,2));
	gsl_matrix_set(m, 0,1,freq);
	gsl_matrix_set(m, 0,2,1);
	gsl_matrix_set(m, 1,0,pow(freq-bandwith/2,2));
	gsl_matrix_set(m, 1,1,freq-10);
	gsl_matrix_set(m, 1,2,1);
	gsl_matrix_set(m, 2,0,pow(freq+bandwith/2,2));
	gsl_matrix_set(m, 2,1,freq+10);
	gsl_matrix_set(m, 2,2,1);

	double signal[] = {signal_level,-100,-100};
	double xyz[3] = {0,0,0};

	int i,j;


	// Make LU decomposition of matrix m
	gsl_linalg_LU_decomp (m, perm, &s);

	// Invert the matrix m
	gsl_linalg_LU_invert (m, perm, inverse);


	for(i=0;i<n;i++){
				for(j=0;j< n;j++){
					xyz[i] = xyz[i] + gsl_matrix_get(inverse,i,j) * signal[j];
				}
	}

	snprintf(equation,100,"(%f)*x**2+(%f)*x+(%f)",xyz[0],xyz[1],xyz[2]);

	return 0;
}

int save_chart_ploted(gnuplot_ctrl *h1, char* namechart){

	char cmd[100];

	snprintf(cmd,100,"set output '%s'",namechart);

	gnuplot_cmd ( h1, "set terminal pngcairo size 1280,720 dashed");
	gnuplot_cmd ( h1, cmd);
	gnuplot_cmd ( h1, "replot");

	return 0;
}

int plot_fs_channels(gnuplot_ctrl *h1, wireless_scan_mi_list* list, char *headfile){

	int i;
	FILE *arquivo;
	char cmd[100];

	arquivo = fopen(TF_FSCHANNEL,"w");

	if (arquivo != NULL)
	{
		for(i = 0; i < CANAL ; i++)
				fprintf(arquivo,"%d %f\n",i+1,list->fator_sobreposicao[i]);
	}

	fclose(arquivo);

	sprintf(cmd,"load '%s'",headfile);
	gnuplot_resetplot ( h1 );
	gnuplot_cmd ( h1,cmd);

	snprintf(cmd,100,"plot \"%s\" using ($1):($2) t\"Overlap Factor\" with boxes",TF_FSCHANNEL);

	gnuplot_cmd ( h1,cmd);

	return 0;
}


int plot_bw_iface(gnuplot_ctrl *h1, int i , int maxbitrate  ,char *arquivo,  char *headfile){

	char cmd[100];

	sprintf(cmd,"load '%s'",headfile);
	gnuplot_resetplot ( h1 );
	gnuplot_cmd ( h1,cmd);

	snprintf(cmd,100,"set xrange [0:%d]",i);
	gnuplot_cmd ( h1,cmd);

	snprintf(cmd,100,"set yrange [0:%d]",maxbitrate);
	gnuplot_cmd ( h1,cmd);

	snprintf(cmd,100,"plot \"%s\" using ($1):($4/1000) t\"Tx\" with lines",arquivo);
	gnuplot_cmd ( h1,cmd);

	snprintf(cmd,100,"replot \"%s\" using ($1):($5/1000) t\"Rx\" with lines",arquivo);
	gnuplot_cmd ( h1,cmd);

	snprintf(cmd,100,"replot \"%s\" using ($1):($6/1000) t\"Total\" with lines",arquivo);
	gnuplot_cmd ( h1,cmd);

	snprintf(cmd,100,"replot \"%s\" using ($1):($9/1000) t\"Maximum\" with lines",arquivo);
	gnuplot_cmd ( h1,cmd);

	snprintf(cmd,100,"replot \"%s\" using ($1):($12/1000) t\"Average\" with lines",arquivo);
	gnuplot_cmd ( h1,cmd);

	return 0;
}


void plot_net_channel(gnuplot_ctrl *h1, wireless_scan_mi_list* list,char *headfile){

		wireless_scan_mi* current_device;
		current_device = list->head_list;

		char equation[100];
		char cmd[100];
		char plot_equation[200];

		int i=0;
		float freq_MHz;

		sprintf(cmd,"load '%s'",headfile);
		gnuplot_resetplot ( h1 );
		gnuplot_cmd ( h1,cmd);
		gnuplot_setstyle(h1, "lines");

		while(current_device != NULL){

			freq_MHz = current_device->frequencia/1000000;

			//printf("%f\n",freq_MHz);
/*
			if(!get_fuction_plot(equation,freq_MHz,current_device->nivel,BANDWITH)){
				snprintf(plot_equation,100,"y =(x >= %f && x <= %f ) ? %s : 0/0\n",freq_MHz-BANDWITH/2,freq_MHz+BANDWITH/2,equation);
				gnuplot_plot_equation(h1, equation, current_device->essid);
			}*/

			if(!get_fuction_plot(equation,freq_MHz,current_device->nivel,BANDWITH)){

							snprintf(plot_equation,100,"%c%c(x) =(x >= %0.2f && x <= %0.2f ) ? %s : 0/0",65+i,65+i,freq_MHz-BANDWITH/2,freq_MHz+BANDWITH/2,equation);

							gnuplot_cmd ( h1, plot_equation);

							if(i ==0)
								snprintf(plot_equation,100,"plot %c%c(x) with lines lt %d lc %d lw 2 t'%s'",65+i,65+i,i+1,i+1,current_device->essid);
							else
								snprintf(plot_equation,100,"replot %c%c(x) with lines lt %d lc %d lw 2 t'%s'",65+i,65+i,i+1,i+1,current_device->essid);

							gnuplot_cmd ( h1, plot_equation);
			}

			i++;

			current_device = current_device->next;
		}


}

void gnuplot_header(FILE* fp, float ymin, float ymax, char* type){

	char string[100];

	snprintf(string,100,"set terminal x11 size 800,600");
	fprintf(fp,"%s",string);

	snprintf(string,100,"\nset key outside Left");
	fprintf(fp,"%s",string);

	snprintf(string,100,"\nset title \"TripleS - Smart Sense Spectre\\n%s x Time\"",type);
	fprintf(fp,"%s",string);

	snprintf(string,100,"\nset yrange [%0.2f:%0.2f]",ymin,ymax);
	fprintf(fp,"%s",string);

	fprintf(fp,"\nset grid");
	fprintf(fp,"\nset xlabel \"Time\"");

	snprintf(string,100,"\nset ylabel \"%s\"",type);
	fprintf(fp,"%s",string);
}

int fprint_gnuplot_files(wireless_scan_mi_list* list){

	FILE *current_file, *fp,*fp2,*fp3,*fp4;
	char filename[50];
	int i=1;
	wireless_scan_mi* current_device;

	int chart=0;

	current_device = list->head_list;

	if ((fp = fopen("wifinetchannel.gnu", "w")) == NULL)
		return -1;

	if ((fp2 = fopen("wifinetquality.gnu", "w")) == NULL)
			return -1;

	if ((fp3 = fopen("wifinetsignal.gnu", "w")) == NULL)
			return -1;

	if ((fp4 = fopen("wifinetolf.gnu", "w")) == NULL)
			return -1;

	while(current_device != NULL){

		strcpy(filename,current_device->essid);
		strcat(filename,".dat");

		if (chart == 0 ){

			gnuplot_header(fp,0, 15, "Channels");
			fprintf(fp,"\nplot \"%s\" using ($1):($6) t\"%s\" with lines lt %d\n",filename,current_device->essid,i);

			gnuplot_header(fp2,0, 100, "Quality");
			fprintf(fp2,"\nplot \"%s\" using ($1):($8) t\"%s\" with lines lt %d\n",filename,current_device->essid,i);

			gnuplot_header(fp3,-20, -90, "Signal");
			fprintf(fp3,"\nset yrange [-30:-60] noreverse");
			fprintf(fp3,"\nplot \"%s\" using ($1):($9) t\"%s\" with lines lt %d\n",filename,current_device->essid,i);

			gnuplot_header(fp4,0, 500, "Overlap factor");
			fprintf(fp4,"\nplot \"%s\" using ($1):($12) t\"%s\" with lines lt %d\n",filename,current_device->essid,i);
			chart = 1;
		}
		else{
			fprintf(fp,"replot \"%s\" using ($1):($6) t\"%s\" with lines lt %d\n",filename,current_device->essid,i);
			fprintf(fp2,"replot \"%s\" using ($1):($8) t\"%s\" with lines lt %d\n",filename,current_device->essid,i);
			fprintf(fp3,"replot \"%s\" using ($1):($9) t\"%s\" with lines lt %d\n",filename,current_device->essid,i);
			fprintf(fp4,"replot \"%s\" using ($1):($12) t\"%s\" with lines lt %d\n",filename,current_device->essid,i);
		}

		i++;

		current_device = current_device->next;

	};


	fprintf(fp,"pause -1\n");
	fprintf(fp2,"pause -1\n");
	fprintf(fp3,"pause -1\n");
	fprintf(fp4,"pause -1\n");

	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);

	return 0;


}

int show_net_spectre(wireless_scan_mi_list* list, int amostras, int time,char *headfile){

	int i = 0;
	gnuplot_ctrl *h1;

	h1 = gnuplot_init ( );

	if ( h1 == NULL )
	{
		    printf ( "\n" );
		    printf ( "ANIM - Fatal error!\n" );
		    printf ( "  The gnuplot command is not in your path.\n" );
		    exit ( 1 );
	}

	while(i != amostras){
				gnuplot_resetplot ( h1 );
				gnuplot_cmd ( h1, "load 'spectre.gnu'");
				plot_net_channel(h1,&list,headfile);
				sleep(time);

				i++;
	}


	return 0;
}
