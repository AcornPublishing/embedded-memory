/*----------------------------------------------------------------------------
 *
 *					DEMONSTRATION OF THE SEQUENCE OF ADDRESSES
 *				IN BURST EXCHANGE CYCLE
 *				================================================
 *
 * Build 0x001 25.07.2002
 ---------------------------------------------------------------------------*/

// NOTE ¹1	Unfavorable order of requesting cells is only possible
//========	when reading dependable data.

// NOTE ¹2	This is true both for main memory and for L2 cache
// ========

// CONFIGURATION
#define BLOCK_SIZE	(4*M)					// size of the processed memory block
#define BRST_LEN	32						// number of bytes is burst cycle

#include <DoCPU.h>

main()
{
	int		a;
	int		*p;
	int		x =0, y = 0;

	// TITLE
	PRINT("=== Demonstration of the address sequence in burst cycle ===\n");
	PRINT_TITLE;

	// allocating and initializing memory
	p = malloc(BLOCK_SIZE); memset(p, 0, BLOCK_SIZE);

	/*------------------------------------------------------------------------
	 *
	 *					NORMAL TRANSFER ORDER
	 *
	 -----------------------------------------------------------------------*/
	A_BEGIN(0)
		for (a = 0; a < (BLOCK_SIZE - BRST_LEN); a += BRST_LEN)
		{
			x += *(int *)((char *)p + a + 0x0 + x);
			x += *(int *)((char *)p + a + 0x8 + x);
		}
	A_END(0)


	/*------------------------------------------------------------------------
	 *
	 *						STARTING AND ENDING ADDRESSES
	 *
	 -----------------------------------------------------------------------*/
	A_BEGIN(1)
		for (a = 0; a < (BLOCK_SIZE - BRST_LEN); a += BRST_LEN)
		{
			x += *(int *)((char *)p + a + 0x0  + x);
			x += *(int *)((char *)p + a + 0x18 + x);
		}
	A_END(1)


	/*------------------------------------------------------------------------
	 *
	 *	NEIGHBOURING ADDRESSES 0x8 & 0x10 ARE NOT TRANSMITTED AT THE START AND END OF THE LOOP
	 *
	 -----------------------------------------------------------------------*/
	A_BEGIN(2)
		for (a = 0; a < (BLOCK_SIZE - BRST_LEN); a += BRST_LEN)
		{
			// ======= CORRECTING ADDRESS ORDER TO FAVORABLE ONE =========
			// if we start requesting cells from 0x0 address, the cells 0x8 and 0x10
			// will be transmitted in adjacent packets, and this will be fine!
			// *
			//y += *(int *)((int)p + a + 0x0 + 0x0 + 0); // 31 % vs 75
			y += *(int *)((int)p + a + 0x0 + 0x0 + y); // 88% vs 66%
			//x += *(int *)((char *)p + a + 0x0 + 0x0 + x); // 111% vs 133% <- ops!
			// 117% vs 133%
			// ===============================================================
			x += *(int *)((char *)p + a + 0x0 + 0x8 + x);
			x += *(int *)((char *)p + a + 0x8 + 0x10 + x);
		}
	A_END(2)

	// console output of the results
	Ax_OUT("NORMAL vs FIRST-to-END", 0, 1);
	Ax_OUT("NORMAL vs OPTIMIZED   ", 0, 2);

	return x+y;
}
