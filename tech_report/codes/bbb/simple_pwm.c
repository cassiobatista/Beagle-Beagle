int simple_pwm_run(int flag) 
{
	FILE *fp;
	char path[MAX_BUF];
	
	snprintf(path, sizeof path, "/sys/devices/ocp.3/bs_pwm_test_P8_13.12/run");
	
	if((fp = fopen(path, "w")) == NULL) {
		printf("Cannot open pwm run file, %s\n", path);
		return 1;
	}

	fprintf(fp, "%d\n", flag);
	fclose(fp);
}

int simple_pwm_duty(unsigned int ns) 
{
	FILE *fp;
	char path[MAX_BUF];
	
	snprintf(path, sizeof path, "/sys/devices/ocp.3/bs_pwm_test_P8_13.12/duty");

	if((fp = fopen(path, "w")) == NULL){
		printf("Cannot open pwm duty_ns file, %s\n", path);
		return 1;
	}

	fprintf(fp, "%d\n", ns);
	fclose(fp);
}

int simple_pwm_period(unsigned int ns) 
{
	FILE *fp;
	char path[MAX_BUF];
	
	snprintf(path, sizeof path, "/sys/devices/ocp.3/bs_pwm_test_P8_13.12/period");

	if((fp = fopen(path, "w")) == NULL){
		printf("Cannot open pwm period_freq file, %s\n", path);
		return 1;
	}

	fprintf(fp, "%d\n", ns);
	fclose(fp);
}
