/*----------------------------------------------------------------------------
 *
 *						COMPARISON OF ASSEMBLER IMPLEMENTATION OF
 *				THE FUNCTION SEARCHING FOR MINIMUM WITH THE SAME FUNCTION IMPLEMENTED IN C
 *				===========================================================
 *
---------------------------------------------------------------------------- */
#include <DoCPU.h>

// CONFIGURATION
#define N_MAX		10000					// quantity of numbers

extern void __cdecl asm_min(int *src, int n);

// Min function implemented in C
int __cdecl c_min(int *src, int n)
{
	int a;
	
	// temporarily make t the smallest number
	int t = *src;

	// test all numbers one by one
	for(a = 1; a < n; a++)
		// if the smaller value as compared to t is found,
		// it gets the status of "minimum"
		if (t > src[a]) t = src[a];

	return t;
}



main()
{

	int	a;
	int	r2;
	int	*z1, *z2;

	PRINT("= = = Comparison of Assembler and C implementations of the function searching for minimum = = =\n");
	PRINT_TITLE;

	// allocating memory
	z1 = malloc(N_MAX*sizeof(int));
	z2 = malloc(N_MAX*sizeof(int));

	// generate N_MAX arbitrary numbers to find the smallest one among them
	srand((unsigned)time(NULL));
	for (a=0;a<N_MAX;a++) z1[a]=rand();

	// to make the chances equal for both functions,
	// feed them with identical sets of numbers
	memcpy(z2, z1, N_MAX * sizeof(int));

	A_BEGIN(1);
		asm_min(z1,N_MAX);
	A_END(1);

	A_BEGIN(2);
		c_min(z2,N_MAX);
	A_END(2);

	// console output of the results
	A1_OUT("ASM MIN:");		A_LIST_ITER(1);
	Ax_OUT("C   MIN:",1,2);	A_LIST_ITER(2);
}