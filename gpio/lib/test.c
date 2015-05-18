#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include "bbbgpio.h"

#define DELAY 3 

void main(){
	
	int i;
	double time;
	//measuring time
	clock_t c0,c1;
	
	c0=clock();
	/* setting wich pins to use
	* header 8 pinos 7,8,9
	*/
	gpioExport(66);
	gpioExport(67);
	gpioExport(69);
	
	gpioSetDirection(66, OUTPUT);
	gpioSetDirection(67, OUTPUT);
	gpioSetDirection(69, OUTPUT);
	
	//for(){
		gpioSetValue(66, HIGH);
		gpioSetValue(67, HIGH);
		gpioSetValue(69, HIGH);
		//sleep(DELAY);

		//gpioSetValue(,HIGH);                                            
                //gpioSetValue(,HIGH);                                            
                //gpioSetValue(,HIGH);                                            
                //sleep(DELAY);
	//}

	c1=clock();

	time =(double)((c1-c0)/CLOCKS_PER_SEC);

	printf("tempo de execucao em segundos : %lf segundos", time);


}
