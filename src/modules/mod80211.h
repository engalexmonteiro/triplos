/*
 * midiscovery.h
 *
 *  Created on: 05/03/2013
 *      Author: alex
 */

#ifndef MIDISCOVERY_H_
#define MIDISCOVERY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
// Librarys Bluetooth and Wi-fi
#include <iwlib.h>

#include "../multinterfaces.h"


int iw_scan_wifi(wireless_scan_mi_list* list);

int iw_scan_wifiv2(char* iface, wireless_scan_mi_list* list);

int score_net_snr(wireless_scan_mi_list* list);

int print_scan_mi(wireless_scan_mi_list* list);

int print_classi_network(wireless_scan_mi_list* list);

int fprint_scan_mi(int itr, wireless_scan_mi_list* list, char *prefix_path);

int fprint_olf(int itr, wireless_scan_mi_list* list, char* filename);

int fprint_best_channels(int itr, wireless_scan_mi_list* list, char* filename);

void limpar_scan_mi(wireless_scan_mi_list* list);


#endif /* MIDISCOVERY_H_ */
