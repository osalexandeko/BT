//gcc -g -o rf rfcomm-server_mt.c -lbluetooth -lpthread
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


using namespace std;
#define NUM_OF_THREADS                      (10)
void * bt_comm(void *);
pthread_t _pt_arr[NUM_OF_THREADS];
pthread_mutex_t comm_mtx;
int main(){
	pthread_mutex_init(&comm_mtx , NULL);
	for(int i = 0; i < 1; i++){
		pthread_create(&_pt_arr[i], NULL, bt_comm, NULL );
		pthread_join(_pt_arr[i], NULL);
	}
	pthread_mutex_destroy(&comm_mtx);
	return 0;
}

int write_gpio_drv(char ch[1] )
{
	int fd;
	fd=open("/dev/etx_device",O_RDWR);

	write(fd,&ch[0],1);
	close(fd);
	return 0;
}


void * bt_comm(void * p)
{
	static char ch[1] = {'1'};
	
	
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
///////////////// keepalive start	
	/* Check the status for the keepalive option */
	if(getsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
	  perror("getsockopt()");
	  close(s);
	  exit(EXIT_FAILURE);
	}
	printf("SO_KEEPALIVE is %s\n", (optval ? "ON" : "OFF"));
	/* Set the option active */
	optval = 1;
	optlen = sizeof(optval);
	if(setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
		perror("setsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("SO_KEEPALIVE set on socket\n");
/////////////////////////////////// keepalive end ////////////////////////////////////////////////
//https://stackoverflow.com/questions/5875177/how-to-close-a-socket-left-open-by-a-killed-program/
	optval = 1;
	optlen = sizeof(optval);
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR , &optval, optlen) < 0) {
		perror("setsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
//////////////////////////////////////////////////////////////////////////////////////////////////
    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
	//loc_addr.rc_bdaddr = *BDADDR_ANY; 
	bdaddr_t bdaddr_any =  {{0, 0, 0, 0, 0, 0}};
	loc_addr.rc_bdaddr  =  bdaddr_any;
	loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    // put socket into listening mode
    listen(s, 1);
    // accept one connection
	client = accept(s, (struct sockaddr *)&rem_addr, &opt);
	ba2str( &rem_addr.rc_bdaddr, buf );
	fprintf(stderr, "accepted connection from %s \r\n", buf);
	int cnt = 10;
	//ofstream myfile;
	//myfile.open ("./example.jpg");
	do{
		memset(buf, 0, sizeof(buf));
		// read data from the client
		bytes_read = read(client, buf, sizeof(buf));
		if( bytes_read > 0 ) {
			printf("\r\n received %s ", buf);
		}else{//FIX
			/*close(client);
			close(s);
			printf("closeing the connection \r\n "); 
			pthread_mutex_unlock(&comm_mtx);
			return NULL;*/
			goto CLOSE_AND_EXIT;
		}
		
		//if (myfile.is_open()){
		//	myfile << buf;
		//}
		sprintf(resp_buf,"ok man %d \r\n", cnt);
		send(client, resp_buf, strlen(resp_buf),0);
		
		//test1
		if(ch[0] == '1'){
			ch[0] = '0';
		}else{
			ch[0] ='1';
		}
		write_gpio_drv(ch);
		
	}while(bytes_read);
	//myfile.close();
	CLOSE_AND_EXIT: 
    // close connection
	printf("closeing the connection. \r\n "); 
    close(client);
    close(s);
	pthread_mutex_unlock(&comm_mtx);
    return NULL;
}