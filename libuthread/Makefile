# Target library
lib := libuthread.a
objs := sem.o tps.o
givenobjs := queue.o thread.o

CC := gcc
CCFLAGS := -Wall -Werror

ifneq ($(D), 1)
else
CCFLAGS += -g
endif

all: $(lib)

sem.o : sem.c
	$(CC) $(CCFLAGS) -c -o $@ $<

tps.o : tps.c
	$(CC) $(CCFLAGS) -c -o $@ $<

libuthread.a : $(objs)
	ar rcs $@ $^ $(givenobjs)

clean:
	rm -rf $(lib) $(objs)
