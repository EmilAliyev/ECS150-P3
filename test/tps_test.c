#include <assert.h>
#include <stdio.h>

#include "tps.h"

void test_create()
{
	printf("Testing tps_create()...\n");

	assert(tps_create() == 0);

	printf("Success!\n");
}

void test()
{
	test_create();
}

int main()
{
	test();
}
