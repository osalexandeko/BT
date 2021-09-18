#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "logic.hpp"
#include "sw_timer.hpp"
#include "pwm.hpp"
#include "waiting_queue.hpp"
using namespace std;


#define NUM_OF_THREADS                         (1)
void * bt_comm(void *);

pthread_t pt_arr[NUM_OF_THREADS];
pthread_t logic_thread;
pthread_t pwm_thread;

pthread_mutex_t comm_mtx;
void write_to_file(string str);

/*******************************************************************************
*main function
*******************************************************************************/
int main(){
	setup_sw_timer();
	init_pwm();
	pthread_mutex_init(&comm_mtx , NULL);
	
	pthread_create(&logic_thread, NULL, logic_task, NULL );
	
	for(int i = 0; i < NUM_OF_THREADS; i++){
		pthread_create(&pt_arr[i], NULL, bt_comm, NULL );
		pthread_join(pt_arr[i], NULL);
	}
	pthread_mutex_destroy(&comm_mtx);
	return 0;
}

/*******************************************************************************
*writes to file.
*******************************************************************************/
void write_to_file(string str, string filename){
  ofstream fstrm;
  fstrm.open (filename, ios::out | ios::app | ios::binary);
  fstrm << str;
  fstrm.close();
}

/*******************************************************************************
* BT comunication task.
*******************************************************************************/
void * bt_comm(void * p)
{
	wqueue <string> * wqp = waiting_queue_get_wq();
	
	pthread_mutex_lock(&comm_mtx);
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
	char resp_buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
	int optval;
    socklen_t optlen = sizeof(optval);
    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	optval = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR , &optval, optlen) < 0) {
		perror("setsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
	 
	bdaddr_t bdaddr_any =  {{0, 0, 0, 0, 0, 0}};
	loc_addr.rc_bdaddr  =  bdaddr_any;
	loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    // put socket into listening mode
    listen(s, 1);
	fprintf(stderr, "waiting for connection...\r\n");
	
	
    // accept one connection
	client = accept(s, (struct sockaddr *)&rem_addr, &opt);
	ba2str( &rem_addr.rc_bdaddr, buf );
	fprintf(stderr, "accepted connection from %s \r\n", buf);
	do{
		memset(buf, 0, sizeof(buf));
		// read data from the client
		bytes_read = read(client, buf, sizeof(buf));
		if( bytes_read > 0 ) {
			printf("\r\n received %s ", buf);
			//logic_parser(buf);
			
			wqp->add(buf);
			 
			
			write_to_file(buf, "cmd.log");
		}else{ 
			printf("\r\n goto CLOSE_AND_EXIT. \r\n");
			goto CLOSE_AND_EXIT;
		}
		
		sprintf(resp_buf,"ok man \r\n");
		send(client, resp_buf, strlen(resp_buf),0);
			
	}while(bytes_read);
	CLOSE_AND_EXIT: 
    // close connection
	printf("closeing the connection, buy. \r\n "); 
    close(client);
	close(s);
	pthread_mutex_unlock(&comm_mtx);
	//close msg parsing
	pthread_cancel(logic_thread); 
	
	//close pwm task
	pthread_cancel(pwm_thread); 
	
    return NULL;
}
