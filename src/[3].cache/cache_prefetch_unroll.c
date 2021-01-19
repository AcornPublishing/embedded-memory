/* ---------------------------------------------------------------------------
 *
 *		EXAMPLE DEMONSTRATING THE EFFECT OF UNROLLING THE PREFETCH LOOP
 *		===================================================================
 *
 * Build 0x001 26.08.2002
--------------------------------------------------------------------------- */

#define BLOCK_SIZE	(4*M)		// selecting large memory block,
								// to exclude cache influence

#define STEP_SIZE	4			// read step


#define computation(a, b, p) b += (a)*0x666; b += *(int *)((char *)(p)+a);

#include <DoCPU.h>
#include <DoCPU.cmd.h>

/* ------------------------------------------------------------------

					TEST WITHOUT PREFETCHING
										(non-optimized version)

--------------------------------------------------------------------- */
normal(char *p, int len)
{
	int a;
	int x = 0;
	for(a = 0; a < len; a += 4)
	{
		computation(a, x, p);
	}
	return x;
}


/* ------------------------------------------------------------------

					TEST WITOUT UNROLLING
										(non-optimized version)
	
--------------------------------------------------------------------- */
prefetch(char *p, int len)
{
	int a;
	int x = 0;

	for(a = 0; a < len; a += 4)
	{
		__prefetchnta(p + a + 32*5);
			computation(a, x, p);
	}
	return x;
}


/* -------------------------------------------------------------------
	
					TEST WITH UNROLLING
										(optimized version)

---------------------------------------------------------------------- */
unroll_prefetch(char *p, int len)
{
	int a;
	int x = 0;
	for(a = 0; a < len; a += 32)
	{
		__prefetchnta(p + a + 32*5);

		computation(a + 0,	x, p);
		computation(a + 4,	x, p);
		computation(a + 8,	x, p);
		computation(a + 12,	x, p);
		computation(a + 16,	x, p);
		computation(a + 20,	x, p);
		computation(a + 24,	x, p);
		computation(a + 28,	x, p);
	}
}

main()
{
	char *p;
	

	// allocating memory
	p= (char *) _malloc32(BLOCK_SIZE);

	// TITLE
	PRINT("= = = memory optimization using prefetch whith loop unrolling = = =\n");
	PRINT_TITLE;

	A_BEGIN(0)
		normal(p, BLOCK_SIZE);
	A_END(0)

	A_BEGIN(1)
		prefetch(p, BLOCK_SIZE);
	A_END(1)

	A_BEGIN(2)
		unroll_prefetch(p, BLOCK_SIZE);
	A_END(2)

	// console output of the results
	Ax_OUT("Over Prefetch",0,1);
	Ax_OUT("OPTIMIZED    ",0,2);
}


