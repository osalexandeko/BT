#ifndef PWM_HPP
#define PWM_HPP
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pigpio.h>
#include <stdint.h>
using namespace std;

#define NUM_GPIO 32
#define GPIO_INUSE 21
#define MIN_WIDTH 1000
#define MAX_WIDTH 2000

int pwm_set(uint32_t user_gpio, uint32_t freq, uint32_t duty_cycle);
#endif