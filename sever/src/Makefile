SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=thread_pool_server
CC:=gcc
CFLAGS:=-g -Wall -I /usr/include/mysql/ 
$(ELF):$(OBJS)
	gcc -o $@ $^ -pthread  -lcrypt -lmysqlclient
clean:
	rm -rf $(OBJS) $(ELF)
