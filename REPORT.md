#ECS 150 Project 3 Report
Noah White

Emil Aliyev

##Introduction

Our report will be split into two sections, detailing the two phases of the
project. The first section will examine the design decisions made in
implementing our semaphore library. The second secion will examine the
implementation of our thread private storage (TPS) library.

##Semaphores

###Semaphore Implementation

The data structure used to represent our semaphore is nothing more that an
integer value representing the number of resources availible and a queue
representing blocked threads waiting for the resource. More specifically, the
queue held pointers to the thread ids returned by pthread_self(). When threads
need a resource, but it is unavailable, they are enqueued. When a resource is
freed, if the queue has any threads waiting, they are dequeued and unblocked. 

###Quirks in Semaphore Implementation

sem_up, sem_down and sem_getvalue all have areas marked as critical sections. In
the case of sem_up, this is to prevent another thread from unblocking the thread
that the current thread is trying to unblock. In the case of sem_down, this is
to prevent another thread from taking the resource that this thread has taken.
In the case of sem_getvalue, this is used to prevent the returned value from
changing before the function completes (though if the user is using this value
for a check, they should already be calling the function from inside a critical
area).

##Thread Private Storage (TPS)

###TPS Implmentation

We have a data structure representing an indivdual TPS block. That TPS block
contains a memory page and a thread id associated with the TPS block. We use a
global queue to store pointers to all of our TPS blocks. tps_init creates this
queue.

For phase 2.3, we changed the TPS structure slightly. The TPS structure now 
contains a tid, and a page struct. The page struct contains a pointer to the 
memory page allocated with mmap and a count to keep track of how many TPS 
structs are using the page. 

###TPS Helper Functions

We have several helper functions used in our tps library, to handle accessing
the TPS queue. findTID is passed to queue_iterate, with the intention of finding
the TPS block with a matching tid. findTPS takes a TID, calls queue_iterate with 
findTID and the TID as an arguement, and then returns a pointer to the TPS
block. findCurrentTPS just calles findTPS with the current thread tid as the
arguement. TPSfound and CurrentTPSfound both just check if a thread has a TPS
associated with it or not. We also used helper functions when accessing the 
memory page of the tps. For instance, we used the helper function 
changeProtection() to change the protections of the page rather than calling 
mprotect() directly. We also used the getPage() function to return the memory
page of the tps, rather than attempting to access the memory page directly 
inside other functions. By abstracting access to the memory in such a manner, 
we were able to accomodate the changes to the data structure in phase 2.3 by 
changing only a few lines of code. 

###TPS Testing

We provided extensive testing for our TPS library in the file tps_test.c. In
this file, our tests are broken into 2 categories. The first is a basic battery
of tests, which ensures that a TPS is created, that multiple creates in a single
thread fail, that basic read and write operations work, that illegal reads
and writes fail, and that destroy is working as intended. Once the basic tests
have been passed, we then run the more comprehensive "real-world" style tests.

Three threads are created. Thread 3 makes a TPS, Thread 2 clones from thread 3
and Thread 1 clones from Thread 2. Then Thread 2 writes to the TPS. Checks are
made to Thread 1 and Thread 3 to ensure they are unchanged. Then Thread 3 writes
something else to the TPS. Checks are made to Thread 1 to ensure it remains
unchanged. 
