/*----------------------------------------------------------------------------
 *
 *						COMPARISON OF ASSEMBLER IMPLEMENTATION OF MEMORY COPYING FUNCTION
 *				with the same function implemented in C
 *				===========================================================
 *
---------------------------------------------------------------------------- */
// CONFIGURATION
#define N_MAX 16000						// number of int cells to be copied
										// ATTENTION: to prevent side effects
										// the copied memory must entirely
										// fit into L1 cache.
										// Thus maximum N_MAX
										// is equal to L1_CACHE_SIZE/8 bytes

#include <DoCPU.h>

extern void __cdecl asm_cpy(int *src, int *dst,int n);

// the function copies n double words from src to dst
// double words (rather than bytes!) are chosen,
// since bytes are processed less efficiently
// and optimal C implementation of the copy function
// in any case must manipulate with 32-bit
// data types
void __cdecl c_cpy(int *src, int *dst, int n)
{
	int a; int t;

	// copy
	// built-in memcpy function is not called,
	// because it actually is implemented in assembler
	// rather than in C.
	for (a = 0; a < n; a++) dst[a]=src[a];

	return;
}

main()
{
	int *z1, *z2;
	PRINT("= = = Comparin Assembler and C implementations of memcpy = = =\n");
	PRINT_TITLE;

	// allocating memory
	z1 = malloc(N_MAX*sizeof(int));
	z2 = malloc(N_MAX*sizeof(int));

	A_BEGIN(1);
		asm_cpy(z1, z2, N_MAX);
	A_END(1);

	A_BEGIN(2);
		c_cpy(z2, z1, N_MAX);
	A_END(2);

	// console output of the results
	A1_OUT("ASM CPY:");		A_LIST_ITER(1);
	Ax_OUT("C   CPY:",1,2);	A_LIST_ITER(2);

}