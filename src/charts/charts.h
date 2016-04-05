/*
 * charts2.h
 *
 *  Created on: Mar 17, 2016
 *      Author: tizen
 */

#ifndef SRC_CHARTS_CHARTS_H_
#define SRC_CHARTS_CHARTS_H_

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_cblas.h>
#include <math.h>
#include "gnuplot_i.h"
#include "../multinterfaces.h"
#include "../modules/mod80211.h"
#include "../bandwith/bwinterface.h"


int get_fuction_plot(char* equation, double freq, double signal_level, double bandwith);

void plot_net_channel(gnuplot_ctrl *h1, wireless_scan_mi_list* list, char *headfile);

int show_net_spectre(wireless_scan_mi_list* list, int amostras, int time, char *headfile);

int plot_fs_channels(gnuplot_ctrl *h1, wireless_scan_mi_list* list,  char *headfile);

int plot_bw_iface(gnuplot_ctrl *h1, int i , int maxbitrate  ,char *arquivo,  char *headfile);

int fprint_gnuplot_file(wireless_scan_mi_list* list);


#endif /* SRC_CHARTS_CHARTS_H_ */
