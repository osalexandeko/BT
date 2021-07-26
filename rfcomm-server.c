//gcc -o rf rfcomm-server.c -lbluetooth
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
int main(int argc, char **argv)
{
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

//////////////////////////////// keepalive end

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
	client = accept(s, (struct sockaddr *)&rem_addr, &opt);
	ba2str( &rem_addr.rc_bdaddr, buf );
	fprintf(stderr, "accepted connection from %s \r\n", buf);
	int cnt = 10;
	
	
	
	while(cnt--){
		memset(buf, 0, sizeof(buf));
		// read data from the client
		bytes_read = read(client, buf, sizeof(buf));
		if( bytes_read > 0 ) {
			printf("\r\n received %s ", buf);
		}else{//FIX
			close(client);
			close(s);
			printf("closeing the connection \r\n "); 
			return 0;
		}
		sprintf(resp_buf,"ok man %d \r\n", cnt);
		
		send(client, resp_buf, strlen(resp_buf),0);
		
	}

    // close connection
    close(client);
    close(s);
    return 0;
}