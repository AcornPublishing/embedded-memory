/* ---------------------------------------------------------------------------
 * @
 *				UTILITY DEMONSTRATING EFFICIENCY
 *			OF ALIGNING BYTE DATA STREAMS
 *			==============================================
 *
 * Build 0x001 05.07.2002
--------------------------------------------------------------------------- */
// configuration
#define BLOCK_SIZE (4*M)

// number of iterations
#define N (BLOCK_SIZE/sizeof(int))


#include <DoCPU.h>

// -[Simple byte-crypt]-------------------------------------------------------
//
//	ARG:
//		src		- pointer to the block being encrypted
//		mask	- encryption mask (bytes)
//
//	README:
//		to achieve maximum performance, the block being encrypted must be
// aligned by the 4 bytes boundary
//----------------------------------------------------------------------------
void unalign_crypt(char *src, int n, int mask)
{
	int a;
	int supra_mask;

	// spawining the byte mask to obtain double word	
	supra_mask = mask+(mask<<8)+(mask<<16)+(mask<<24);
	
	// process bytes by double words
	for (a = 0; a < n; a += 4)
	{
		 *(int *)src ^= supra_mask;  src+=4;
	}

	// process the remaining "tail" (if there is one)
	for (a = (n & ~3); a < n; a++)
	{
		 *src ^= mask;	 src += 1;
	}

}

// -[Simple byte-crypt]-------------------------------------------------------
//
//	ARG:
//		src		- pointer to the block being encrypted
//		mask	- encryption mask (byte)
//
//	DEPENCE:
//		unalign_crypt
//
//	README:
//		this function automatically aligns the data being encrypted
//----------------------------------------------------------------------------
void align_crypt(char *src, int n, int mask)
{
	int n_ualign;
	
	// calculate the offset value
	// required to make the block an aligned one
	n_ualign= 32 - ((int) src & 15);

	// encrypt until reaching the boundary of the burst cycle
	unalign_crypt(src, n_ualign, mask);

	// encrypt the remainder
	// since src+n_ualign is guaranteed to be aligned pointer!
	unalign_crypt(src+n_ualign, n-n_ualign, mask);
	/* don't forget to decrease the   ^^^^^^^^^^^^ number of bytes being encrypted */
}


main()
{
	char *p;
	
	// TITLE
	PRINT("= = = demonstration of the efficiency of aligning byte streams = = =\n");
	PRINT_TITLE;

	// allocating memory
	p=malloc(BLOCK_SIZE);


	/* -----------------------------------------------------------------------
	 *
	 *					processing aligned block
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		unalign_crypt(p, BLOCK_SIZE, 0x66);
	A_END(0)


	/* -----------------------------------------------------------------------
	 *
	 *					processing unaligned block
	 *
	----------------------------------------------------------------------- */
	VVV;
	// unalign the pointer
	p=p + 1;

	A_BEGIN(1)
		unalign_crypt(p , BLOCK_SIZE, 0x66);
	A_END(1)

	/* -----------------------------------------------------------------------
	 *
	 *					automatic alignment
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(2)
		align_crypt(p, BLOCK_SIZE, 0x66);
	A_END(2)

	// console output of the results
	Lx_OUT("unalign...",Ax_GET(0),Ax_GET(1));
	Lx_OUT("auto align",Ax_GET(0),Ax_GET(2));
	return 0;
}





