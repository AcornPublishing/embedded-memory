/* ---------------------------------------------------------------------------
 * @
 *					UTILITY FOR MEASURING THE DEPENDENCE 
 *		OF THE BLOCK PROCESSING TIME DEPENDING ON THE CELL ADDRESS
 *		======================================================
 *
 * Build 0x001	26.04.2002 turkey
 * Build 0x002	24.05.2002 see standing.wave
 * Build 0x003	24.05.2002 
--------------------------------------------------------------------------- */

// CONFIGURATION
#define BLOCK_SIZE		(128*K)					// size of the processed block
#define STEP_FACTOR		(256)					// Size of the step increment

#define MAIN_ROOL(a)	for(a=0;a<BLOCK_SIZE;a+=STEP_FACTOR)

#include <DoCPU.h>
main()
{
	int *p;
	unsigned int t;
	int a, x = 0;

	// TITLE
	PRINT("=== demonstration of the dependence of the block processing time on the step size ===\n");
	PRINT_TITLE; PRINT_PIPE;

	// allocating memory
	p = (int*) _malloc32(BLOCK_SIZE);

	// header
	printf("STEP"); MAIN_ROOL(a) printf("\t%d",a/1024); printf("\nTime");

	// uncomment this to load pages into TLB
	//for (i = 0; i <= BLOCK_SIZE; i += 4*K) x += *(int *)((int)p + i+256);
 
	CLEAR_L2_CACHE();
	MAIN_ROOL(a)
	{
			PRINT_PROGRESS(100*a/BLOCK_SIZE);VVV;

			A1(&t);						// <- starting point of the measurement of the execution time
				x += *(int *)((int)p + a);
			t=A2(&t);					// <- ending point of the measurment of the execution time
			
			// output of the results
			printf("\t%d",t);
	}
	printf("\n");

	return 0;
}


