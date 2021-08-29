#include "sw_timer.hpp"
//https://katastrophos.net/symbian-dev/GUID-B4039418-6499-555D-AC24-9B49161299F2.html
 
static bool ms_passed = false;

bool get_ms_passed(void){
	return ms_passed;
}

void reset_ms_passed(void){
	ms_passed = false;
}

//Thread function to be invoked when the periodic timer expires
void sig_hndler (union sigval val)
{
	  ms_passed = true;
}


int sw_timer(void){
    int Ret;

    pthread_attr_t attr;
    pthread_attr_init( &attr );

    struct sched_param parm;
    parm.sched_priority = 255;
    pthread_attr_setschedparam(&attr, &parm);

    struct sigevent sig;
    sig.sigev_notify = SIGEV_THREAD;
    sig.sigev_notify_function = sig_hndler;
    sig.sigev_value.sival_int =20;
    sig.sigev_notify_attributes = &attr;

    //create a new timer.
    timer_t timerid;
    Ret = timer_create(CLOCK_REALTIME, &sig, &timerid);
    if (Ret == 0)
    {
        struct itimerspec in, out;
        in.it_value.tv_sec = 2;
        in.it_value.tv_nsec = 0;
        in.it_interval.tv_sec = 0;
        in.it_interval.tv_nsec = 1000000;//1 ms
        //issue the periodic timer request here.
        Ret = timer_settime(timerid, 0, &in, &out);
        if(Ret != 0){
			printf("timer_settime() failed with %d\n", errno);
		}
    }
    else{
		printf("timer_create() failed with %d\n", errno);
	}
    return Ret;
}