/*
 * network.h
 *
 *  Created on: Mar 6, 2014
 *      Author: alex
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>


char *getlocalipaddress(char* iface);

int ping(char *adress);

#endif /* NETWORK_H_ */
