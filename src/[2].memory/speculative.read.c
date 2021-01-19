/* ---------------------------------------------------------------------------
 * @
 *					UTILITY DEMONSTRATING EFFICIENCY
 *			OF SPECULATIVE DATA LOADING
 *			===========================================
 *
 * Build 0x001 30.06.2002
--------------------------------------------------------------------------- */
// configuration
#define BLOCK_SIZE	(4*M)

#include <DoCPU.h>
main()
{
		int a;
		int x = 0;
		int y = 0;
		int *p;

		PRINT("= = = Demonstration of efficiency of speculative data loading = = =");
		PRINT_TITLE;

		// allocating memory
		p = malloc(BLOCK_SIZE);

		// initializing 
		memset(p, 6, BLOCK_SIZE);

		/*--------------------------------------------------------------------
		 *
		 *					non-optimized version
		 *
		------------------------------------------------------------------- */
		VVV;
		A_BEGIN(0)
		for(a = 0; a < BLOCK_SIZE; a += 32)
		{
			// loading cells
			x += (*(int *)((char *) p + a));
			x += (*(int *)((char *) p + a + 4));
			x += (*(int *)((char *) p + a + 8));
			x += (*(int *)((char *) p + a + 12));
			x += (*(int *)((char *) p + a + 16));
			x += (*(int *)((char *) p + a + 20));
			x += (*(int *)((char *) p + a + 24));
			x += (*(int *)((char *) p + a + 28));

			// performing some calculations
			x += a/x/666;
		}
		A_END(0)

		/*--------------------------------------------------------------------
		 *
		 *					optimized version with
		 *				   speculative loading
		 *
		------------------------------------------------------------------- */
		VVV;
		A_BEGIN(1)
		// loading the first portion of data
		x += (*(int *)((char *) p + a));
		for(a = 0; a < BLOCK_SIZE; a += 32)
		{
			// preloading of the new data portion
			y = (*(int *)((char *) p + a + 32));

			// processing previously loaded cells
			x += (*(int *)((char *) p + a + 4));
			x += (*(int *)((char *) p + a + 8));
			x += (*(int *)((char *) p + a + 12));
			x += (*(int *)((char *) p + a + 16));
			x += (*(int *)((char *) p + a + 20));
			x += (*(int *)((char *) p + a + 24));
			x += (*(int *)((char *) p + a + 28));
			
			// performing some calculations
			x += a/x/666;

			// using preloaded data
			x += y;
		}
		A_END(1)

		// console output of the results
		Lx_OUT("Speculative read",Ax_GET(0),Ax_GET(1));
		//printf("%f\n%f\n",BLOCK_SIZE/cpu2time(Ax_GET(0)),
		//BLOCK_SIZE/cpu2time(Ax_GET(1)));

		return x;
}



