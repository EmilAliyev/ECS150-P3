# Target library
lib := libuthread.a
objs := sem.o tps.o

all: $(lib)

sem.o : sem.c
	gcc -Wall -Werror -c sem.c -o sem.o

tps.o : tps.c
	gcc -Wall -Werror -c tps.c -o tps.o

libuthread.a : $(objs)
	ar rcs libuthread.a sem.o queue.o thread.o tps.o

clean:
	rm -rf libuthread.a sem.o queue.o thread.o tps.o 
