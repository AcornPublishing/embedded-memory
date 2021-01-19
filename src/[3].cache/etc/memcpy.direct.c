/* 
		
		Example demonstrating speed of memory copying in both directions


*/
#include <DoCPU.h>

// CONFIGURATION
#define __READ_AFTER_WRITE	// uncomment this line to 
								// read memory block after copy

#define BLOCK_SIZE (1*M)		// block size
								// ATTENTION: the size of allocated memory is 
								// equal to 6*A_NITER*BLOCK_SIZE

#define	_PAGE_SIZE		4*K		// memory page size


main()
{
	int b,c;
	int *p1_src, *p2_src, *p3_src, *p1_dst, *p2_dst, *p3_dst;

	PRINT("\n_____memcpy direct test_____\n");
	PRINT_TITLE;

	// allocating memory
	p1_src = malloc(BLOCK_SIZE*A_NITER);
	p2_src = malloc(BLOCK_SIZE*A_NITER);
	p3_src = malloc(BLOCK_SIZE*A_NITER);
	p1_dst = malloc(BLOCK_SIZE*A_NITER);
	p2_dst = malloc(BLOCK_SIZE*A_NITER);
	p3_dst = malloc(BLOCK_SIZE*A_NITER);
	
	// for copying memory from the end
	//								position pointers to that location
	p2_src += BLOCK_SIZE*A_NITER;
	p2_dst += BLOCK_SIZE*A_NITER;

	p3_src+=BLOCK_SIZE*A_NITER;
	p3_dst+=BLOCK_SIZE*A_NITER;


	/*------------------------------------------------------------------------

								COPYING ---->

	----------------------------------------------------------------------- */
	A_BEGIN(0);
		// copying memory
		memcpy(p1_dst, p1_src, BLOCK_SIZE);

		// imitating block processing
		#ifdef __READ_AFTER_WRITE
			A_READ((p1_dst), BLOCK_SIZE);
		#endif

		p1_dst += BLOCK_SIZE;
		p1_src += BLOCK_SIZE;
	A_END(0);
	A1_OUT("-->");


	/* -----------------------------------------------------------------------

								COPYING <---------

	----------------------------------------------------------------------- */
	A_BEGIN(1);
		__asm{
			STD						; // reverse copying direction flag
			MOV	ESI, [p2_src]		; // loading the pointer to the source block
			MOV	EDI, [p2_dst]		; // loading the pointer to the target block
			MOV	ECX, BLOCK_SIZE/4	; // number of double words to be copied
			REP	MOVSD				; // copying block by double words...
									; // ... from end to the beginning
			MOV	[p2_src], ESI		; // saving the pointer to the last...
									; // ...copied byte of the source
			MOV	[p2_dst], EDI		; // saving the pointer to the last...
									; // ...copied byte of the target
			CLD						; // change the direction flag
									; // (this is required for C
									; //  library functions)
		}

		// imitating block processing
		#ifdef __READ_AFTER_WRITE
			A_READ(p2_dst, BLOCK_SIZE);
		#endif
	A_END(1);
	Ax_OUT("<--", 0, 1);


	/* -----------------------------------------------------------------

							OPTIMIZED VERSION

	-------------------------------------------------------------------- */

	A_BEGIN(2);
		b = BLOCK_SIZE;
		c = _PAGE_SIZE;
		
		// copy everything while at least one page fragment remains
		while(b)
		{
			// handling the situation when BLOCK_SIZE % _PAGE_SIZE != 0
			if (b < c) c = b;

			// moving the pointers backwards by the page length
			p3_dst -= c; p3_src -= c;

			// copying memory in direct direction
			memcpy(p3_dst, p3_src, c);

			// subtract number of copied bytes from BLOCK_SIZE
			b -= c;
		}

		// imitating block processing
		#ifdef __READ_AFTER_WRITE
			A_READ(p3_dst, BLOCK_SIZE);
		#endif
	A_END(2);

	Ax_OUT("<->", 0, 2);
}


