/*********************************************************************************************************************
 * 编译器优化会重组程序的指令执行顺序，并在寄存器中存储变量。longjmp()操作会导致经过优化的变量被赋以错误值。
 * 将变量声明为volatile告诉优化器不要对其进行优化，从而避免了代码重组。
**********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

static void doJump(int nvar, int rvar, int vvar)
{
	printf("Inside doJump(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
	longjmp(env, 1);
}

int main(int argc, char **argv)
{
	int nvar;
	register int rvar;
	volatile int vvar;

	nvar = 111;
	rvar = 222;
	vvar = 333;

	if (setjmp(env) == 0) {
		nvar = 777;
		rvar = 888;
		vvar = 999;
		doJump(nvar, rvar, vvar);
	} else {
		printf("After longjmp(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
	}
	exit(EXIT_SUCCESS);
}