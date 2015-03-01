#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_BUF 127

int pwm_run(int flag);
int pwm_duty(unsigned int ns);
int pwm_period(unsigned int ns);

/*
 N A D A
*/
int pwm_run(int flag) {
	FILE *fp;
	char path[MAX_BUF];
	
	//snprintf(path, sizeof path, "/sys/class/pwm/%s/run", pwm);
	snprintf(path, sizeof path, "/sys/devices/ocp.3/bs_pwm_test_P8_13.12/run");
	
	if((fp = fopen(path, "w")) == NULL){
		printf("Cannot open pwm run file, %s\n", path);
		return 1;
	}

	//rewind(fp);
	fprintf(fp, "%d\n", flag);
	//fflush(fp);
	fclose(fp);
}

int pwm_duty(unsigned int ns) {
	FILE *fp;
	char path[MAX_BUF];
	
	snprintf(path, sizeof path, "/sys/devices/ocp.3/bs_pwm_test_P8_13.12/duty");

	if((fp = fopen(path, "w")) == NULL){
		printf("Cannot open pwm duty_ns file, %s\n", path);
		return 1;
	}

	//rewind(fp);
	fprintf(fp, "%d\n", ns);
	//fflush(fp);
	fclose(fp);
}

int pwm_period(unsigned int ns) {
	FILE *fp;
	char path[MAX_BUF];
	
	snprintf(path, sizeof path, "/sys/devices/ocp.3/bs_pwm_test_P8_13.12/period");

	if((fp = fopen(path, "w")) == NULL){
		printf("Cannot open pwm period_freq file, %s\n", path);
		return 1;
	}

	//rewind(fp);
	fprintf(fp, "%d\n", ns);
	//fflush(fp);
	fclose(fp);
}
