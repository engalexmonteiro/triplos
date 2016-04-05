/*
 * mod802151.c
 *
 *  Created on: Aug 7, 2013
 *      Author: alex
 */


#include "mod802151.h"

int convertebinario(uint8_t *map, int* map_afh_int)
{
		int i,j; //declaração das variáveis necessárias
		uint8_t dec, q;

	//	for(i=0;i<80;i++) map_afh_int[i] = 0;

		for(i=0;i<10;i++){

			dec = map[i];

			/*Algoritmo para o cálculo */
			for(j = 7 + 8*i; j >= 8*i ; j--)   {
				q = dec / 2;
				map_afh_int[j] = dec % 2;
				dec = q;
			}

		}

		return(0);
}

void print_afh_channels(int* map_afh_int){

	int i;

	printf("\nGood channels: ");
	for(i = 1; i <= 79 ; i++){
		if(map_afh_int[i]) printf("%d, ",i);
	}

	printf("\nBad channels: ");
	for(i = 1; i <= 79 ; i++){
			if(!map_afh_int[i]) printf("%d, ",i);
	}

}

void print_realrssi(int8_t rssi){

	if(rssi < 0)
		printf("%d",-56+rssi);
	else
		if(rssi == 0)
			printf("%d",-51);
		else
			printf("%d",-46 + rssi);
}

static int connect_scan(int sock, int dev_id)
{
	struct hci_conn_list_req *cl;
	struct hci_conn_info *ci;
	int i;


	char name[248] = { 0 };
	//add
	  // Create conection parameters
	uint16_t handle;
	uint8_t role;
	unsigned int ptype;

	role = 0x01;
	ptype = HCI_DM1 | HCI_DM3 | HCI_DM5 | HCI_DH1 | HCI_DH3 | HCI_DH5;
	//----

	struct hci_conn_info_req *cr;
	int8_t rssi;
	uint8_t lq;
	int8_t level;

	uint8_t type;
	uint8_t reason;
	uint8_t mode, map[10];
	int  map_afh_int[80];


	//end add


	if (!(cl = malloc(10 * sizeof(*ci) + sizeof(*cl)))) {
		perror("Can't allocate memory");
		exit(1);
	}
	cl->dev_id = dev_id;
	cl->conn_num = 10;
	ci = cl->conn_info;

	if (ioctl(sock, HCIGETCONNLIST, (void *) cl)) {
		perror("Can't get connection list");
		exit(1);
	}

	printf("\tBDADDR\t\tDevice name\tRSSI\tQuality\t\tTPL\tAFH MAP\n");

	for (i = 0; i < cl->conn_num; i++, ci++) {
		char addr[18];
		char *str;
		ba2str(&ci->bdaddr, addr);
		str = hci_lmtostr(ci->link_mode);

	    //Read name
	    if (hci_read_remote_name(sock, &ci->bdaddr, sizeof(name),
	            name, 0) < 0)
	        strcpy(name, "[unknown]");

	    //Print BD_ADDR and NAME DEVICE
	    printf("%s\t%s\t", addr, name);


	    //Rssi, link, quality, tpl
	    cr = malloc(sizeof(*cr) + sizeof(struct hci_conn_info));
	    if (!cr) {
	      	perror("Can't allocate memory");
	 		exit(1);
	    }

	    bacpy(&cr->bdaddr, &ci->bdaddr);
	    cr->type = ACL_LINK;


	    if (ioctl(sock, HCIGETCONNINFO, (unsigned long) cr) < 0) {
	       	perror("Get connection info failed");
	       	exit(1);
	    }

	    if (hci_read_rssi(sock, htobs(cr->conn_info->handle), &rssi, 1000) < 0) {
	       	perror("Read RSSI failed");
	       	exit(1);
	    }

	    print_realrssi(rssi);
	    printf(" dBm\t");

	    if (hci_read_link_quality(sock, htobs(cr->conn_info->handle), &lq, 1000) < 0) {
	       	perror("HCI read_link_quality request failed");
	       	exit(1);
	    }

	    printf("%d\t", lq);

	    if (hci_read_transmit_power_level(sock, htobs(cr->conn_info->handle), type, &level, 1000) < 0) {
	       	perror("HCI read transmit power level request failed");
	 		exit(1);
	    }

	    printf("%s %d\t",(type == 0) ? "Current" : "Maximum", level);

	    handle = htobs(cr->conn_info->handle);

	    if (hci_read_afh_map(sock, handle, &mode, map, 1000) < 0) {
	 		perror("HCI read AFH map request failed");
	 		exit(1);
	 	}

	    if (mode == 0x01) {
	         	int i;
	         	printf("0x");
	         	for (i = 0; i < 10; i++)
	         		printf("%02x", map[i]);
	         		//printf("%02x", map[i]);

	         	printf("\n");

	         	convertebinario(map,map_afh_int);

	         	print_afh_channels(map_afh_int);


	         	printf("\n");

	         } else
	         	printf("AFH disabled\n");

/*
		printf("\t%s %s %s handle %d state %d lm %s\n",
			ci->out ? "<" : ">", type2str(ci->type),
			addr, ci->handle, ci->state, str);
*/


		bt_free(str);
	}

	free(cl);
	return 0;
}

