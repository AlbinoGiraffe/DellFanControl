#ifndef FANCONTROL_H
#define FANCONTROL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

// enable network checking
#define NETSTAT true
#define DEFAULT_SPEED 15
#define TEMP_LIMIT 70
/**
 * Return current CPU temperatures
 * 
 * On a dual CPU system, averages the two processor temperatures
 * 
 * @param fp Points to a file stream of the output command
 * @return Average system temperature in degrees Celcius
 */
int get_temp(FILE *fp);

/**
 * Set fan speed
 * 
 * Converts % to hex
 * 
 * @param speed desired speed in % (0-100)
 */
void set_speed(int speed);

#endif