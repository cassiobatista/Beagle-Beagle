#ifndef SIMPLE_PWM_H
#define SIMPLE_PWM_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_BUF 127

int simple_pwm_run(int flag);
int simple_pwm_duty(unsigned int ns);
int simple_pwm_period(unsigned int ns);

#endif
