# Target library
lib := libuthread.a
objs := sem.o tps.o
givenobjs := queue.o thread.o

all: $(lib)

sem.o : sem.c
	gcc -Wall -Werror -c sem.c -o sem.o

tps.o : tps.c
	gcc -Wall -Werror -c tps.c -o tps.o

libuthread.a : $(objs)
	ar rcs $(lib) $(objs) $(givenobjs)

clean:
	rm -rf $(lib) $(objs)
