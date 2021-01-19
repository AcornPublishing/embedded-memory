/*----------------------------------------------------------------------------
 * @
 *				UTILITY FOR DEMONSTRATING THE EFFICIENCY 
 *			OF PROCESSING MEMORY IN DOUBLE WORDS
 *			===============================================
 * 
 * Build 0x001 06.07.2002
--------------------------------------------------------------------------- */

// CONFIGURATION
#define BLOCK_SIZE	(1*M)					// size of the processed block


#include <DoCPU.h>

main()
{
	int a, x = 0;
	int   *p_dw;
	float d = 0;
	char  *p_b, *buf;	

	// TITLE
	PRINT("= = = demonstrating of efficiency of processing double words = = =\n");
	PRINT_TITLE;

	// allocating memory
	buf		= (char *) _malloc32(8);
	p_b		= (char *) _malloc32(BLOCK_SIZE);
	p_dw	= (int *) _malloc32(BLOCK_SIZE);
	
	
	/*------------------------------------------------------------------------
	 *
	 *						processing memory in bytes
	 *								(read)
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		for(a = 0; a < BLOCK_SIZE; a++)
			x+=p_b[a];
	A_END(0)

	/*------------------------------------------------------------------------
	 *
	 *					processing memory in double words
	 *								(read)
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(1)
		for(a = 0; a < BLOCK_SIZE; a += sizeof(int))
			x+=*(int*)((char *)p_dw + a);
	A_END(1)

	/*------------------------------------------------------------------------
	 *
	 *					processing memory in quad words
	 *								(read)
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(2)
		for(a = 0; a< BLOCK_SIZE; a += 8)
			_mmx_cpy(buf, (char *)((char *)p_dw+a));
	A_END(2)
	RM;	/* restoring after MMX */



	/*------------------------------------------------------------------------
	 *
	 *						processing memory in bytes
	 *								(write)
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(3)
		for(a = 0; a < BLOCK_SIZE; a++)
			p_b[a]=x;
	A_END(3)


	/*------------------------------------------------------------------------
	 *
	 *						processing memory in double words
	 *								(write)
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(4)
		for(a = 0; a < BLOCK_SIZE; a += sizeof(int))
			*(int*)((char *)p_dw + a)=x;
	A_END(4)


	/*------------------------------------------------------------------------
	 *
	 *						processing memory in quad words
	 *								(write)
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(5)
		for(a = 0; a < BLOCK_SIZE; a += 8)
			_mmx_cpy((char *)((char *)p_dw+a), buf);
	A_END(5)
	RM;	/* restoring after MMX */

	// output of the results
	Lx_OUT("byte vs dword READ ",Ax_GET(0),Ax_GET(1));
	Lx_OUT("byte vs qword READ ",Ax_GET(0),Ax_GET(2));

	Lx_OUT("byte vs dword WRITE",Ax_GET(3),Ax_GET(4));
	Lx_OUT("byte vs qword WRITE",Ax_GET(3),Ax_GET(5));

	return x+d;
}



