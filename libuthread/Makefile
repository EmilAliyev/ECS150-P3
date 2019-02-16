# Target library
lib := libuthread.a

all: $(lib)

sem.o : sem.c
	gcc -Wall -Werror -c sem.c -o sem.o

tps.o : tps.c
	gcc -Wall -Werror -c tps.c -o tps.o

libuthread.a : sem.o tps.o
	ar rcs libuthread.a sem.o queue.o thread.o tps.o 
