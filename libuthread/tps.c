#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "queue.h"
#include "thread.h"
#include "tps.h"

/*
TPS struct
//Fields:
//1. Void pointer to memory page
//2. tid
*/

typedef struct tps
{
	void *memoryPage;
	unsigned short tid;
} tps;

typedef tps* tps_t;

/* Global variables */

static queue_t tpsqueue; //Store the tpses


/* Helper functions */

//Function to be passed to q_iterate
//returns 1 if tid passed in matches data tid, returns 0 otherwise
static int find_tid(void *data, void *arg)
{
	struct tps* tps = (struct tps *)data;
	long unsigned int tid = tps->tid;
	long unsigned int match = *(long unsigned int *)arg;

	if (tid == match)
		return 1;

	return 0;
}

//Find the current tps
static tps_t findCurrentTPS()
{
	unsigned long int tid = pthread_self();

	struct tps* ptr = NULL;

	queue_iterate(tpsqueue, find_tid, (void *)tid, (void**)&ptr);

	return ptr;	
}


int tps_init(int segv)
{
	/* TODO: Phase 2 */
	/*
	Phase 2.1: Do nothing
	*/
	tpsqueue = queue_create();

	return 0;
}

int tps_create(void)
{
	/* TODO: Phase 2 */

	/*
	Phase 2.1

	1. Malloc new tps
	2. Use mmap to allocate the memory page
	3. Set tid of tps to current tid
	4. Add tps to queue
	*/
	
	//need to check if tps is already allocated

	struct tps* new_tps = malloc(sizeof(struct tps));

	new_tps->tid = pthread_self();
	//new_tps->mem = mmap(args);
	
	queue_enqueue(tpsqueue, new_tps);

	return 0;
}

int tps_destroy(void)
{
	/* TODO: Phase 2 */

	/*
	Phase 2.1

	1. Find tps for current thread
	2. Deallocate memory page
	3. Deallocate tps
	*/
	
	struct tps* tps = findCurrentTPS();

	int val = queue_delete(tpsqueue, tps);
	if (val != 0)
		return -1;

	return 0;
}

int tps_read(size_t offset, size_t length, char *buffer)
{
	/* TODO: Phase 2 */

	/*
	Phase 2.1

	1. Find tps for current thread.

	2. Check for errors

	3. 
	*/
	return 0;
}

int tps_write(size_t offset, size_t length, char *buffer)
{
	/* TODO: Phase 2 */
	return 0;
}

int tps_clone(pthread_t tid)
{
	/* TODO: Phase 2 */

	/*
	Phase 2.1

	1. Find tps for thread tid

	2. Find tps for current thread

	3. Use memcpy to copy tps_tid's memory page to tps_current
	*/
	return 0;
}


