/*----------------------------------------------------------------------------
 *
 *					THE EXAMPLE DEMONSTRATING THE INFLUENCE
 *				OF PREFETCHED CACHE HIERARCHY ON THE PERFORMANCE
 *				=================================================
 *
 * Build 0x002 10.08.2002
----------------------------------------------------------------------------*/
#define BLOCK_SIZE	(4*M)
#define STEP_SIZE	32

//#include <math.h> 
#include <DoCPU.h>
#include <DoCPU.cmd.h>

/*----------------------------------------------------------------------------
 *
 *						TEST WITHOUT PREFETCHING
 *										(non-optimized version)
 *
----------------------------------------------------------------------------*/
int no_prefetch(int *p)
{
	int a;
	int x = 0;

	for(a = 0; a < BLOCK_SIZE; a += STEP_SIZE)
	{
		// performing some calculations (which in particular is not important)
		x += a/(a+1);
	
		// read the next portion of cells
		x += *(int *)((char *)p + a + 0);
		x += *(int *)((char *)p + a + 4);
		x += *(int *)((char *)p + a + 8);
		x += *(int *)((char *)p + a + 12);
		x += *(int *)((char *)p + a + 16);
		x += *(int *)((char *)p + a + 20);
		x += *(int *)((char *)p + a + 24);
		x += *(int *)((char *)p + a + 28);
	}
return x;
}

/*----------------------------------------------------------------------------
 *
 *						TEST WITH prefetchnta
 *								mem -> L1
 *
----------------------------------------------------------------------------*/
int have_prefetchnta(int *p)
{
	int a;
	int x = 0;

	for(a = 0; a < BLOCK_SIZE; a += STEP_SIZE)
	{
		// issue the command for prefetching the next cache line into L1 cache
		// and it will load in parallel with performing calculations of a/(a+1);
		// when appropriate cell will be requested, it will be present in cache
		// and the processor won't have to wait for it to load
		__prefetchnta((char *)((char *)p + a + STEP_SIZE*6));

		// performing some calculations
		x += a/(a+1);

		// reading the next portion of cells
		x += *(int *)((char *)p + a + 0);
		x += *(int *)((char *)p + a + 4);
		x += *(int *)((char *)p + a + 8);
		x += *(int *)((char *)p + a + 12);
		x += *(int *)((char *)p + a + 16);
		x += *(int *)((char *)p + a + 20);
		x += *(int *)((char *)p + a + 24);
		x += *(int *)((char *)p + a + 28);
	}
return x;
}


/*----------------------------------------------------------------------------
 *
 *						TEST WITH prefetchnt0
 *								mem -> L2 -> L1
 *
----------------------------------------------------------------------------*/
int have_prefetcht0(int *p)
{
	int a;
	int x = 0;

	for(a = 0; a < BLOCK_SIZE; a += STEP_SIZE)
	{
		// issue the command for prefetching the next cache line into L1/L2 cache
		// and it will load in parallel with performing calculations of a/(a+1);
		// when appropriate cell will be requested, it will be present in cache
		// and the processor won't have to wait for it to load
		_prefetcht0((char *)((char *) p + a + STEP_SIZE*6));

		// performing some calculations
		x += a/(a+1);

		// reading the next portion of cells
		x += *(int *)((char *)p + a + 0);
		x += *(int *)((char *)p + a + 4);
		x += *(int *)((char *)p + a + 8);
		x += *(int *)((char *)p + a + 12);
		x += *(int *)((char *)p + a + 16);
		x += *(int *)((char *)p + a + 20);
		x += *(int *)((char *)p + a + 24);
		x += *(int *)((char *)p + a + 28);
	}
return x;
}

/*----------------------------------------------------------------------------
 *
 *						TEST WITH prefetchnt1
 *								mem -> L2
 *
----------------------------------------------------------------------------*/
int have_prefetcht1(int *p)
{
	int a;
	int x = 0;

	for(a = 0; a < BLOCK_SIZE; a += STEP_SIZE)
	{
		// issue the command for prefetching the next cache line into L2 cache
		// and it will load in parallel with performing calculations of a/(a+1);
		// when appropriate cell will be requested, it will be present in cache
		// and the processor won't have to wait for it to load
		_prefetcht1((char *)((char *)p + a + STEP_SIZE*6));

		// performing some calculations
		x += a/(a+1);

		// reading the next portion of cells
		x += *(int *)((char *)p + a + 0);
		x += *(int *)((char *)p + a + 4);
		x += *(int *)((char *)p + a + 8);
		x += *(int *)((char *)p + a + 12);
		x += *(int *)((char *)p + a + 16);
		x += *(int *)((char *)p + a + 20);
		x += *(int *)((char *)p + a + 24);
		x += *(int *)((char *)p + a + 28);
	}
return x;
}



main()
{	
	int *p;

	PRINT("= = = memory optimization using prefetch = = =\n");
	PRINT_TITLE;

	// allocating memory
	p = malloc(BLOCK_SIZE);
	
	A_BEGIN(0)
		no_prefetch(p);
	A_END(0)

	A_BEGIN(1)
		have_prefetchnta(p);
	A_END(1)

	A_BEGIN(2)
		have_prefetcht0(p);
	A_END(2)

	A_BEGIN(3)
		have_prefetcht1(p);
	A_END(3)

	Lx_OUT("prefetchnta",Ax_GET(0),Ax_GET(1));
	Lx_OUT("prefetcht0 ",Ax_GET(0),Ax_GET(2));
	Lx_OUT("prefetcht1 ",Ax_GET(0),Ax_GET(3));


}


