/*----------------------------------------------------------------------------
 *
 *					THE EXAMPLE DEMONSTRATING EFFICIENT USAGE OF 
 *				                    STORE BUFFERS
 *
 * Build 0x001 08.08.2002
----------------------------------------------------------------------------*/

// CONFIGURATION
//#define __WRITE_MISS__
// uncomment the previous line for testing write misses
// otherwise the write will be done to the memory,
// already loaded into L1-cache

#include <DoCPU.h>

main()
{
	int		*p;
	int		a, b, d;
	float	x = 0.666;

	// TITLE
	PRINT("= = = Usage of store buffers = = =\n");
	PRINT_TITLE;

	// allocating memory
	p = malloc(1*M);

	/*------------------------------------------------------------------------
	 *
	 *						NON-OPTIMIZED VERSION
	 *
	------------------------------------------------------------------------*/
	A_BEGIN(0)
		// 192 DWORD cells give 776 bytes of memory,
		// which exceeds the store buffers size twice
		// on P-II/P-III. processors. The stall will happen
		// approximately in the middle of operation and all
		// further writes will be significantly slower, because
		// they will have to wait until buffers get unloaded
		for(a = 0; a < 192; a += 8)
		{
			// to eliminate the overhead
			// the loop must be unrolled, otherwise
			// its splitting will decrease the performance
			p[a + 0] = (a + 0);
			p[a + 1] = (a + 1);
			p[a + 2] = (a + 2);
			p[a + 3] = (a + 3);
			p[a + 4] = (a + 4);
			p[a + 5] = (a + 5);
			p[a + 6] = (a + 6);
			p[a + 7] = (a + 7);
		}
		
		// do some calculations
		for(b = 0; b < 66; b++)
			x += x/cos(x);
		
		#ifdef __WRITE_MISS__
			// testing writing to the memory,
			// missing from L1 cache
			p += 196;
		#else
			// testing writing to the memory,
			// present in L1 cache
		#endif

	A_END(0)

	/*------------------------------------------------------------------------
	 *
	 *						OPTIMIZED VERSION
	 *
	------------------------------------------------------------------------*/
	A_BEGIN(1)

		// perform part of the planned calculations,
		// so that the buffers have enough time
		// to flush their current contents, because
		// it is not guaranteed that they will be empty
		// for the starting moment of the write loop
		for(b = 0; b < 33; b++)
			x += x/cos(x);

		// performing  96  write operations of the DWORD cells,
		// which corresponds to the size of write buffers; 
		// to the moment when the loop finishes execution
		// practically all buffers will be filled.
		// "Practically" is said, because some part of them
		// will still have time to flush
		for(a = 0; a < 96; a += 8)
		{
			p[a + 0] = (a + 0);
			p[a + 1] = (a + 1);
			p[a + 2] = (a + 2);
			p[a + 3] = (a + 3);
			p[a + 4] = (a + 4);
			p[a + 5] = (a + 5);
			p[a + 6] = (a + 6);
			p[a + 7] = (a + 7);
		}

		// executing the remaining part of calculations
		// it will be fine if during this time
		// all buffers will flush,
		// in which case we will reach full parallelism!
		for(b = 0; b < 33; b++)
			x += x/cos(x);

		// performing the remaining 96 write operations.
		// Since buffers will be free to that moment,
		// write operation will execute as fast as possible
		for(a = 96; a < 192; a += 8)
		{
			p[a + 0] = (a+0);
			p[a + 1] = (a+1);
			p[a + 2] = (a+2);
			p[a + 3] = (a+3);
			p[a + 4] = (a+4);
			p[a + 5] = (a+5);
			p[a + 6] = (a+6);
			p[a + 7] = (a+7);
		}
		#ifdef __WRITE_MISS__
			// testing writing to memory,
			// missing in L1 cache
			p+=196;
		#else
			// testing writing to memory,
			// present in L1 cache
		#endif	
	A_END(1)

	/*------------------------------------------------------------------------
	 *
	 *				OPTIMIZED ROLLED VERSION
	 *
	------------------------------------------------------------------------*/
	A_BEGIN(2)

		// introduction of a new loop allows to exclude the repeating code,
		// thus improving the program size.
		for(d = 0;d<192;d += 96)
		{
			for(b = 0; b < 33; b++)
				x += x/cos(x);

			for(a = d; a < d+96; a += 8)
			{
				p[a + 0] = (a + 0);
				p[a + 1] = (a + 1);
				p[a + 2] = (a + 2);
				p[a + 3] = (a + 3);
				p[a + 4] = (a + 4);
				p[a + 5] = (a + 5);
				p[a + 6] = (a + 6);
				p[a + 7] = (a + 7);
			}
		}

		#ifdef __WRITE_MISS__
			//  testing writing to memory,
			// missing in L1 cache
			p+=196;
		#else
			// testing writing to memory,
			// present in L1 cache
		#endif
	A_END(2)

	// console output of the results
	Lx_OUT("OPTIMIZE FOR SPEED        ",Ax_GET(0),Ax_GET(1));
	Lx_OUT("OPTIMIZE FOR SPEED & SPACE",Ax_GET(0),Ax_GET(2));
}


