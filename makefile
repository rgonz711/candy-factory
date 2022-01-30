#CS 570

#Jaylan Pierce
#RedID: 822644713
#cssc3652
#Assignment 4 - Part 2
#Professor Ben Shen
#April 16, 2021

CC = g++ -std=c++11
CFLAGS = -Wall -g
LDFLAGS= -lpthread -lrt
OBJFILES = main.o consumer.o producer.o belt.o io.o
TARGET= mizzo

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean: 
	rm *.o $(TARGET)