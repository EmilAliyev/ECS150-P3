# Target library
lib := libuthread.a

all: $(lib)

sem.o : sem.c
	gcc -Wall -Werror -c sem.c -o sem.o

libuthread.a : sem.o
	ar rcs libuthread.a sem.o queue.o thread.o
