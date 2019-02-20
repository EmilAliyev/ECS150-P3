#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sem.h>
#include <tps.h>

static sem_t sem1, sem2;

static char msg1[TPS_SIZE] = "Hello 1\n";
static char msg2[TPS_SIZE] = "hello 2\n";

void *thread2(void *ptid)
{
    char *buffer = malloc(TPS_SIZE);

    /* Create TPS and initialize with *msg1 */
    tps_create();
    tps_write(0, TPS_SIZE, msg1);

    /* Read from TPS and make sure it contains the message */
    memset(buffer, 0, TPS_SIZE);
    tps_read(0, TPS_SIZE, buffer);
    assert(!memcmp(msg1, buffer, TPS_SIZE));
    printf("thread2: read OK!\n");

    /* Transfer CPU to thread 1 and get blocked */
    sem_up(sem1);
    sem_down(sem2);

    /* When we're back, read TPS and make sure it sill contains the original */
    memset(buffer, 0, TPS_SIZE);
    tps_read(0, TPS_SIZE, buffer);
    assert(!memcmp(msg1, buffer, TPS_SIZE));
    printf("thread2: read OK!\n");

    /* Transfer CPU to thread 1 and get blocked */
    sem_up(sem1);
    sem_down(sem2);

    /* Destroy TPS and quit */
    tps_destroy();
    return NULL;
}


void *thread1()
{
    pthread_t tid;
    char *buffer = malloc(TPS_SIZE);

    /* Create thread 2 and get blocked */
    pthread_create(&tid, NULL, thread2, NULL);
    sem_down(sem1);

    /* When we're back, clone thread 2's TPS */
    tps_clone(tid);

    /* Read the TPS and make sure it contains the original */
    memset(buffer, 0, TPS_SIZE);
    tps_read(0, TPS_SIZE, buffer);
    assert(!memcmp(msg1, buffer, TPS_SIZE));
    printf("thread1: read OK!\n");

    /* Modify TPS to cause a copy on write */
    buffer[0] = 'h';
    tps_write(0, 1, buffer);

    /* Transfer CPU to thread 2 and get blocked */
    sem_up(sem2);
    sem_down(sem1);

    /* When we're back, make sure our modification is still there */
    memset(buffer, 0, TPS_SIZE);
    tps_read(0, TPS_SIZE, buffer);
    assert(!strcmp(msg2, buffer));
    printf("thread1: read OK!\n");

    /* Transfer CPU to thread 2 */
    sem_up(sem2);

    /* Wait for thread2 to die, and quit */
    pthread_join(tid, NULL);
    tps_destroy();

    return NULL;
}

//Basic read test (no offset)
void test_read3()
{
    printf("Testing tps_read()...\n");

    char buffer[20];
    int size = 5;
    int offset = 5;

    assert(tps_read(offset, size, buffer) == 0);

    printf("Buffer reads: %s\n", buffer);
    printf("Success!\n");
}

//Illegal read test
void test_read2()
{ 
    char buffer[20];
    int size = 5;
    int offset = 4096;

    printf("Testing out of bounds tps_read()...\n");

    assert(tps_read(offset, size, buffer) == -1);

//    printf("Out of bounds read failed properly!\n"); 
    printf("Success!\n");
}

//Basic read test (no offset)
void test_read1()
{
    printf("Testing tps_read()...\n");

    char buffer[20];
    int size = 5;
    int offset = 0;

    assert(tps_read(offset, size, buffer) == 0);

    printf("Buffer reads: %s\n", buffer);
    printf("Success!\n");
}

void test_read()
{
    test_read1();
    test_read2();
    test_read3();
}

//Basic write test (no offset)
void test_write1()
{
    printf("Testing tps_write()...\n");

    char* buffer = "Hello World";
    int size = 10;
    int offset = 0;

    assert(tps_write(offset, size, buffer) == 0);

    printf("Success!\n");
}

//illegal offset write test
void test_write2()
{
    printf("Testing illegal offest tps_write()...\n");

    char* buffer = "Hello World";
    int size = 10;
    int offset = 4096;

    assert(tps_write(offset, size, buffer) == -1);

    printf("Success!\n");
}

void test_write()
{
    test_write1();
    test_write2();
}

//Test tps already exists error
void test_clone2()
{

    
}

//Test tid not found error
void test_clone1()
{
    printf("Cloning test 1... TID not found test\n");

    assert(tps_clone(100) == -1);

    printf("Success!\n");
}


//Basic clone test
void test_clone()
{
    test_clone1();
    test_clone2();
}

//Init test - must return -1 if called for second time
void test_init()
{
    printf("Testing tps_init()...\n");

    assert(tps_init(1) == -1);

    printf("Success!\n");
}

//Basic destroy test
void test_destroy()
{
    printf("Testing tps_destroy()...\n");

    //First destroy should be successful
    assert(tps_destroy() == 0);
    
    //Second destroy should be unsuccessful as there is nothing to destroy!
    assert(tps_destroy() == -1);

    printf("Success!\n");
    
}

//Basic create test
void test_create()
{
    printf("Testing tps_create()...\n");

    //First create should be successful
    assert(tps_create() == 0);

    //Second create should be unsuccessful
    assert(tps_create() == -1);

    printf("Success!\n");
}

void basic_test()
{
    test_init();
    test_create();

    test_write();
    test_read();

    test_destroy();
    test_clone();
}

void comp_test()
{
    printf("\nBeginning comprehensive tests\n");
    
    pthread_t tid;
 
    //maybe move all this to an expanded testing fn?
    sem1 = sem_create(0);
    sem2 = sem_create(0);

    pthread_create(&tid, NULL, thread1, NULL);
    pthread_join(tid, NULL);

    sem_destroy(sem1);
    sem_destroy(sem2);   
}

int main()
{
    tps_init(1);

    //start by running all the basic diagonostics/tests
    basic_test();

    //Comprehensive test
    comp_test();
}
