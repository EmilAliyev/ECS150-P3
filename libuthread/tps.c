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
    unsigned long int tid;
} tps;

typedef tps* tps_t;

/* Global variables */

static queue_t tpsqueue; //Store the tpses
int initialized = 0; //Whether the API has been initialized


/* Helper functions */

//Function to be passed to q_iterate
//returns 1 if tid passed in matches data tid, returns 0 otherwise
static int findTid(void *data, void *arg)
{
    tps_t tps = (tps_t) data;
    long unsigned int tid = tps->tid;
    long unsigned int match = *(long unsigned int *)arg;

    if (tid == match)
        return 1;

    return 0;
}

//Find any tps
static tps_t findTPS(unsigned long int tid)
{
    tps_t ptr = NULL;

    queue_iterate(tpsqueue, findTid, (void *) tid, (void **) &ptr);

    return ptr;
}

//Find the current tps
static tps_t findCurrentTPS()
{
    tps_t ptr = findTPS(pthread_self());

    return ptr; 
}

//Check if tps exists for tid
static int TPSFound(unsigned long int tid)
{
    tps_t ptr = findTPS(tid);

    if(ptr)
    {
        return 1;
    }
    
    else
    {
        return 0;
    }
}

//Check if tps exists for current thread
static int currentTPSFound()
{
    tps_t ptr = findCurrentTPS();

    if(ptr)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}


int tps_init(int segv)
{
    /* TODO: Phase 2 */

    //Make sure API has not already been initialized
    if(initialized)
    {
        return -1;
    }

    //initialize global queue
    tpsqueue = queue_create();

    initialized = 1;

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
    if(currentTPSFound())
    {
        return -1;
    }

    //malloc tps
    tps* new_tps = malloc(sizeof(struct tps));

    //Check to see if allocation failed
    if(new_tps == NULL)
    {
        return -1;
    }

    //set tid to current tid
    new_tps->tid = pthread_self();

    //nmap allocates mempage
    //private means that only this thread can access it
    //anonymous means no actual file by this name exists
    //Not sure if read/write protections are necessary 
    new_tps->memoryPage = mmap(NULL, TPS_SIZE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    //Check to see if allocation failed
    if(new_tps->memoryPage == MAP_FAILED)
    {
        return -1;
    }
    
    //enqueue tps
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

    //Make sure current thread has tps
    if(!currentTPSFound())
    {
        return -1;
    }

    //get tps pointer for current thread    
    tps_t tps = findCurrentTPS();

    //free pointer to mem
    munmap(tps->memoryPage, TPS_SIZE);
    

    //remove from queue
    int val = queue_delete(tpsqueue, tps);
    if (val != 0)
        return -1;

    //free tps

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

 
    //Error checking


    //Make sure current thread has tps
    if(!currentTPSFound())
    {
        return -1;
    }


    //Make sure read operation is in bounds of tps
    if((offset + length) > TPS_SIZE)
    {
        return -1;
    }

    //Make sure buffer isn't null
    if(buffer == NULL)
    {
        return -1;
    }


    //get tps pointer for current thread    
    tps_t tps = findCurrentTPS();

    //cast memory page to char pointer
    char* memorypagechptr = (char *) tps->memoryPage;

    //Copy starting from offset
    for(int i = 0; i < length; i++)
    {
        buffer[i] = memorypagechptr[i + offset];
    }

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

    //Make sure current thread has tps
    if(!currentTPSFound())
    {
        return -1;
    }

    //Make sure read operation is in bounds of tps
    if((offset + length) > TPS_SIZE)
    {
        return -1;
    }

    //Find tps for current thread
    tps_t tps = findCurrentTPS();

    //Cast memory page into char ptr
    char *mempagechptr = (char *) tps->memoryPage;

    //Copy starting from offset
    for(int i = 0; i < length; i++)
    {
        mempagechptr[i + offset] = buffer[i];
    }

    return 0;
}

int tps_clone(pthread_t tid)
{
    /* TODO: Phase 2 */

    /*
    Phase 2.1

    1. Check for errors:

    2. Find tps for thread tid

    3. Create tps for current thread

    4. Use memcpy to copy tps_tid's memory page to tps_current
    */

    //Check for errors

    //tps not found for tid
    if(!TPSFound(tid))
    {
        return -1;
    }

    //tps found for current thread
    if(currentTPSFound())
    {
        return -1;
    }

    //Find tps for tid
    tps_t tpssrc = findTPS(tid);

    //Create tps for current thread
    tps_create();

    //Find it
    tps_t tpscurr = findCurrentTPS();

    memcpy(tpscurr->memoryPage, tpssrc->memoryPage, TPS_SIZE);

    return 0;
}


