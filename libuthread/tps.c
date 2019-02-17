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

/* TODO: Phase 2 */

/* Helper functions */

//Find the current tps
static tps_t findCurrentTPS()
{
	//Dummy line so program compiles
	queue_enqueue(tpsqueue, NULL);

	//Dummy return to compile
	return NULL;
}

int tps_init(int segv)
{
	/* TODO: Phase 2 */
	/*
	Phase 2.1: Do nothing
	*/

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
	return 0;
}

int tps_read(size_t offset, size_t length, char *buffer)
{
	/* TODO: Phase 2 */

	/*
	Phase 2.1

	1. Find tps for current thread.

	2. Check for errors

	3. Cast memory page of tps to char pointer memorypagechptr

	4. Starting at offset, and going on for length characters,
	copy characters from memorypagechptr to buffer.
	*/
	return 0;
}

int tps_write(size_t offset, size_t length, char *buffer)
{
	/* TODO: Phase 2 */

	/*
	Phase 2.1

	1. Find tps for current thread

	2. Check for errors

	3. Cast memory page of tps to char pointer memorypagechptr

	4. Starting at offset of memorypagechptr, and going on for length
	chaaracters, copy characters buffer to memorypagechptr
	*/

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

