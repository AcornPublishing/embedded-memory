/* ---------------------------------------------------------------------------
 *
 *				DEMONSTRATION OF THE MEMORY DELAYS
 *			WHEN READING/WRITING DATA OF DIFFERENT SIZE
 *			====================================================
 *
 * Build 0x001 01.09.2002
--------------------------------------------------------------------------- */
#define BLOCK_SIZE	(16*K)			// size of the processed block

#include <DoCPU.h>
main()
{
	int		a;
	int		*p;
	char	tmp8  = 0;
	int		tmp32 = 0;

	PRINT("= = = Demonstration of the delays when reading/writing data of different size = = =\n");
	PRINT_TITLE;

	// allocating memory
	p = (int*)_malloc32(BLOCK_SIZE);

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *						OPTIMIZED VERSION
	 *					Long Write/Long Read (same addrr)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(0)
		for(a = 0; a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			*(int *)((char *)p + a) = tmp32;
			tmp32 += *(int *)((char *)p + a);
		}
	A_END(0)

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Short Write/Long Read (same addrr)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(1)
		for(a = 0; a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			*(char *)((char *)p + a) = tmp8;
			tmp32 += *(int *)((char *)p + a);
		}
	A_END(1)

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Short Write/Long Read (overlap space)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(2)
		for(a = sizeof(tmp32); a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			*(char *)((char *)p + a + (sizeof(tmp32)/2)) = tmp8;
			tmp32 += *(int *)((char *)p + a);
		}
	A_END(2)

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Long Write/Short Read (same addrr)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(3)
		for(a = 0; a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			*(int *)((char *)p + a) = tmp32;
			tmp8 += *(char *)((char *)p + a);
		}
	A_END(3)

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Long Write/Short Read (overlap space)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(4)
		for(a = sizeof(tmp32); a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			*(int *)((char *)p + a) = tmp32;
			tmp8 += *(char *)((char *)p + a + (sizeof(tmp32)/2));
		}
	A_END(4)

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Long Write/Long Read (overlap space)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(5)
		for(a = sizeof(tmp32); a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			*(int *)((char *)p + a) = tmp32;
			tmp32 += *(int *)((char *)p + a - (sizeof(tmp32)/2));
		}
	A_END(5)


	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Long Write/Short Read (overlap space)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(6)
		for(a = 0; a < BLOCK_SIZE; a += sizeof(int))
		{
			*(int *)((char *)p + a)= tmp32;
			tmp8 += (((*(int *)((char *)p + a))  & 0x00FF0000 ) >> 0x10 );
		}
	A_END(6)



	Lx_OUT("Short Write/Long  Read (same addrr)   ", Ax_GET(0), Ax_GET(1));
	Lx_OUT("Short Write/Long  Read (overlap space)", Ax_GET(0), Ax_GET(2));
	Lx_OUT("Long  Write/Short Read (same addrr)   ", Ax_GET(0), Ax_GET(3));
	Lx_OUT("Long  Write/Short Read (overlap space)", Ax_GET(0), Ax_GET(4));
	Lx_OUT("Long  Write/Long  Read (overlap space)", Ax_GET(0), Ax_GET(5));
	Lx_OUT("Long  Write/Long  Read (overlap space)", Ax_GET(0), Ax_GET(6));
	return tmp8+tmp32;
}






