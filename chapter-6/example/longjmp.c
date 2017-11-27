#include <setjmp.h>
#include "tlpi_hdr.h"

static jmp_buf env;

static void f2(void)
{
	longjmp(env, 2);
}

static void f1(int argc)
{
	if (argc == 1)
		longjmp(env, 1);
	f2();
}

int main(int argc, char *argv[])
{
	switch(setjmp(env))
	{
	case 0:
		printf("Calling f1() after the initial setjmp()\n");
		f1(argc);
		break;
	case 1:
		printf("We jumped back from f1()\n");
	case 2:
		printf("We jumped back from f2()\n");	
	}
	exit(EXIT_SUCCESS);