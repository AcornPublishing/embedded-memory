/*----------------------------------------------------------------------------
 *
 *						THE EXAMPLE DEMONSTRATING THE EFFICIENCY
 *			OF LOADING DATA INTO L1 CACHE BYPASSING L2
 *			==========================================================
 *
 * Build 0x002 10.08.2002
----------------------------------------------------------------------------*/
#define BLOCK1_SIZE	(128*K)				// block that fits entirely into L2 cache,
										// but exceeding the L1 cache limits

#define BLOCK2_SIZE	(4*M)				// block that significantly exceeds
										// L2 cache size
#define STEP_SIZE	32
#include <DoCPU.h>
#define a _p2

/*----------------------------------------------------------------------------
 *
 *							TEST WITHOUT PREFETCHING
 *												(non-optimized version)
 *
----------------------------------------------------------------------------*/
float no_prefetch(int *p1, int *p2)
{
	int x = 0;
	int _p2, _p1 = 0;
	
	for(_p2 = 0; _p2 < BLOCK2_SIZE; _p2 += STEP_SIZE)
	{
		//				* * * processing BLOCK1 * * *
		// since it is processed in the loop (the A_BEGIN loop),
		// it is desirable that nothing would force it out of the L2 cache
		x += *(int *)((char *)p1 + _p1); if ((_p1 += 4) > BLOCK1_SIZE) _p1 = 0;

		//				* * * processing BLOCK1 * * *
		// this block significantly exceeds the L2 cache size, therefore,
		// it is not necessary to cache it - anyway at next loop iterations
		// the required data won't be present in cache...
		// Furthermore, L2 already contains BLOCK1, and discarding it is also
		// undesirable. However, the processor automatically places data
		// from BLOCK2 into L2 cache, thus choosing the least optimal caching strategy...
		x += *(int *)((char *)p2 + a);
		x += *(int *)((char *)p2 + a + 4);
		x += *(int *)((char *)p2 + a + 8);
		x += *(int *)((char *)p2 + a + 12);
		x += *(int *)((char *)p2 + a + 16);
		x += *(int *)((char *)p2 + a + 20);
		x += *(int *)((char *)p2 + a + 24);
		x += *(int *)((char *)p2 + a + 28);
	}
	return x;
}

/*----------------------------------------------------------------------------
 *
 *				TEST WITH prefetchnta (mem -> L1)
 *												(optimized version)
 *
----------------------------------------------------------------------------*/
int have_prefetchnta(int *p1, int *p2)
{
	int x = 0;
	int _p2, _p1 = 0;

	for(_p2 = 0; _p2 < BLOCK2_SIZE; _p2 += STEP_SIZE)
	{
		//				* * * processing BLOCK1 * * *
		// prefetching into L1 cache is not needed, because the performance
		// of the L2 cache (where this block resides)  is sufficient and
		// overhead for loading this data from L2 cache is negligible
		x += *(int *)((char *)p1 + _p1); if ((_p1 += 4) > BLOCK1_SIZE) _p1 = 0;

		// instruct th processor to load the data from BLOCK2 into L1 cache,
		// bypassing L2 cache. First, this allows to eliminate the delay
		// for loading the data from the slow RAM,  and, second,
		// prevents forcing BLOCK1 out of L2 cache
		_prefetchnta((char *)((char *)p2 + a + STEP_SIZE*6));
		//											^^^^
		// Note: we are prefetching the data that will be accessed six
		// iterations later. Why? The point is, that 
		// memory subsystem latency exceeds the time of one loop iteration;
		// by loading the data required for next iterations we are losing only 6 first iterations
		// rather than each six iterations, i.e., this technique is 
		// acceptable as it provides maximum performance gain
		// (for more details see "Planning prefetching distance")
		
		//				* * * processing BLOCK1 * * *
		// The data now is being loaded from L1 cache!
		x += *(int *)((char *)p2 + a);
		x += *(int *)((char *)p2 + a + 4);
		x += *(int *)((char *)p2 + a + 8);
		x += *(int *)((char *)p2 + a + 12);
		x += *(int *)((char *)p2 + a + 16);
		x += *(int *)((char *)p2 + a + 20);
		x += *(int *)((char *)p2 + a + 24);
		x += *(int *)((char *)p2 + a + 28);
	}
	return x;
}

