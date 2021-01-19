/*----------------------------------------------------------------------------
 *
 *					TEST PROGRAM FOR MEASURING
 *			EXECUTION TIME OF INDIVIDUAL MACHINE COMMANDS
 *			==============================================
 *
 * Build 0x001 16.08.2002
----------------------------------------------------------------------------*/
#include <DoCPU.h>

main()
{
	int		a;
	int		*p;

	PRINT("= = = Measuring execution time of individual machine commands = = =\n");
	PRINT_TITLE;

	// calling test.mod
	p=DoCPU(0);

	// console output of the results
	printf("{ // see test.mod \n");
		for (a = 0; a < 10; a++) printf("\tpass %d, CPU clock %d\n", a, p[a]);
	printf("}\n");
}