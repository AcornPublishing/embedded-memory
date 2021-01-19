/* ---------------------------------------------------------------------------
 * @
 *						UTILITY DEMONSTRATING THE INFLUENCE
 *		OF ADDRESS ALIGNMENT ON THE COPYING SPEED OF LARGE MEMROY BLOCKS
 *		==================================================================
 *
 * Build 0x001 06.07.2002
--------------------------------------------------------------------------- */
// configuration
#define BLOCK_SIZE		(8*M)				// size of the copied block


#include <DoCPU.h>
main()
{
	char *p1, *p2;

	// TITLE
	PRINT("= = = influence of address alignment on the memory copying speed = = =\n");
	PRINT_TITLE;

	// allocating the memory
	p1 = malloc(BLOCK_SIZE);
	p2 = malloc(BLOCK_SIZE);


	/*------------------------------------------------------------------------
	 *
	 *					copying aligned block
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		memcpy(p2, p1, BLOCK_SIZE);
	A_END(0)

	/*------------------------------------------------------------------------
	 *
	 *				copying the block with unaligned target
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(1)
		memcpy(p2 + 1, p1, BLOCK_SIZE);
	A_END(1)

	/*------------------------------------------------------------------------
	 *
	 *				copying the block with unaligned source
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(2)
		memcpy(p2, p1 + 1, BLOCK_SIZE);
	A_END(2)

	/*------------------------------------------------------------------------
	 *
	 *				copying the totally unaligned block
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(3)
		memcpy(p2 + 1, p1 + 1, BLOCK_SIZE);
	A_END(3)

	// output
	Lx_OUT("unalign dst......",Ax_GET(0),Ax_GET(1));
	Lx_OUT("unalign src......",Ax_GET(0),Ax_GET(2));
	Lx_OUT("unalign src & dst",Ax_GET(0),Ax_GET(3));
}