/*----------------------------------------------------------------------------
 *
 *				TEST WITH prefetcht0 (Mem -> L2 -> L1)
 *													(optimized version)
 *
----------------------------------------------------------------------------*/
int have_prefetcht0(int *p1, int *p2)
{
	int x = 0;
	int _p2, _p1 = 0;

	for(_p2 = 0; _p2 < BLOCK2_SIZE; _p2 += STEP_SIZE)
	{
		//			* * * processing BLOCK1 * * *
		// prefetching into L1 cache is not needed, because the performance
		// of the L2 cache (where this block resides)  is sufficient and
		// overhead for loading this data from L2 cache is negligible
		x += *(int *)((char *)p1 + _p1); if ((_p1 += 4) > BLOCK1_SIZE) _p1 = 0;

		// for experiment, let us prefetch data into L1 and L2 cache.
		// This also eliminates delays for loading the data from slow RAM
		// but also litters L2 cache, which is filled with intensely used
		// BLOCK1;  consequently, prefetching efficiency slightly decreases
		// This dicrease, however, is not severe, because only one cache line
		// per iteration is discarded from cache
		_prefetcht0((char *)((char *)p2 + a + STEP_SIZE*6));

		//		* * * processing BLOCK1 * * *
		// The data now is being loaded from L1 cache!
		x += *(int*) ((char *)p2 + a);
		x += *(int *)((char *)p2 + a + 4);
		x += *(int *)((char *)p2 + a + 8);
		x += *(int *)((char *)p2 + a + 12);
		x += *(int *)((char *)p2 + a + 16);
		x += *(int *)((char *)p2 + a + 20);
		x += *(int *)((char *)p2 + a + 24);
		x += *(int *)((char *)p2 + a + 28);
	}
	return x;
}


/*----------------------------------------------------------------------------
 *
 *				TEST WITH prefetcht1 (Mem -> L2)
 *													(optimized version)
 *
----------------------------------------------------------------------------*/
int have_prefetcht1(int *p1, int *p2)
{
	int x = 0;
	int _p2, _p1 = 0;

	for(_p2 = 0; _p2 < BLOCK2_SIZE; _p2 += STEP_SIZE)
	{
		
		//				* * * processing BLOCK1 * * *
		// prefetching into L1 cache is not needed, because the performance
		// of the L2 cache (where this block resides)  is sufficient and
		// overhead for loading this data from L2 cache is negligible
		x += *(int *)((char *)p1 + _p1); if ((_p1 += 4) > BLOCK1_SIZE) _p1 = 0;

		// for experiment let us perform prefetching only into L2 cache; this
		// is the most unefficient prefetching strategy!  
		// Now the processor will have to wait until the data is loaded from L2 cache
		// into L1 cache, useful data are now forced out of the L2 cache
		// Naturally, this does not mean that  prefetch1  is "inefficient" instruction.
		// Simply, in this particular case such strategy proved to be
		// most inefficient
		_prefetcht1((char *)((char *)p2 + a + STEP_SIZE*6));

		//				* * * processing BLOCK1 * * *
		// the data are in L2 cache and the CPU has to wait several clocks
		// for the data to be loaded from there
		x += *(int *)((char *)p2 + a);
		x += *(int *)((char *)p2 + a + 4);
		x += *(int *)((char *)p2 + a + 8);
		x += *(int *)((char *)p2 + a + 12);
		x += *(int *)((char *)p2 + a + 16);
		x += *(int *)((char *)p2 + a + 20);
		x += *(int *)((char *)p2 + a + 24);
		x += *(int *)((char *)p2 + a + 28);
	}
	return x;
}

main()
{
	int *p1, *p2;

	// TITLE
	PRINT("= = = memory optimization using prefetch (prefetch type) = = =\n");
	PRINT_TITLE;

	// allocating memory
	p1 = (int *) _malloc32(BLOCK1_SIZE);
	p2 = malloc(BLOCK2_SIZE);

	A_BEGIN(1);
		no_prefetch(p1,p2);
	A_END(1);

	A_BEGIN(2)
		have_prefetchnta(p1, p2);
	A_END(2);
	
	A_BEGIN(3)
		have_prefetcht0(p1, p2);
	A_END(3);

	A_BEGIN(4)
			have_prefetcht1(p1, p2);
	A_END(4);

	Lx_OUT("PreFetchNTA",Ax_GET(1),Ax_GET(2));
	Lx_OUT("PreFetchT0 ",Ax_GET(1),Ax_GET(3));
	Lx_OUT("PreFetchT1 ",Ax_GET(1),Ax_GET(4));

}


