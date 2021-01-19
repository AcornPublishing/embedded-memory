/* ---------------------------------------------------------------------------
 * @
 *
 *		UTILITY FOR DEMONSTRATING THE EFFICIENCY OF PARALLEL MEMORY READ
 *		=================================================================
 *
 * Build 0x003	12.06.2002
 * Build 0x002	20.05.2002
--------------------------------------------------------------------------- */

// CONFIGURATION
#define _BLOCK_SIZE			(16*M)					// processed block size
#define _STEP_SIZE			(L1_CACHE_SIZE)			// size of the processed sub-block


#include <DoCPU.h>
main()
{
	int a, b;
	int x = 0;
	int *p;
	int BLOCK_SIZE = _BLOCK_SIZE;
	int STEP_SIZE  = _STEP_SIZE;
	
	// Allocating the memory
	p = malloc(BLOCK_SIZE);

	// TITLE
	PRINT("=== demonstrating the efficiency of parallel read ===\n");
	PRINT_TITLE;
	PRINT("------------------ Testing ------------------\n");

	/* -----------------------------------------------------------------------
	 *
	 *	measuring the throughput when sequentially reading the data
	 *
	----------------------------------------------------------------------- */
	CLEAR_L2_CACHE();VVV;
	A_BEGIN(1)
		for (a=0; a<BLOCK_SIZE; a += 8 * sizeof(int))
		{
			// loading the first cell
			// since it is missing from the cache,
			// processor sends to the chipset 
			// a request to read it
			x += *(int *)((char *)p + a + 0 * sizeof(int));

			// loading the next cell
			// since there is no dependence by data,
			// the processor can execute this command
			// without waiting for the previous one to complete
			// however, since the CPU sees 
			// that this cell is returned with the previous
			// request, it doesn't generate a new request,
			// but instead, waits for the current to complete
			x += *(int *)((char *)p + a + 1 * sizeof(int));

			// similarly, the processor waits for the first
			// request to accomplish..
			x += *(int *)((char *)p + a + 2 * sizeof(int));
			x += *(int *)((char *)p + a + 3 * sizeof(int));
			x += *(int *)((char *)p + a + 4 * sizeof(int));
			x += *(int *)((char *)p + a + 5 * sizeof(int));
			x += *(int *)((char *)p + a + 6 * sizeof(int));
			x += *(int *)((char *)p + a + 7 * sizeof(int));
		}
	A_END(1)

	printf("%s:\t %4.2f (Mbytes/s)\n",	/* displaying the results */
		_TEXT("Sequential read"),((BLOCK_SIZE)/cpu2time(Ax_GET(1))));


	/* -----------------------------------------------------------------------
	 *
	 *	Measuring the throughput at parallel data read
	 *
	----------------------------------------------------------------------- */
	CLEAR_L2_CACHE();VVV;
	A_BEGIN(2)
		for (b = 0; b < BLOCK_SIZE; b += STEP_SIZE)
		{						//       ^^^^^^	size of the processed block
								//				must not exceed the
								//				size of L1 cache

			// first pass of the loop in which the parallel
			// data loading is performed
			for (a=b; a<(b+STEP_SIZE); a+= 128)
			{
				// loading the first cell
				// since it is missing from the cache,
				// the processor sends the request to the chipset
				// to read this cell
				x += *(int *)((char *)p + a + 0);

				// loading the next cell
				// since there is no dependence by data,
				// the processor can execute this command
				// without waiting for the previous one to complete
				// however, since the processor sees that the
				// will not be returned with the block just
				// requested, it sends just another request
				// to the chipset without 
				// waiting for the previous one to accomplish
				x += *(int *)((char *)p + a + 32);

				// proceeding the similar way, - now there are three requests on the bus!
				x += *(int *)((char *)p + a + 64);

				// the fourth request is sent to the bus
				// while the first one might not be 
				// accomplished yet
				x += *(int *)((char *)p + a + 96);
			}

			for (a=b; a<(b+STEP_SIZE); a += 32)
			{
				// it is not necessary to read the next cell
				// since it was read in the first loop
				// x += *(int *)((char *)p + a + 0);

				// these cells will be already in cache!
				// now they can be loaded very fast!
				x += *(int *)((char *)p + a + 4);
				x += *(int *)((char *)p + a + 8);
				x += *(int *)((char *)p + a + 12);
				x += *(int *)((char *)p + a + 16);
				x += *(int *)((char *)p + a + 20);
				x += *(int *)((char *)p + a + 24);
				x += *(int *)((char *)p + a + 28);
			}
		}
	A_END(2)

	printf("%s:\t %4.2f (Mbytes/s)\n",	/* output of the results */
		_TEXT("Read with 32-byte step"),(BLOCK_SIZE)/cpu2time(Ax_GET(2)));
	PRINT(_TEXT("--------------------------------------------------\n"));

return x;

}

