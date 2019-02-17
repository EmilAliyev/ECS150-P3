#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#include <tps.h>

void thread1()
{
	printf("Testing thread 1...\n");
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

	assert(tps_destroy() == 0);

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
}

int main()
{
    tps_init(1);
	test();
}
