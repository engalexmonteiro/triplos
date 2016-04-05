/*
 * database.c
 *
 *  Created on: Aug 7, 2013
 *      Author: alex
 */

#include "database.h"

char *host;
char *userdb;
char *passworddb;
char *database;

int init_database(char* databasefile){



	   cfg_t *cfg;
	   cfg_opt_t opts[] = {
	             CFG_SIMPLE_STR ("HOST", &host),
	             CFG_SIMPLE_STR ("USERDB", &userdb),
	             CFG_SIMPLE_STR ("PASSWORD", &passworddb),
	             CFG_SIMPLE_STR ("DATABASE", &database),
	             CFG_END()
	             };

	   cfg = cfg_init (opts, 0);

	   if(cfg_parse(cfg,databasefile) == CFG_PARSE_ERROR)
		   return 1;


	   cfg_free(cfg);       /* Libera a memória alocada por cfg_init */
	   return 0;
}

void print_information(){
	   printf ("\nHost: %s \n", host);
	   printf ("User DB: %s \n", userdb);
	   printf ("password: %s \n", passworddb);
	   printf ("Database: %s \n", database);
}



int save_scan_mi_BD(wireless_scan_mi_list* list, time_t currentTime){

	struct tm *timeinfo = localtime(&currentTime);

	int i;
	int menor_fator;
	int melhor_canal;
	wireless_scan_mi* current_device;

	char query_string[250];

	MYSQL conexao;

	mysql_init(&conexao);
		if (mysql_real_connect(&conexao, host, userdb, passworddb, database, 0, NULL, 0))
		{
			printf("conectado com sucesso!\n");

			current_device = list->head_list;

			while(current_device != NULL){

					snprintf(query_string,SIZEQUERY,"INSERT INTO %s VALUES('%d-%d-%d','%d:%d:%d','wifi','%s','%d','%d','%f','%f','%d','%d','%s','%f','%s');",
								TABLESCAN,
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
								current_device->essid);

								mysql_query(&conexao,query_string);

								//printf("\n%s",query_string); Função para teste da string de inserção

								current_device = current_device->next;

			};

				menor_fator = list->fator_sobreposicao[0];
				melhor_canal = 1;

				for(i = 0; i < CANAL-1 ; i++){

					if(menor_fator > list->fator_sobreposicao[i]){
						menor_fator = list->fator_sobreposicao[i];
						melhor_canal = i+1;
					}

					snprintf(query_string,SIZEQUERY,"INSERT INTO %s(data,hora,canal,fsobreposicao)"
							" values('%d-%d-%d','%d:%d:%d','%d','%f');",
							TABLEFSCANAIS,
							timeinfo->tm_year+1900,timeinfo->tm_mon,timeinfo->tm_mday,
							timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
							i+1,
							list->fator_sobreposicao[i]
					);

			//		printf("\n%s",query_string); Função para teste da string de inserção
					mysql_query(&conexao,query_string);
				}

				snprintf(query_string,SIZEQUERY,"INSERT INTO %s(data,hora,melhorcanal,fatordiversidade)"
						" values('%d-%d-%d','%d:%d:%d','%d','%d');",
						TABLEMCI,
						timeinfo->tm_year+1900,timeinfo->tm_mon,timeinfo->tm_mday,
						timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
						melhor_canal,
						list->factor_diversity);

				mysql_query(&conexao,query_string);

		//		printf("\n%s",query_string); Função para teste da string de inserção

			    mysql_close(&conexao);

		}
		else
		{
		     printf("Falha de conexao\n");
		     printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
		}

	return 0;
}
