/* ---------------------------------------------------------------------------
 *
 *			MEASUREMENT OF THE MEMORY INITIALIZATION SPEED IN BOTH DIRECTIONS
 *			===========================================================
 *
 * Build 0x001 26.08.2002
--------------------------------------------------------------------------- */

#include <DoCPU.h>

#define _PAGE_SIZE (4*K)				// size of one memory page
#define BLOCK_SIZE (4*M)				// processed block size


/* ---------------------------------------------------------------------------

								INITIALIZATION /---->

--------------------------------------------------------------------------- */
l_to_r(char *p)
{
			memset(p,0x66,BLOCK_SIZE);			// initializing memory
			A_READ(p,BLOCK_SIZE);				// imitating block processing
}


/* ---------------------------------------------------------------------------

							INITIALIZATION <-------/

--------------------------------------------------------------------------- */
r_to_l(char *p)
{
	char *p2;
	p2 = p;
	__asm{								// implement reverse copying in assembler
			STD							// setting the flag of reverse direction
			MOV	EDI,[p2]				// load the pointer to the block to be initialized into EDI
			MOV	ECX,BLOCK_SIZE/4		// load the number of double words to be written into ECX
			REP	STOSD					// initialize the memory by arbitrary value that happens to be contained in EAX
			MOV	[p2],EDI				// обновляем содержимое указателя
			CLD							// restoring the direction flag (this is required by C library functions)
	}
	A_READ(p2, BLOCK_SIZE);				// imitating block processing
}


/* ---------------------------------------------------------------------------

						OPTIMIZED VERSION

--------------------------------------------------------------------------- */
optimize(char *p)
{
	int a, b, c;

	b = BLOCK_SIZE;						// store the size of the initialized block into b
	c = _PAGE_SIZE;						// store the size of memory fragments into c
	while(b)							// initializing, until the entire block gets initialized
	{
		if (b < c) c = b;				// if the remaining "tail" is smaller than the page size,
										// correct the size of the remainder being initialized
			p -= c;							// correct the pointer
			memset(p, 0x66, c);				// initialize
			b -= c;							// decrease the block size by the value of "slice"
	}
	A_READ(p, BLOCK_SIZE);				// imitate processing
}


main()
{
	int b,c;
	char *p1,*p2,*p3;

	// allocating memory
	p1 = (char *) _malloc32(BLOCK_SIZE);
	p2 = (char *) _malloc32(BLOCK_SIZE);
	p3 = (char *) _malloc32(BLOCK_SIZE);

	// set the pointer to the end (memory will be initialized in reverse order)
	p2 += BLOCK_SIZE;
	p3 += BLOCK_SIZE;

	PRINT("= == memset direct test = = =\n");
	PRINT_TITLE;
	
	A_BEGIN(0)
		l_to_r(p1);
	A_END(0)

	A_BEGIN(1)
		r_to_l(p2);
	A_END(1)

	A_BEGIN(2)
		optimize(p3);
	A_END(2)


	Ax_OUT("<--", 0, 1);							// output <-- (first measurement - base)
	Ax_OUT("<->", 0, 2);							// output <-->
	return 0;
}



