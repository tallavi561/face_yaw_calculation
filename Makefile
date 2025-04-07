# Makefile לקימפול yaw_calculations_avg.c לספרייה שיתופית yaw_calculations_avg.so

CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared

SRC = yaw_calculations_avg.c
TARGET = yaw_calculations_avg.so

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
