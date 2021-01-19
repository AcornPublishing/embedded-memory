/*----------------------------------------------------------------------------
 *						
 *						THIS EXAMPLE DEMONSTRATES DEPENDENCE:
 *				PERFORMANCE (SIZE OF THE PROCESSED DATA BLOCK)
 *				========================================================
 *
 * Build 0x02 29.08.02
 * Build 0x01 30.05.02
--------------------------------------------------------------------------- */

// CONFIGURATION
#define	BLOCK_SIZE	(745*K)	// size of the processed memory block
#define STEP_FACTOR	(1*K)	// increment step for the tested size
#define X			b		// :'b' - for subtracting linear component
							// :'1' - show graph "as is"

#include <DoCPU.h>

main()
{
	int *p;
	int b, c;
	int tmp = 0;

	#ifdef MOVE_STACK
		char o_b[20*K];
	#endif

	// allocating memory
	p = malloc(BLOCK_SIZE);

	// header for table
	printf("---\t");
	for (b = 1*K; b < BLOCK_SIZE; b += STEP_FACTOR)
		printf("%d\t",b/K); printf("\n");
	

	/*------------------------------------------------------------------------
	 *
	 *							SEQUENTIAL READ
	 *
	 ---------------------------------------------------------------------- */
	printf("R\t");	for (b = 1*K; b < BLOCK_SIZE; b += STEP_FACTOR)
	{ //			^^^^^^^^^^^^ loop for testing blocks of different size
		// displaying progress
		PRINT_PROGRESS(25*b/BLOCK_SIZE); VVV;

		// STARTING POINT OF THE MEASUREMENT OF THE EXECUTION TIME FOR PROCESSING BLOCK OF THE SIZE b
		A_BEGIN(0)
			for (c = 0; c <= b; c += sizeof(int)*4)
			{ //								 ^- loop unrolling depth
				tmp += *(int*)((char *)p + c + 0);
				tmp += *(int*)((char *)p + c + 4);
				tmp += *(int*)((char *)p + c + 8);
				tmp += *(int*)((char *)p + c + 12);
			}
		A_END(0)		// end of measurement

		// console output of the result
		printf("%d\t", 100*Ax_GET(0)/X);
	}
	printf("\n");


	/*------------------------------------------------------------------------
	 *
	 *							SEQUENTIAL WRITE
	 *
	 ---------------------------------------------------------------------- */
	printf("W\t"); for (b = 1*K; b < BLOCK_SIZE; b += STEP_FACTOR)
	{
		PRINT_PROGRESS(25+25*b/BLOCK_SIZE); VVV;

		A_BEGIN(1)
			for (c = 0; c <= b; c += sizeof(int)*4)
			{
				*(int*)((char *)p + c + 0) = tmp;
				*(int*)((char *)p + c + 4) = tmp;
				*(int*)((char *)p + c + 8) = tmp;
				*(int*)((char *)p + c + 12) = tmp;
			}
		A_END(1)

		printf("%d\t", 100*Ax_GET(1)/X);
	}
	printf("\n");


	/*------------------------------------------------------------------------
	 *
	 *					SEQUENTIAL READ then WRITE
	 *
	 ---------------------------------------------------------------------- */
	printf("RW\t"); for (b = 1*K; b < BLOCK_SIZE; b += STEP_FACTOR)
	{
		PRINT_PROGRESS(50+25*b/BLOCK_SIZE); VVV;

		A_BEGIN(2)
			for (c = 0; c <= b; c += sizeof(int)*2)
			{
				tmp += *(int*)((char *)p + c + 0);
				tmp += *(int*)((char *)p + c + 4);
				*(int*)((char *)p + c + 0) = tmp;
				*(int*)((char *)p + c + 4) = tmp;

			}
		A_END(2)

		printf("%d\t", 100*Ax_GET(2)/X);
	}
	printf("\n");


	/*------------------------------------------------------------------------
	 *
	 *					SEQUENTIAL READ then WRITE
	 *
	 ---------------------------------------------------------------------- */
	printf("WR\t"); for (b = 1*K; b < BLOCK_SIZE; b += STEP_FACTOR)
	{
		PRINT_PROGRESS(75+25*b/BLOCK_SIZE); VVV;

		A_BEGIN(3)
			for (c = 0; c <= b; c += sizeof(int)*2)
			{
				*(int*)((char *)p + c + 0) = tmp;
				*(int*)((char *)p + c + 4) = tmp;
				tmp += *(int*)((char *)p + c + 0);
				tmp += *(int*)((char *)p + c + 4);
			}
		A_END(3)

		printf("%d\t", 100*Ax_GET(3)/X);
	}
	printf("\n");

	return tmp;
}
