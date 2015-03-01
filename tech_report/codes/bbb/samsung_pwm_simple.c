#include "nada.h"
#include <signal.h>

#define CARRIER_FREQ 37900
#define PERIOD 26385

void mark(int us);
void space(int us);

int main() 
{
	int i,j;
	//char *stream = "1111000001110000011100000000111110";
	char *stream = "1111000001110000001000000101111110";

	pwm_run(0); //force PWM to be turned off before start
	pwm_duty(0); //duty cannot be greater than period. So start with '0' for safety issues

	pwm_period(period);

	pwm_run(1); //Turn PWM on

	for(j=0; j<1; j++) {
		mark(4500);
		space(4500);
		for(i=1; i<34; i++) {
			mark(560);
			if(stream[i]=='1') {
				space(1690);
			} else {
				space(560);
			}
		}
		usleep(200000); //200ms, since signal lasts 108ms
	}

	pwm_run(0);

	return 0;
}

void mark(int us) {
	pwm_duty(PERIOD/3); //set duty period to 1/3 of the PWM period <-> 33%
	usleep(us-150);
}

void space(int us) {
	pwm_duty(PERIOD); //set duty period to 1/1 of the PWM period <-> 100% 
	usleep(us-150);
}
