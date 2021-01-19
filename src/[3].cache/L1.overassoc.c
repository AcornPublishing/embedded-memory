/*----------------------------------------------------------------------------
 *
 *				THE EXAMPLE DEMONSTRATING PERFORMANCE DROP
 *			that occurs when processed data hit the same cache line
 *			==============================================================
 *
 * Build 0x001 06.08.2002
----------------------------------------------------------------------------*/
// CONFIGURATION
#define N_ITER			466			// number of iterations
									// theoretically, LINE_SIZE*N_ITER bytes of cache memory 
									// will be required, since in this case
									// 466*64 = ~30 KB

#define CACHE_BANK_SIZE	(4*K)		// cache bank size

#define LINE_SIZE		64			// maximum possible size of cache lines

#define BLOCK_SIZE		((CACHE_BANK_SIZE+LINE_SIZE)*N_ITER) // block size

#include <DoCPU.h>					// include the profiler


//----------------------------------------------------------------------------
//
//					VERSION DEMONSTRATING CACHE LINE CONFLICT
//
//----------------------------------------------------------------------------
int over_assoc(int *p)
{
	int a;
	volatile int x=0;

	// attention: top-level loop is skipped since the 
	// profiler will anyway execute it 10 times
	for(a=0; a < N_ITER; a++)
		// read memory with 4 KB step, as a result,
		// cache saturation is quickly reached both on
		// P-II/P-III/P-4  and on AMD Athlon.
		// Cache is overloaded, since more that
		// 12 cells are processed, and read buffering
		// implemented on Athlon doesn't save
		// the situation
		//x+=*(int *)((int)p + a*CACHE_BANK_SIZE);
		*(int *)((char *)p + a*CACHE_BANK_SIZE)=x;

	return x;
}

//----------------------------------------------------------------------------
//
//					VERSION THAT EXECUTES WITHOUT CONFLICTS
//
//----------------------------------------------------------------------------
int optimize(int  *p)
{
	int a=0;
	volatile int x=0;

	// attention: top-level loop is skipped since the 
	// profiler will anyway execute it 10 times
	for(a=0; a < N_ITER; a++)
	{
		// Read memory with the step of  CACHE_BANK_SIZE+LINE_SIZE
		// since in this case 4096+64=4160 bytes;
		// since base addrsses of all cells are different
		// we have no conflicts and cache memory is used
		// with 100% efficiency
		//x+=*(int *)((int)p + a*(CACHE_BANK_SIZE+LINE_SIZE));
		*(int *)((char *)p + a*(CACHE_BANK_SIZE+LINE_SIZE))=x;
	}
	return x;
}


main()
{
	int *p;
	int a = 0;

	PRINT("= = = Demonstration of the cache line conflict = = =\n");
	PRINT_TITLE;

	// allocating memory
	p = (int *)_malloc32(BLOCK_SIZE);

	// pessimized version
	A_BEGIN(0)
		a+=over_assoc(p);
	A_END(0)

	// optimized version
	A_BEGIN(1)
		a+=optimize(p);
	A_END(1)

	// console output of the results
	Lx_OUT("",Ax_GET(1),Ax_GET(0));

}
