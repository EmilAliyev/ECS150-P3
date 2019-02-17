#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#include <tps.h>


void *thread2(void *ptid)
{
    return NULL;
}


void *thread1()
{
    return NULL;
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

void test()
{
    test_init();
    test_create();
    test_destroy();
    test_clone();
}

int main()
{
    tps_init(1);
    test();
}
