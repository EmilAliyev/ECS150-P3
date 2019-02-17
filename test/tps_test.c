#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#include <tps.h>

void thread1()
{
	printf("Testing thread 1...\n");
}

void test_destroy()
{
	printf("Testing tps_destroy()...\n");

	assert(tps_destroy() == 0);

	printf("Success!\n");
	
}

void test_create()
{
	printf("Testing tps_create()...\n");

	assert(tps_create() == 0);

	printf("Success!\n");
}

void test()
{
	test_create();
	test_destroy();
}

int main()
{
    tps_init(1);
	test();
}
