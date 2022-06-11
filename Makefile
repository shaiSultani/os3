#
# To compile, type "make" or make "all"
# To remove files, type "make clean"
#
OBJS = server.o request.o segel.o list.o task.o threadPool.o
TARGET = server

CC = gcc
CFLAGS = -g -Wall

LIBS = -lpthread 

.SUFFIXES: .c .o 

server: server.o request.o segel.o list.o task.o threadPool.o
	$(CC) $(CFLAGS) -o server server.o request.o segel.o list.o task.o threadPool.o $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-rm -f $(OBJS) server