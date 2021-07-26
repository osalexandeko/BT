CC=g++
CFLAGS=-g -Wall   
LFLAGS=-lpthread -lbluetooth
TARGET = rf

all: $(TARGET)
$(TARGET): rfcomm-server_mt.o
	$(CC) $(LFLAGS) rfcomm-server_mt.o -o $(TARGET)
rfcomm-server_mt.o: rfcomm-server_mt.c
	$(CC) rfcomm-server_mt.c $(CFLAGS) -c  
clean:
	rm -f ./*.o  $(TARGET)