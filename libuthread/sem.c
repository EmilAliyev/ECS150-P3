#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "thread.h"

struct semaphore {
	int count;		//# of resources
	queue_t waitlist;	//Threads waiting for resources
};

sem_t sem_create(size_t count)
{
	struct semaphore* new_sem = malloc(sizeof(struct semaphore));

	//initialize count to given value and queue to be an empty queue
	new_sem->count = count;
	new_sem->waitlist = queue_create();

	return new_sem;
}

int sem_destroy(sem_t sem)
{
	//sanity check
	if(sem == NULL){
		return -1;
	}

	//queue empty check is made here for us
	int response = queue_destroy(sem->waitlist);

	//check if queue was deleted
	if(response != 0){
		return -1;
	}

	//free semaphore
	free(sem);

	return 0;
}

int sem_down(sem_t sem)
{
	//sanity check
	if(sem == NULL){
		return -1;
	}

	//maybe make this a crit section? so that check isn't interrupted after it's made?

	//take from resource pile if resources are availible 
	if(sem->count != 0){
		sem->count -= 1;
	}
	else{
		//pthread_self https://piazza.com/class/jqmqktp98ox44m?cid=302
		
		long unsigned int tid = pthread_self();

		queue_enqueue(sem->waitlist, &tid);
		thread_block();
	}

	return 0;
}

int sem_up(sem_t sem)
{
	//sanity check
	if(sem == NULL){
		return -1;
	}

	//lets make this a crit section also

	//add resource back to pile
	sem->count += 1;
	if(queue_length(sem->waitlist) != 0){
	}

	return 0;
}

int sem_getvalue(sem_t sem, int *sval)
{
	return 0;
}

