#include "waiting_queue.hpp"
#include <string>

/*******************************************************************************
* Waiting_queue. 
*******************************************************************************/
wqueue <string> wq;

/*******************************************************************************
* Getter fo wq. 
*******************************************************************************/
wqueue<string> * waiting_queue_get_wq(void){
	return &wq;
}