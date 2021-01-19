/*----------------------------------------------------------------------------
 *						
 *						THE EXAMPLE DEMONSTRATES DEPENDENCE:
 *				PERFORMANCE (SIZE OF THE PROCESSED BLOCK OF CODE)
 *				======================================================
 *
 * Build 0x01 31.07.02
--------------------------------------------------------------------------- */

#include "code.cache.size.h"		// h-file is automatically generated
									// by make-file and contains the value
									// of the CODE_SIZE macro

#include <DoCPU.h>

main()
{
	int		a;

	// STARTING POINT OF MEASUREMENT OF THE EXECUTION TIME OF THE VLOCK OF CODE OF THE CODE_SIZE SIZE
	A_BEGIN(1);
		// calling the function containing CODE_SIZE NOP machine commands
		DoCPU(&a);
	A_END(1);	// end of measurement

	// console output of the result
	printf("%03d\t %d\n", CODE_SIZE, Ax_GET(1));
}