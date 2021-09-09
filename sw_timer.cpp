#include "sw_timer.hpp"
//https://katastrophos.net/symbian-dev/GUID-B4039418-6499-555D-AC24-9B49161299F2.html

static timer_t timerid;
extern pthread_t pwm_thread;
sem_t pwm_step_sem;

static uint8_t last_duty_cycle = 0; //The last duty cycle.
static uint8_t new_duty_cycle = 0;  //The new duty cycle.

static void * pwm_step(void * param);

/*******************************************************************************
* Updates duty cycle. 
*@param dc - duty cycle.
*******************************************************************************/
void update_duty_cycle(uint8_t dc){
	setup_sw_timer();
	new_duty_cycle = dc;
	sem_post(&pwm_step_sem);
}

/*******************************************************************************
* Deletes SW timer.
*******************************************************************************/
void delete_sw_timer(void){
    //delete the timer.
	timer_delete(timerid);
	//pthread_cancel(pwm_thread);
}

/*******************************************************************************
* Thread function to be invoked when the periodic timer expires.
*******************************************************************************/
void sig_hndler (union sigval val)
{
	sem_post(&pwm_step_sem);
}

/*******************************************************************************
* Sets up SW timer.
*******************************************************************************/
int setup_sw_timer(void){
	pthread_create(&pwm_thread, NULL, pwm_step, NULL );
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

/*******************************************************************************
* A step of pwm apdate.
* 
*@param param - a parameter of the task.
*******************************************************************************/
void * pwm_step(void * param){
	sem_init(&pwm_step_sem, 0, 0);
	uint32_t div =0;
	while(1){
		sem_wait(&pwm_step_sem);
		//cout << "pwm_step_sem" << endl;
		if(20 == div++){
			div =0;
			if(new_duty_cycle > last_duty_cycle){
				last_duty_cycle++;
				gpioPWM(19, last_duty_cycle);
				printf("%d \n",last_duty_cycle);
			}
			else
			if(new_duty_cycle < last_duty_cycle){
				last_duty_cycle--;
				gpioPWM(19, last_duty_cycle);
				printf("%d \n",last_duty_cycle);
			}else{
				delete_sw_timer();
				printf("\n pwm_step: pthread_exit\n");
				pthread_exit(NULL);//BF_20210909_JOIN_PWM_THREAD
			}
		}
	}
}