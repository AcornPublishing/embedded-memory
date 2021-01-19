/*----------------------------------------------------------------------------
 *
 *					COMPARING ASSEMBLER AND c IMPLEMENTATIONS
 *	OF THE BUBBLE SORTING ALGORITHM
 *	=======================================================================
 *
---------------------------------------------------------------------------- */
// CONFIGURATION
#define N_MAX 1000							// quantity of numbers to sort
											// ATTENTION! do not get distracted
											// and do not take too many numbers
											// bubble sorting is too inefficient ;-)

#include <DoCPU.h>

extern void __cdecl asm_sort(int *src, int n);

void __cdecl c_sort(int *src, int n)
{
	int a; int t; int f;

	if (n<2) return;				// if there are less than two elements, they cannot be sorted!

	do{
		f = 0;						// reset the change flag

		// test all numbers one by one
		for (a = 1; a < n; a++)
			if (src[a-1] > src[a])	// are the two numbers sorted as required?
			{						// ...otherwise change their...
				t = src[a-1]; src[a-1] = src[a]; src[a] = t; //...places and...
				f = 1;				// set the change flag
			}
	
	} while(f);	// run the loop until in the last pass
				// there will be no changes in the order of the sorted sequence, - this means
				// that all numbers are already in correct order
}


main()
{
	int	a;
	int	*z1, *z2, *zx;

	PRINT("= = = Comparison of assembler and C implementations of the bubble sort function = = =\n");
	PRINT_TITLE;

	// allocating memory
	z1 = malloc(N_MAX*sizeof(int));
	z2 = malloc(N_MAX*sizeof(int));
	zx = malloc(N_MAX*sizeof(int));

	// generate N_MAX random numbers for sorting
	srand((unsigned)time(NULL));
	for (a = 0; a < N_MAX; a++) z1[a] = rand();

	// create identic conditions for both functions
	memcpy(z2, z1, N_MAX*sizeof(int));
	memcpy(zx, z1, N_MAX*sizeof(int));

	A_BEGIN(0);
		asm_sort(z1,N_MAX);
		memcpy(z1,zx,N_MAX*sizeof(int));
	A_END(0);

	A_BEGIN(1);
		c_sort(z2,N_MAX);
		memcpy(z2,zx,N_MAX*sizeof(int));
	A_END(1);

	// console output of the results
	A1_OUT("ASM SORT:");		A_LIST_ITER(0);
	Ax_OUT("C   SORT:",0,1);	A_LIST_ITER(1);


}