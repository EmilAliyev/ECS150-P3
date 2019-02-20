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

/*
Phase 2.3 - Structs

Page:
-Void pointer to memory
-Count - number of tpses using this page

TPS:
-pointer to page
-tid
*/

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

//Get the page for tps
void *getPage(tps_t tps)
{
    return tps->memoryPage;
}

//Create a page for given tps
static void createPage(tps_t tps)
{
    //Allocate memory
    tps->memoryPage = mmap(NULL, TPS_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    /*
    Phase 2.3
    //Allocate new page

    //Count for page starts at 1

    //Allocate memory for page
    */

}


//Destroy page for tps
static int destroyPage(tps_t tps)
{
    //Get the page
    void *mempage = getPage(tps);

    //Deallocate memory
    munmap(mempage, TPS_SIZE);

        

    /*
    Phase 2.3
    //If count for page is greater than 1, do not destroy it and return -1

    //Otherwise:

    //Deallocate the memory with munmap

    //Free the page
    */
    
    return 0; 
}

//Change protection for tps memory page
static void changeProtection(tps_t tps, int prot)
{
    //Get the page
    void *mempage = getPage(tps);

    //Change protection
    mprotect(mempage, TPS_SIZE, prot);

    /*
    Phase 2.3

    Change protection for tps->page->memory
    */
}

//Copy memory page
static void copyPage(tps_t src, tps_t dest)
{
    //Copy the memory
    memcpy(dest->memoryPage, src->memoryPage, TPS_SIZE);

    /*
    Phase 2.3

    Copy from src->page->memory to dest->page->memory
    */
}

//Check to see if allocation failed for tps memory page
static int allocationFailed(tps_t tps)
{
    //Check to see if allocation failed
    if(tps->memoryPage == MAP_FAILED)
    {
        return 1;
    }   

    return 0;
}

//Read from page to buffer
static void readFromPage(tps_t tps, size_t offset, size_t length, char * buffer)
{
    //cast memory page to char pointer
    char* memorypagechptr = (char *) tps->memoryPage;

    //Copy starting from offset
    for(int i = 0; i < length; i++)
    {
        buffer[i] = memorypagechptr[i + offset];
    }
}

//Write to page from buffer
static void writeToPage(tps_t tps, size_t offset, size_t length, char *buffer)
{
    //Cast memory page into char ptr
    char *mempagechptr = (char *) tps->memoryPage;

    //Copy starting from offset
    for(int i = 0; i < length; i++)
    {
        mempagechptr[i + offset] = buffer[i];
    }
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

//find if a tps from another tps pointer
static int findTPSfromfault(void* data, void *fault)
{
   tps_t tps = (tps_t) data;
   void * memPage = tps->memoryPage; 
   
   if (memPage == fault)
       return 1;

   return 0;
}

//segv handler
static void segv_handler(int sig, siginfo_t *si, void *context)
{
    /*
     * Get the address corresponding to the beginning of the page where the
     * fault occurred
     */
    void *p_fault = (void*)((uintptr_t)si->si_addr & ~(TPS_SIZE - 1));

    tps_t ptr = NULL;
    
    queue_iterate(tpsqueue, findTPSfromfault, p_fault, (void **) &ptr);
   
    if (ptr != NULL)
        fprintf(stderr, "TPS protection error!\n");

    /* In any case, restore the default signal handlers */
    signal(SIGSEGV, SIG_DFL);
    signal(SIGBUS, SIG_DFL);
    /* And transmit the signal again in order to cause the program to crash */
    raise(sig);
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

    if (segv) {
        struct sigaction sa;

        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = segv_handler;
        sigaction(SIGBUS, &sa, NULL);
        sigaction(SIGSEGV, &sa, NULL);
    }

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

    /*
    Phase 2.2
    //When allocating new tps, give memory page no read/write permissions
    */

    /*
    Phase 2.3
    Instead of allocating memory with mmap, allocate a page using createPage()
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
    createPage(new_tps);

    //Check to see if allocation failed
    if(allocationFailed(new_tps))
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

    /*
    Phsae 2.3

    Instead of deallocating memory, use destroyPage() to destroy the page.
    If destroyPage() fails (returns -1), decrement the page's count.
    */

    //Make sure current thread has tps
    if(!currentTPSFound())
    {
        return -1;
    }

    //get tps pointer for current thread    
    tps_t tps = findCurrentTPS();

    //Destroy the page
    destroyPage(tps);
   

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

    /*
    Phase 2.2
    
    Before reading, change protection of memory page to allow read,
    then after reading, reset protection to not allow access
    */

    /*
    Phase 2.3

    
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

    //Change permission of memory page to allow read operation
    changeProtection(tps, PROT_READ);

    readFromPage(tps, offset, length, buffer);

    //Reset permission of memory page
    changeProtection(tps, PROT_NONE);

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

    /*
    Phase 2.2
    
    Before writing, change protection of memory page to allow write,
    then after reading, reset protection to not allow access
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

    //Change permission of memory page to allow write operation
    changeProtection(tps, PROT_WRITE);

    writeToPage(tps, offset, length, buffer);

    //Reset permission of memory page
    changeProtection(tps, PROT_NONE);

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

    //Change permission of source memory page to allow read operation
    changeProtection(tpssrc, PROT_READ);

    //Create tps for current thread
    tps_create();

    //Find it
    tps_t tpscurr = findCurrentTPS();

    //Change permission of current memory page to allow write operation
    changeProtection(tpscurr, PROT_WRITE);

    copyPage(tpssrc, tpscurr);

    //Reset permission of memory pages
    changeProtection(tpssrc, PROT_NONE);
    changeProtection(tpscurr, PROT_NONE);


   

    return 0;
}


