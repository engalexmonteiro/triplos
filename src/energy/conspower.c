/*
 ============================================================================
 Name        : TestACPI.c
 Author      : Alex Monteiro
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "conspower.h"

int get_info_energy(energy_md * emd) {

	global_t *global = malloc (sizeof (global_t));

	if(check_acpi_support() == SUCCESS){

        init_acpi_batt(global);
        init_acpi_acadapt(global);

        read_acpi_acstate(global);

		if(!global->adapt.ac_state){
			emd->ac = 1;
		}else{
			emd->ac = 0;
		}

		read_acpi_batt(0);

		emd->charge_time = batteries[0].charge_time;
		emd->design_cap = batteries[0].design_cap;
		emd->design_voltage = batteries[0].design_voltage / 1000;
		emd->last_full_cap = batteries[0].last_full_cap;
		emd->percentage = batteries[0].percentage;
		emd->present = batteries[0].present;
		emd->present_rate = batteries[0].present_rate;
		emd->present_voltage = batteries[0].present_voltage;
		emd->remaining_cap = batteries[0].remaining_cap;

	 }

	  return EXIT_SUCCESS;
}

int print_info_energy(energy_md * emd) {

		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
		emd->ac,
		emd->charge_time,
		emd->design_cap,
		emd->design_voltage,
		emd->last_full_cap,
		emd->percentage,
		emd->present,
		emd->present_rate,
		emd->present_voltage,
		emd->remaining_cap);

	  return 0;
}


int print_all_info_energy(int itr, energy_md * emd) {

		if (itr == 0)
			printf("#S\tAC\tMin\tmAh\tV\tmAh\t%%\tOn\tRate\tV\tMin\n");

		printf("%d\t",itr);

		print_info_energy(emd);

	  return 0;
}

int fprint_info_energy(FILE *file, energy_md * emd) {

		fprintf(file,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
		emd->ac,
		emd->charge_time,
		emd->design_cap,
		emd->design_voltage,
		emd->last_full_cap,
		emd->percentage,
		emd->present,
		emd->present_rate,
		emd->present_voltage,
		emd->remaining_cap);

	  return 0;
}


int fprint_all_info_energy(int itr, energy_md * emd,char* namefile) {

		FILE *file;

		if (itr == 0){
			file = fopen(namefile,"w");
			fprintf(file, "#Sa\tAC\tMin\tmAh\t\tV\tmAh\t\t%%\tOn\tRate\tV\tMin\n");
		}else
			file = fopen(namefile,"a");

		fprintf(file,"%d\t",itr);

		fprint_info_energy(file,emd);

		fclose(file);

	  return 0;
}

int teste(int n){

	energy_md * emd=NULL;
	int i;

	for(i=0; i< n; i++){
		get_info_energy(emd);
		print_all_info_energy(i,emd);
		fprint_all_info_energy(i,emd,"powerconsume");
		sleep(1);
	}

	return 0;
}
