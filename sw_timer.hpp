#ifndef SW_TIMER_HPP
#define SW_TIMER_HPP
//sw_timer.hpp

#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include "pwm.hpp"

using namespace std;

bool get_ms_passed(void);
void reset_ms_passed(void);
void update_duty_cycle(uint8_t dc);
int setup_sw_timer(void);
void delete_sw_timer(void);
#endif