CC=g++
CFLAGS=-g -Wall   
LFLAGS=-lpthread -lbluetooth -lpigpio -lrt
TARGET = rf

all: $(TARGET)
$(TARGET): rfcomm-server_mt.o logic.o pwm.o sw_timer.o
	$(CC) $(LFLAGS) rfcomm-server_mt.o logic.o pwm.o sw_timer.o -o $(TARGET)
rfcomm-server_mt.o: rfcomm-server_mt.cpp
	$(CC) rfcomm-server_mt.cpp $(CFLAGS) -c
logic.o: logic.cpp logic.hpp
	$(CC) logic.cpp   $(CFLAGS) -c  
pwm.o: pwm.cpp pwm.hpp
	$(CC) pwm.cpp $(CFLAGS) -c 
sw_timer.o: sw_timer.cpp sw_timer.hpp 
	$(CC) sw_timer.cpp $(CFLAGS) -c 
clean:
	rm -f ./*.o  $(TARGET)