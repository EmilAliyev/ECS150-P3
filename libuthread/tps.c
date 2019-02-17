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

/* TODO: Phase 2 */

//Helper functions

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
	return 0;
}

