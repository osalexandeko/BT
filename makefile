CC=g++
CFLAGS=-g -Wall   
LFLAGS=-lpthread -lbluetooth -lpigpio -lrt
TARGET = rf_bt

all: $(TARGET)
$(TARGET): rfcomm-server_mt.o logic.o pwm.o sw_timer.o waiting_queue.o
	$(CC) $(LFLAGS) rfcomm-server_mt.o logic.o pwm.o sw_timer.o waiting_queue.o -o $(TARGET)
logic.o: logic.cpp logic.hpp
	$(CC) logic.cpp   $(CFLAGS) -c  	
waiting_queue.o: waiting_queue.cpp waiting_queue.hpp
	$(CC) waiting_queue.cpp   $(CFLAGS) -c  	
rfcomm-server_mt.o: rfcomm-server_mt.cpp
	$(CC) rfcomm-server_mt.cpp $(CFLAGS) -c
pwm.o: pwm.cpp pwm.hpp
	$(CC) pwm.cpp $(CFLAGS) -c 
sw_timer.o: sw_timer.cpp sw_timer.hpp 
	$(CC) sw_timer.cpp $(CFLAGS) -c 
	
clean:
	rm -f ./*.o  $(TARGET)
