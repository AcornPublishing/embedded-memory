/* ---------------------------------------------------------------------
 * @
 *					UTILITY FOR MEASURING THE DEPENDENCE
 *		OF BLOCK PROCESSING TIME DEPENDING ON THE READ STEP
 *		=====================================================
 *
 * Build 0x001	26.04.2002 turkey
 * Build 0x002	24.05.2002 SEE standing.wave	
 * Build 0x003	24.05.2002 
------------------------------------------------------------------------ */
/*
	32KB saturation is explained by the fact that page table elements are cached in L1
	по 4 (dw) x 8 = 32 line size
*/

// CONFIGURATION
#define BLOCK_SIZE		(MAX_AVIAL_MEM/2)	// size of the processed block
#define STEP_FACTOR		(256)				// step increment size
#define MAX_STEP_SIZE	(100*K)				// maximum tested step

//#define UNTLB	 // uncomment this to mask the delay of 
//					page mapping

#define MAX_ITER		(BLOCK_SIZE/MAX_STEP_SIZE)
#define MAIN_ROOL(a)	for(a=STEP_FACTOR;a<MAX_STEP_SIZE;a+=STEP_FACTOR)

#include <DoCPU.h>
main()
{
	int *p;
	int a, b, i, x = 0;

	// TITLE
	PRINT("=== demonstration of the dependence of the block processing time on the steip size ===\n");
	PRINT_TITLE;

	// allocating memory
	p=(int*) _malloc32(BLOCK_SIZE+8*K);

	// header
	printf("STEP"); MAIN_ROOL(a) printf("\t%d",a); printf("\nTime:");

	memset(p,0,BLOCK_SIZE);

	MAIN_ROOL(a)
	{
		PRINT_PROGRESS(100*a/MAX_STEP_SIZE);VVV;
		AL_BEGIN;
			CLEAR_L2_CACHE();
			#ifdef UNTLB
				// loading pages into TLB
				for (i=0;i<=BLOCK_SIZE;i+=4*K) x += *(int *)((int)p + i+32);
			#endif
			L_BEGIN(0);		// <-- starting point of the execution time measurement
				i=0;
				// reading memory with step a
				for(b = 0; b < MAX_ITER; b++)
				{
					x += *(int *)((int)p + i);
					i += a;
					
				}
			L_END(0);		// <-- end point of the execution time measurement
		AL_END;

		// output of the execution time as a table for importing into MS Graph
		printf("\t%d", Ax_GET(0)/(GetCPUclock()/4));
	}
	printf("\n");

	return 0;
}


