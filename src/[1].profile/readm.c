/*----------------------------------------------------------------------------
 *
 *				EXAMPLE DEMONSTRATING INCONSTANCY
 *			of the memory block processing	time
 *          ======================================
 *
 * Build 0x001 16.08.2002
----------------------------------------------------------------------------*/
//#define __OVER_BRANCH__
// ^^^^ remove the comments from the previous line to activate "over-branching"
// and saturate the branch prediction block to clear the result from
// introduced distortions
#include <DoCPU.h>
#define	BLOCK_SIZE	_NORMAL

main()
{
	int		*p;
	int		x = 0;

	#define	a (int *)((char *)p + x)
	#define	b (int *)((char *)p + BLOCK_SIZE - x)

	p = (int *) malloc(BLOCK_SIZE);

    PRINT_TITLE; PRINT(("= = = Demonstrating inconstancy of the memory block processing time = = =\n"));

	A_BEGIN(0)
		for (x = 0; x < BLOCK_SIZE/2; x += sizeof(int))
		{
			#ifdef __OVER_BRANCH__
				if (x & 1)
			#endif
				*a = *a^*b; *b = *b^*a; *a = *a^*b;
		}
	A_END(0)

	// displaying the result of all measurements
	A_LIST_ITER(0)
}
