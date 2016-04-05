/*
 * multinterfaces.h
 *
 *  Created on: Aug 7, 2013
 *      Author: alex
 */

#ifndef MULTINTERFACES_H_
#define MULTINTERFACES_H_



#define CANAL 14

typedef struct wireless_scan_mi
{
  /* Linked list */
  struct wireless_scan_mi *	next;

  /* Device identification */
  int 		type;					/* Bluetooth or Wi-fi */
  char		mac_address[50];		/* Access point address */
  char		essid[50];				/* Access point address */
  int		key;
  int 		canal;					/* Channel for wi-fi */
  int       map_afh_int[80];         /* AFG Map for bluetooth */

  float 	frequencia;				/* Frequence for wi-fi */
  float		qualidade;				/* Quality for wi-fi */
  int		nivel;					/* Level for bluetooth or wi-fi */
  int 		maxbitrate;				/* Maxbitrate for wi-fi */
  char 		modo[50];				/* Mode for blueooth: slave or master and wi-fi */
  float 	factor_overlap;			/* Factor of Overlap */
  int		score_snr;
  int 		service;				/*Service disponibility */
  int 		connected;				/*connectado */

} wireless_scan_mi;


/*
 * Context used for non-blocking scan.
 */
typedef struct wireless_scan_mi_list
{
  wireless_scan_mi*		head_list;				/* Result of the scan */
  wireless_scan_mi*		end_list;
  int 					channel_util[CANAL];	/* Canais utilizados 802.11 */
  float 				fator_sobreposicao[CANAL];	/* Canais utilizados 802.11 */
  int					size_list;		        /* Tamanho da lista */
  int 					factor_diversity;
  int				 	melhores_canais[CANAL];
  char*					iface;

} wireless_scan_mi_list;

typedef struct current_net
{
  wireless_scan_mi*		head_list;				/* Result of the scan */
  wireless_scan_mi*		end_list;
  int 					channel_util[CANAL];	/* Canais utilizados 802.11 */
  float 				fator_sobreposicao[CANAL];	/* Canais utilizados 802.11 */
  int					size_list;		        /* Tamanho da lista */
  int 					factor_diversity;
  int				 	melhores_canais[CANAL];

} current_net;

/* Modes as human readable strings */
static char *wi_type_device[2] = {"802.11","802.15.1"};

#endif /* MULTINTERFACES_H_ */
