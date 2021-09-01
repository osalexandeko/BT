#include "pwm.hpp"

/*******************************************************************************
* Sets up PWM.
*https://abyz.me.uk/rpi/pigpio/cif.html
*@param user_gpio - pin.
*@param freq - frequency.
*@param duty_cycle - the duty cycle.
*******************************************************************************/
int pwm_set(uint32_t user_gpio, uint32_t freq, uint32_t duty_cycle){
	gpioTerminate();
	if (gpioCfgClock(5, 1, 0)) return -1;
	if (gpioInitialise() < 0) return -1;
	int ret = gpioHardwarePWM(user_gpio, freq, duty_cycle);
	if (ret != 0){
	   perror("PWM failed \n");
	   return -1;
	}
	return 0;
}

/*******************************************************************************
* Initiates PWM.
*******************************************************************************/
void init_pwm(void){
	 pwm_set(19, 1000, 0);
}

