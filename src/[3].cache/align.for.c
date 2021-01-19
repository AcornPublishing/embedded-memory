/*----------------------------------------------------------------------------
 *
 *				DEMONSTRATION OF THE CONSEQUENCES OF THE SPLITTING LOOP COUNTER
 *				=====================================================
 *
 * Build 0x001 05.08.2002
---------------------------------------------------------------------------- */

// CONFIGURATION
#define N_ITER	1000				// number of loop iterations
#define _MAX_CACHE_LINE_SIZE	32	//maximum possible size of the cache line

#define UN_FOX	(*(int*)((char*)fox + _MAX_CACHE_LINE_SIZE - sizeof(int)/2))
#define FOX	(*fox)					// determining aligned (FOX) and
									// unaligned (UN_FOX) counters

#include <DoCPU.h>

/*------------------------------------------------------------------------
 *
 *							OPTIMIZED VERSION
 *					(loop counter does not split cache line)
 *
------------------------------------------------------------------------*/
optimize(int *fox)
{
	int c = 0;

	for(FOX = 0; FOX < N_ITER; FOX+=1)
		c+=FOX;
		// ^^^^	it is also possible ot use c++, however, in this case MS VC 6.0
		//		would recognize this construction and replace
		//		the entire loop by c += N_ITER, which does not correspond 
		//		to our plans!!!

	return c;
}


/*------------------------------------------------------------------------
 *
 *						PESSIMIZED VERSION
 *					(loop counter splits cache line)
 *
------------------------------------------------------------------------*/
pessimize(int *fox)
{
	int c = 0;

	for(UN_FOX = 0; UN_FOX < N_ITER; UN_FOX+=1)
		c+=FOX;

	return c;
}

main()
{
	int		*fox;

	PRINT("= = = Demonstration of the consequences of the splitting loop counter = = =\n");
	PRINT_TITLE;

	// allocating memory
	fox = malloc(1*M);			// fox is guaranteed to be aligned 
								// by 32 bytes boundary

	// STARTING POINT OF THE MEASUREMENT OF THE EXECUTION TIME FOR OPTIMIZED VERSION
	A_BEGIN(0)
		optimize(fox);
	A_END(0)		// end of measurement

	// STARTING POINT OF THE MEASUREMENT OF THE EXECUTION TIME FOR PESSIMIZED VERSION
	A_BEGIN(1)
		pessimize(fox);
	A_END(1)		// end of measurement

	// console output of the results
	Lx_OUT("",Ax_GET(0),Ax_GET(1));

	return 0;
}