static int inquiry_scan(int sock, int dev_id){
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    // Create conection parameters
	uint16_t handle;
	uint8_t role;
	unsigned int ptype;

	role = 0x01;
	ptype = HCI_DM1 | HCI_DM3 | HCI_DM5 | HCI_DH1 | HCI_DH3 | HCI_DH5;
    //----
	struct hci_conn_info_req *cr;
	int8_t rssi;
	int8_t lq;
	int8_t level;

	uint8_t type;
	uint8_t reason;
	uint8_t mode, map[10];
	int  map_afh_int[80];

	type =  0;
	reason =  HCI_OE_USER_ENDED_CONNECTION;

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    printf("\tBDADDR\t\tDevice name\tRSSI\tQuality\t\tTPL\tAFH MAP\n");

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));

        //Read name
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
            name, 0) < 0)
        strcpy(name, "[unknown]");

        printf("%s\t%s\t", addr, name);

        //Create connection
        if (hci_create_connection(sock, &(ii+i)->bdaddr, htobs(ptype),
				htobs(0x0000), role, &handle, 25000) < 0)
        	perror("Can't create connection");

        //Rssi, link, quality, tpl
        cr = malloc(sizeof(*cr) + sizeof(struct hci_conn_info));
        if (!cr) {
        	perror("Can't allocate memory");
		exit(1);
        }

        bacpy(&cr->bdaddr, &(ii+i)->bdaddr);
        cr->type = ACL_LINK;


        if (ioctl(sock, HCIGETCONNINFO, (unsigned long) cr) < 0) {
        	perror("Get connection info failed");
        	exit(1);
        }

        if (hci_read_rssi(sock, htobs(cr->conn_info->handle), &rssi, 1000) < 0) {
        	perror("Read RSSI failed");
        	exit(1);
        }

        printf("%d\t", rssi);

        if (hci_read_link_quality(sock, htobs(cr->conn_info->handle), &lq, 1000) < 0) {
        	perror("HCI read_link_quality request failed");
        	exit(1);
        }

        printf("%d\t", lq);

        if (hci_read_transmit_power_level(sock, htobs(cr->conn_info->handle), type, &level, 1000) < 0) {
        	perror("HCI read transmit power level request failed");
		exit(1);
        }

        printf("%s %d\t",
        		(type == 0) ? "Current" : "Maximum", level);

        handle = htobs(cr->conn_info->handle);

        if (hci_read_afh_map(sock, handle, &mode, map, 1000) < 0) {
			perror("HCI read AFH map request failed");
			exit(1);
		}

        if (mode == 0x01) {
        	int i;
        	printf("0x");
        	for (i = 0; i < 10; i++)
        		printf("%02x", map[i]);
        		//printf("%02x", map[i]);

        	printf("\n");

        	convertebinario(map,map_afh_int);

        	print_afh_channels(map_afh_int);


        	printf("\n");

        } else
        	printf("AFH disabled\n");



        if (hci_disconnect(sock, htobs(cr->conn_info->handle),
						reason, 10000) < 0)
        	perror("Disconnect failed");

        free(cr);

    }

    free( ii );

	return 0;
}


int iw_scan_bluetooth(wireless_scan_mi_list* list){

    int dev_id, sock;


    	dev_id = hci_get_route(NULL);
    	sock = hci_open_dev( dev_id );

    	if (dev_id < 0 || sock < 0) {
    		perror("opening socket");
    		exit(1);
    	}

    	connect_scan(sock,dev_id);

    	//inquiry_scan(int sock, int dev_id);

    	close( sock );

	    return 0;

}

int test_scan()
{
    int dev_id, sock;


    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    connect_scan(sock,dev_id);

    //inquiry_scan(int sock, int dev_id);

    close( sock );
    return 0;
}
