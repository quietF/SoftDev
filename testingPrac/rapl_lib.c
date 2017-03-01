/* Read the RAPL registers on a sandybridge-ep machine                */
/* Code based on Intel RAPL driver by Zhang Rui <rui.zhang@intel.com> */
/*                                                                    */
/* The /dev/cpu/??/msr driver must be enabled and permissions set     */
/* to allow read access for this to work.                             */
/*                                                                    */
/* Code to properly get this info from Linux through a real device    */
/*   driver and the perf tool should be available as of Linux 3.14    */
/* Compile with:   gcc -O2 -Wall -o rapl-read rapl-read.c -lm         */
/*                                                                    */
/* Vince Weaver -- vincent.weaver @ maine.edu -- 29 November 2013     */
/*                                                                    */
/* Additional contributions by:                                       */
/*   Romain Dolbeau -- romain @ dolbeau.org                           */
/*                                                                    */
/* Turned into a skimmed down library implementation for the          */
/* Adept Project by Nick Johnson, EPCC -- Nick.Johnson@ed.ac.uk       */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "rapl_lib.h"


void open_msr(int core) {

  char msr_filename[BUFSIZ];

  sprintf(msr_filename, "/dev/cpu/%d/msr", core);
  fd = open(msr_filename, O_RDONLY);

}

uint64_t read_msr(int which) {

  uint64_t data;

  if ( pread(fd, &data, sizeof data, which) != sizeof data ) {
    perror("rdmsr:pread - you probably forgot to run as sudo or with CAP_SYS_RAWIO=ep set on your executable.");
    exit(127);
  }

  return data;
}



void rapl_init(void){

  uint64_t result = 0;

  open_msr(0);
  result=read_msr(MSR_RAPL_POWER_UNIT);
  power_units=pow(0.5,(double)(result&0xf));
  energy_units=pow(0.5,(double)((result>>8)&0x1f));
  time_units=pow(0.5,(double)((result>>16)&0xf));

}

void rapl_deinit(void){
  close(fd);
}


/* void rapl_start(void){ */

/*   /\* Calculate the units used *\/ */
/*   /\* uint64_t result=read_msr(fd,MSR_RAPL_POWER_UNIT); *\/ */

/*   printf("Power units = %.3lfW\n",power_units); */
/*   printf("Energy units = %.8lfJ\n",energy_units); */
/*   printf("Time units = %.8lfs\n",time_units); */
/*   printf("\n"); */


/*   uint64_t result_tmp = 0; */


/*   result_tmp=read_msr(MSR_PP0_ENERGY_STATUS); */
/*   while(result_tmp==(result_start=read_msr(MSR_PP0_ENERGY_STATUS))){ */
/*     usleep(20); */
/*   } */


/*   uint64_t i = 0; */
/*   while(result_start==(result_tmp=read_msr(MSR_PP0_ENERGY_STATUS))){ */
/*     i++; */
/*   } */

/*   uint64_t result_diff = result_tmp - result_start; */
/*   printf("%lld %lld %lld %lld %.6lf\n", result_tmp, result_start, i, result_diff, (double)(result_diff/(double)i)*energy_units    ); */

/* } */


void rapl_start(void){

  /* printf("Power units = %.3lfW\n",power_units); */
  /* printf("Energy units = %.8lfJ\n",energy_units); */
  /* printf("Time units = %.8lfs\n",time_units); */
  /* printf("\n"); */

  uint64_t result_tmp = 0;
  result_tmp=read_msr(MSR_PP0_ENERGY_STATUS);
  while(result_tmp==(result_start=read_msr(MSR_PP0_ENERGY_STATUS))){
    usleep(50);
  }

}

void rapl_end(void){

  uint64_t result_tmp = 0;
  double pp0 = 0;
  result_tmp=read_msr(MSR_PP0_ENERGY_STATUS);
  while(result_tmp==(result_end=read_msr(MSR_PP0_ENERGY_STATUS))){
    usleep(50);
  }
  pp0=(double)(result_end-result_start)*energy_units;
  printf("PowerPlane0 (core) for core %d energy expended: %.9f J\n",0,pp0);

}
