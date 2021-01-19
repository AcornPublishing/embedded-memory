/* ---------------------------------------------------------------------------
 * @
 *							UTILITY DEMONSTRATING
 *
 *						INEFFICIENCY
 *
 *					OF ALIGNINING STREAMS OF int DATA
 *
 * Build 0x001 05.07.2002
--------------------------------------------------------------------------- */
// configuration
#define BLOCK_SIZE	(8*M)

// number of iterations
#define N			(BLOCK_SIZE/sizeof(int))


#include <DoCPU.h>

// -[calculating array sum]----------------------------------------------------
//
//	ARG:
//		array	- pointer to array
//		n		- number of elements for sorting
//
// README:
//		This function doesn't align the pointer to array! It is up to you!
//----------------------------------------------------------------------------
int sum(int *array, int n)
{
	int a, x = 0;
	for(a = 0; a < n; a += 8)
	{
		x+=array[a + 0];
		x+=array[a + 1];
		x+=array[a + 2];
		x+=array[a + 3];
		x+=array[a + 4];
		x+=array[a + 5];
		x+=array[a + 6];
		x+=array[a + 7];
	}
	return x;
}

// -[calculating array sum]----------------------------------------------------
//
//	ARG:
//		array	- pointer to array
//		n		- number of elements for sorting
//
// README:
//		Function handles unaligned arrays, and aligns them
//	(to tell the truth, it would be better if it didn't)
//----------------------------------------------------------------------------
int sum_align(int *array, int n)
{
	int a, x = 0;
	char supra_bytes[4];
//	supra_bytes=_malloc32(4);

		// attention: this is a solution for a _particular_ case
		// when array & 15== 1, i.e., the pointer is simply
		// shifted 1 byte to the right from the 
		// address aligned by a 32 byte boundary
		// common solution without using loops
		// is impossible (and loops decrease the performance)
		// the only possible approach is manual creation
		// of a handler for each situation
		// There will be 32 - 32/4 = 24 cases, which is too
		// bulky for this book
	if (((int)array & 15)!=1)
		ERROR("-ERR: Invalid alignment\n");
		
	for(a = 0; a < n; a += 8)
	{
		
		// copying all double words that
		// do not cross the boundary of the burst exchange cycle
		x+=array[a + 0];
		x+=array[a + 1];
		x+=array[a + 2];
		x+=array[a + 3];
		x+=array[a + 4];
		x+=array[a + 5];
		x+=array[a + 6];

		// double word that crosses the boundary
		// must be copied into temporary buffer by b y t e s
		supra_bytes[0]=*((char *) array + (a+7)*sizeof(int) + 0);
		supra_bytes[1]=*((char *) array + (a+7)*sizeof(int) + 1);
		supra_bytes[2]=*((char *) array + (a+7)*sizeof(int) + 2);
		supra_bytes[3]=*((char *) array + (a+7)*sizeof(int) + 3);

		// get extra bytes and process them as double word
		x+=*(int *)supra_bytes;
	}

	return x;
}


main()
{
	int a;
	int *p;

	PRINT("= = = demonstration of inefficiency of alignment = = =\n");
	PRINT_TITLE;

	// allocating memeory
	p = (int *) _malloc32(BLOCK_SIZE);

	/*------------------------------------------------------------------------
	 *
	 *					processing aligned block
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		sum(p, N);
	A_END(0)

	/*------------------------------------------------------------------------
	 *
	 *					processing unaligned block
	 *
	----------------------------------------------------------------------- */
	// unalign the pointer
	p = (int *)((int)p + 1);

	VVV;
	A_BEGIN(1)
		sum(p, N);
	A_END(1)

	/*------------------------------------------------------------------------
	 *
	 *				trying to align the block from the called function
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(2)
		sum_align(p, N);
	A_END(2)

	// console output
	Lx_OUT("unalign.....",Ax_GET(0),Ax_GET(1));
	Lx_OUT("try optimize",Ax_GET(0),Ax_GET(2));
	
	
}



