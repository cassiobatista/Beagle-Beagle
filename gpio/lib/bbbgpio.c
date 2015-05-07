/*
 * gpio.c
 *
 * Author: Gaburiero
 * 
 * based on Derek Malloy's code
 *
 * Basic functions fo export use and unexport gpio via linux device tree
 * avaible from kernel 3.8 and on
 *
 *
 * 05/2015
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include "bbbgpio.h"


/*
 *TODO implementar gpioGetValue, gpioSetEdge e corrigir erros
 *
*/

/*
 * gpioExport function
 * makegpio avaible for use
*/

void gpioExport(unsigned int gpio){
    int fd, len;
    char buf[MAX];

    fd = open(GPIO_LOCATION"/export", O_WRONLY);
    if(fd<0){
        perror("gpio/export");
        exit(1);
    }
    
    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);

}

/*
 * gpioUnexport function
 * disable gpio
*/

void gpioUnexport(unsigned int gpio){
    int fd, len;
    char buf[255];

    fd = open(GPIO_LOCATION"/unexport", O_WRONLY);
    if(fd<0){
        perror("gpio/unexport");
        exit(1);
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
}

/*
 * gpioSetDirection
 * set gpio direction IN or OUT
*/

void gpioSetDirection(unsigned int gpio, PIN_DIR flag){
    int fd;
    char buf[MAX];

    snprintf(buf, sizeof(buf), GPIO_LOCATION "gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);
    if(fd<0){
        perror("gpio/direction");
        exit(1);
    }
    if(flag == OUTPUT){
        write(fd, "out", 4);
    }else{
        write(fd, "in", 3);
    }

    close(fd);
}

/*
 *gpioSetValue
 *set gpio pin value
*/

void gpioSetValue(unsigned int gpio, PIN_VALUE value){
    int fd;
    char buf[MAX];

    snprintf(buf, sizeof(buf), GPIO_LOCATION "/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);
    if(fd<0){
        perror("gpio/set-value");
        exit(1);
    }

    if(value==LOW){
        write(fd, "0", 2);
    }else{
        write(fd, "1", 2);
    }
    close(fd);
}

