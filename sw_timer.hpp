#ifndef SW_TIMER_HPP
#define SW_TIMER_HPP
//sw_timer.hpp

#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

using namespace std;
int sw_timer(void);

bool get_ms_passed(void);

void reset_ms_passed(void);

#endif