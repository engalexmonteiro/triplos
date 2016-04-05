/*
 * database.h
 *
 *  Created on: Aug 7, 2013
 *      Author: alex
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <time.h>
#include <confuse.h>
#include <mysql/mysql.h>
#include "../multinterfaces.h"

// Paramêtros de acesso da Base de dados MySQL
#define TABLESCAN "varredura"
#define TABLEMCI "MCI"
#define TABLEFSCANAIS "fscanais"
#define SIZEQUERY 300
//#############################################

int init_database();

void print_information();

int save_scan_mi_BD(wireless_scan_mi_list* list, time_t currentTime);

#endif /* DATABASE_H_ */
