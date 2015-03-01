#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include "bbbgpio.h"

#define DELAY 15 

void main(){
	
	int i;
	double time, diff, micro;
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
		sleep(DELAY);

		//gpioSetValue(,HIGH);                                            
                //gpioSetValue(,HIGH);                                            
                //gpioSetValue(,HIGH);                                            
                //sleep(DELAY);
	//}
	gpioSetValue(66,LOW);
	gpioSetValue(67,LOW);
	gpioSetValue(69,LOW);

	gpioUnexport(66);
	gpioUnexport(67);
	gpioUnexport(69);
	
	c1=clock();
	
	diff = (double)(c1-c0);
	
	time =(diff/1000)/CLOCKS_PER_SEC;
	micro = time/1000;
	
	printf ("CLOCKS PER SEC : %d", CLOCKS_PER_SEC);
	printf("pulsos de clock : %lf\n", diff);
	printf("tempo de execucao : %lf segundos\n", (time*1000));
	printf("tempo de execucao : %lf millisegundos\n", time);
	printf("tempo de execucao : %lf microsegundos\n", micro);





}
