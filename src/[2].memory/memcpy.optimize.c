/* ---------------------------------------------------------------------------
 * @
 *							MEMCPY OPTIMIZATION
 *							==================
 *
 * Build 0x001	24.06.2002
--------------------------------------------------------------------------- */
// configuration
#define BLOCK_SIZE		(8*M)				// size of the processed block
#define subBLOCK_SIZE	(L1_CACHE_SIZE/2)	// size of the sub-block
#define BRUST_LEN		(32)				// size of the read burst cycle
											// 32 bytes is the optimal choice
											// for all processors, although
											// to achieve maximum performance
											// it is necessary to determine this
											// value exactly

#include <DoCPU.h>
main()
{

	int *p1, *p2;
	int a, b, tmp=0;

	// TITLE
	PRINT("= = = demonstration of the optimized version of memcpy = = =\n");
	PRINT_TITLE;

	// выделяем память
	p1 = (int *) _malloc32(BLOCK_SIZE);		/* source */
	p2 = (int *) _malloc32(BLOCK_SIZE);		/* target */

	/*------------------------------------------------------------------------
	 *
	 *			non-optimized memory copying
	 *
	------------------------------------------------------------------------ */
	VVV;
	A_BEGIN(0)
		memcpy(p2,p1,BLOCK_SIZE);
	A_END(0)

	/*------------------------------------------------------------------------
	 *
	 *			optimized memory copying
	 *
	------------------------------------------------------------------------ */
	VVV;
	A_BEGIN(1)
		for (a = 0; a < BLOCK_SIZE; a+= subBLOCK_SIZE)
		{
			// parallel loading of cellsк
			for(b = 0; b < subBLOCK_SIZE; b += BRUST_LEN)
			{
				tmp += *(int *)((char *)p1 + a + b);
				//*(int *)((char *)p2 + a + b)=tmp;
			}

			// copying loaded cells
			for(b = 0; b < subBLOCK_SIZE; b += 32)
			{
				*(int *)((char *)p2 + a + b)= *(int *)((char *)p1 + a + b);
				*(int *)((char *)p2 + a + b + 4)= *(int *)((char *)p1 + a + b + 4);
				*(int *)((char *)p2 + a + b + 8)= *(int *)((char *)p1 + a + b + 8);
				*(int *)((char *)p2 + a + b + 12)= *(int *)((char *)p1 + a + b + 12);
				*(int *)((char *)p2 + a + b + 16)= *(int *)((char *)p1 + a + b + 16);
				*(int *)((char *)p2 + a + b + 20)= *(int *)((char *)p1 + a + b + 20);
				*(int *)((char *)p2 + a + b + 24)= *(int *)((char *)p1 + a + b + 24);
				*(int *)((char *)p2 + a + b + 28)= *(int *)((char *)p1 + a + b + 28);
			}
			// principally, it is sufficient to use the built-in memcpy, if it is implemented
			// efficiently
			//memcpy((int*)((char *)p2 + a ),(int*)((char *)p1 + a), subBLOCK_SIZE);
		}
	A_END(1)

	// output of the results
	printf("memcpy\t\t");L1_OUT("");
	printf("mymemcpy\t");Lx_OUT("",Ax_GET(0),Ax_GET(1));

	return tmp;
}


