#
# File Name: makefile
# Author: Seree Rakwong
# Date: 12-NOV-2021
#

TARGET = c_unittest
CC     = gcc
CFLAGS = -g -Wall
LFLAGS = -L. -L./lib -L../lib -lm -lrt -lpthread -lsqlite3

.PHONY: default all clean

default: $(TARGET)
all: default

BIN_DIR  = ./bin
SRC_DIR  = ./src
INC_DIR  = ./include
SOURCES  = $(SRC_DIR)/cuteng.c \
           $(SRC_DIR)/test_safedb.c \
           $(SRC_DIR)/c_unittest.c


OBJECTS  = $(SOURCES:.c=.o)
INCLUDES = -I$(INC_DIR) 
DEFINES  = -D__UNIX__ -D__UNIT_TEST__ -D_POSIX_SOURCE -D__LINUX__


%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -f src/*.o $(TARGET)

