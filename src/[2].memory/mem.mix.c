/* ---------------------------------------------------------------------------
 * @
 *			UTILITY FOR DEMONSTRATING THE EFFICIENCY OF
 *		COMBINING CALCULATIONS WITH MEMORY ACCESS
 *		===================================================
 *
 * Build 0x001	25.06.2002
--------------------------------------------------------------------------- */

// CONFIGURATION
#define BLOCK_SIZE		(4*M)			// memory block size

#include <DoCPU.h>
main()
{
	int *p1,*p2;
	float x = 0;
	int a, b, per, z = 1;

	// allocating memory
	p1 = (int *) _malloc32(BLOCK_SIZE);
	p2 = (int *) _malloc32(BLOCK_SIZE);

	// TITLE
	PRINT("= = = demonstration of the efficiency of combining caluclations with memory access = = =\n");
	PRINT_TITLE;

	/* -----------------------------------------------------------------------
	 *
	 *			combining caluclation of x+=cos(x) with memory read
	 *
	------------------------------------------------------------------------ */
	printf("### x+=cos(x) ###\n");
	
	// 1:4
	per=4; VVV;
	A_BEGIN(0)
		for(a = 0; a < BLOCK_SIZE; a += 4)
			z += *(int*)((char *)p1+a);

		for(a = 0; a < (BLOCK_SIZE/per); a++)
			x+=cos(x);
	A_END(0)

	VVV;
	A_BEGIN(1)
	for(a = 0; a < BLOCK_SIZE; a += per)
	{
		z += *(int*)((char *)p1+a);
		x+=cos(x);
	}
	A_END(1)

	per=8;VVV;
	A_BEGIN(2)
		for(a = 0; a < BLOCK_SIZE; a+=4)
			z += *(int*)((char *)p1+a);

		for(a=0; a < (BLOCK_SIZE/per); a++)
		 x+=cos(x);
	A_END(2)

	VVV;
	A_BEGIN(3)
	for(a = 0; a < BLOCK_SIZE; a += per)
	{
		
			z += *(int*)((char *)p1 + a);
			z += *(int*)((char *)p1 + a + 4);

		x+=cos(x);
	}
	A_END(3)


	per=16;VVV;
	A_BEGIN(4)
		for(a = 0; a < BLOCK_SIZE; a+=4)
			z += *(int*)((char *)p1+a);

		for(a=0; a < (BLOCK_SIZE/per); a++)
		 x+=cos(x);
	A_END(4)

	VVV;
	A_BEGIN(5)
	for(a = 0; a < BLOCK_SIZE; a += per)
	{
		z += *(int*)((char *)p1 + a);
		z += *(int*)((char *)p1 + a + 4);
		z += *(int*)((char *)p1 + a + 8);
		z += *(int*)((char *)p1 + a + 12);
		x+=cos(x);
	}
	A_END(5)

	per=32;VVV;
	A_BEGIN(6)
		for(a = 0; a < BLOCK_SIZE; a+=4)
			z += *(int*)((char *)p1+a);
		
		for(a=0; a < (BLOCK_SIZE/per); a++)
			x+=cos(x);
	A_END(6)

	VVV;
	A_BEGIN(7)
	for(a = 0; a < BLOCK_SIZE; a += per)
	{
		z += *(int*)((char *)p1 + a);
		z += *(int*)((char *)p1 + a + 4);
		z += *(int*)((char *)p1 + a + 8);
		z += *(int*)((char *)p1 + a + 12);
		z += *(int*)((char *)p1 + a + 16);
		z += *(int*)((char *)p1 + a + 20);
		z += *(int*)((char *)p1 + a + 24);
		z += *(int*)((char *)p1 + a + 28);

		x+=cos(x);
	}
	A_END(7)


	per=64;VVV;
	A_BEGIN(8)
		for(a = 0; a < BLOCK_SIZE; a+=4)
			z += *(int*)((char *)p1 + a);

		for(a=0; a < (BLOCK_SIZE/per); a++)
		 x+=cos(x);
	A_END(8)

	VVV;
	A_BEGIN(9)
	for(a = 0; a < BLOCK_SIZE; a += per)
	{
		z += *(int*)((char *)p1 + a);
		z += *(int*)((char *)p1 + a + 4);
		z += *(int*)((char *)p1 + a + 8);
		z += *(int*)((char *)p1 + a + 12);
		z += *(int*)((char *)p1 + a + 16);
		z += *(int*)((char *)p1 + a + 20);
		z += *(int*)((char *)p1 + a + 24);
		z += *(int*)((char *)p1 + a + 28);

		z += *(int*)((char *)p1 + a + 32);
		z += *(int*)((char *)p1 + a + 36);
		z += *(int*)((char *)p1 + a + 40);
		z += *(int*)((char *)p1 + a + 44);
		z += *(int*)((char *)p1 + a + 48);
		z += *(int*)((char *)p1 + a + 52);
		z += *(int*)((char *)p1 + a + 56);
		z += *(int*)((char *)p1 + a + 60);

		x+=cos(x);
	}
	A_END(9)

	Lx_OUT(" 1:4  ",Ax_GET(0),Ax_GET(1));
	Lx_OUT(" 1:8  ",Ax_GET(2),Ax_GET(3));
	Lx_OUT(" 1:16 ",Ax_GET(4),Ax_GET(5));
	Lx_OUT(" 1:32 ",Ax_GET(6),Ax_GET(7));
	Lx_OUT(" 1:64 ",Ax_GET(8),Ax_GET(9));


	per=128;VVV;
	A_BEGIN(0)
		for(a = 0; a < BLOCK_SIZE; a+=4)
			z += *(int*)((char *)p1 + a);

		for(a=0; a < (BLOCK_SIZE/per); a++)
		 x+=cos(x);
	A_END(0)

	VVV;
	A_BEGIN(1)
	for(a = 0; a < BLOCK_SIZE; a += per)
	{
		z += *(int*)((char *)p1 + a);
		z += *(int*)((char *)p1 + a + 4);
		z += *(int*)((char *)p1 + a + 8);
		z += *(int*)((char *)p1 + a + 12);
		z += *(int*)((char *)p1 + a + 16);
		z += *(int*)((char *)p1 + a + 20);
		z += *(int*)((char *)p1 + a + 24);
		z += *(int*)((char *)p1 + a + 28);

		z += *(int*)((char *)p1 + a + 32);
		z += *(int*)((char *)p1 + a + 36);
		z += *(int*)((char *)p1 + a + 40);
		z += *(int*)((char *)p1 + a + 44);
		z += *(int*)((char *)p1 + a + 48);
		z += *(int*)((char *)p1 + a + 52);
		z += *(int*)((char *)p1 + a + 56);
		z += *(int*)((char *)p1 + a + 60);

		z += *(int*)((char *)p1 + a + 64);
		z += *(int*)((char *)p1 + a + 68);
		z += *(int*)((char *)p1 + a + 72);
		z += *(int*)((char *)p1 + a + 76);
		z += *(int*)((char *)p1 + a + 80);
		z += *(int*)((char *)p1 + a + 84);
		z += *(int*)((char *)p1 + a + 88);
		z += *(int*)((char *)p1 + a + 92);

		z += *(int*)((char *)p1 + a + 96);
		z += *(int*)((char *)p1 + a + 100);
		z += *(int*)((char *)p1 + a + 104);
		z += *(int*)((char *)p1 + a + 108);
		z += *(int*)((char *)p1 + a + 112);
		z += *(int*)((char *)p1 + a + 116);
		z += *(int*)((char *)p1 + a + 120);
		z += *(int*)((char *)p1 + a + 124);

		x+=cos(x);
	}
	A_END(1)

	per=256;VVV;
	A_BEGIN(2)
		for(a = 0; a < BLOCK_SIZE; a+=4)
			z += *(int*)((char *)p1 + a);

		for(a=0; a < (BLOCK_SIZE/per); a++)
		 x+=cos(x);
	A_END(2)

	VVV;
	A_BEGIN(3)
	for(a = 0; a < BLOCK_SIZE; a += per)
	{
		z += *(int*)((char *)p1 + a);
		z += *(int*)((char *)p1 + a + 4);
		z += *(int*)((char *)p1 + a + 8);
		z += *(int*)((char *)p1 + a + 12);
		z += *(int*)((char *)p1 + a + 16);
		z += *(int*)((char *)p1 + a + 20);
		z += *(int*)((char *)p1 + a + 24);
		z += *(int*)((char *)p1 + a + 28);

		z += *(int*)((char *)p1 + a + 32);
		z += *(int*)((char *)p1 + a + 36);
		z += *(int*)((char *)p1 + a + 40);
		z += *(int*)((char *)p1 + a + 44);
		z += *(int*)((char *)p1 + a + 48);
		z += *(int*)((char *)p1 + a + 52);
		z += *(int*)((char *)p1 + a + 56);
		z += *(int*)((char *)p1 + a + 60);

		z += *(int*)((char *)p1 + a + 64);
		z += *(int*)((char *)p1 + a + 68);
		z += *(int*)((char *)p1 + a + 72);
		z += *(int*)((char *)p1 + a + 76);
		z += *(int*)((char *)p1 + a + 80);
		z += *(int*)((char *)p1 + a + 84);
		z += *(int*)((char *)p1 + a + 88);
		z += *(int*)((char *)p1 + a + 92);

		z += *(int*)((char *)p1 + a + 96);
		z += *(int*)((char *)p1 + a + 100);
		z += *(int*)((char *)p1 + a + 104);
		z += *(int*)((char *)p1 + a + 108);
		z += *(int*)((char *)p1 + a + 112);
		z += *(int*)((char *)p1 + a + 116);
		z += *(int*)((char *)p1 + a + 120);
		z += *(int*)((char *)p1 + a + 124);

		z += *(int*)((char *)p1 + a + 128);
		z += *(int*)((char *)p1 + a + 132);
		z += *(int*)((char *)p1 + a + 136);
		z += *(int*)((char *)p1 + a + 140);
		z += *(int*)((char *)p1 + a + 144);
		z += *(int*)((char *)p1 + a + 148);
		z += *(int*)((char *)p1 + a + 152);
		z += *(int*)((char *)p1 + a + 156);

		z += *(int*)((char *)p1 + a + 160);
		z += *(int*)((char *)p1 + a + 164);
		z += *(int*)((char *)p1 + a + 168);
		z += *(int*)((char *)p1 + a + 172);
		z += *(int*)((char *)p1 + a + 176);
		z += *(int*)((char *)p1 + a + 180);
		z += *(int*)((char *)p1 + a + 184);
		z += *(int*)((char *)p1 + a + 188);

		z += *(int*)((char *)p1 + a + 192);
		z += *(int*)((char *)p1 + a + 196);
		z += *(int*)((char *)p1 + a + 200);
		z += *(int*)((char *)p1 + a + 204);
		z += *(int*)((char *)p1 + a + 208);
		z += *(int*)((char *)p1 + a + 212);
		z += *(int*)((char *)p1 + a + 216);
		z += *(int*)((char *)p1 + a + 220);

		z += *(int*)((char *)p1 + a + 224);
		z += *(int*)((char *)p1 + a + 228);
		z += *(int*)((char *)p1 + a + 232);
		z += *(int*)((char *)p1 + a + 236);
		z += *(int*)((char *)p1 + a + 240);
		z += *(int*)((char *)p1 + a + 244);
		z += *(int*)((char *)p1 + a + 248);
		z += *(int*)((char *)p1 + a + 252);


		x+=cos(x);
	}
	A_END(3)

	VVV;
	A_BEGIN(4)
		// combining calculations with memory access
		// without unrolling the loop
		for(a = 0; a < BLOCK_SIZE; a += per)
		{
			for(b = 0; b < per; b += 4)
				z += *(int*)((char *)p1 + a + b);

			x+=cos(x);
		}
	A_END(4)

	// output of the results (finishing)
	Lx_OUT(" 1:128",Ax_GET(0),Ax_GET(1));
	Lx_OUT(" 1:256",Ax_GET(2),Ax_GET(3));
	Lx_OUT("*1:256",Ax_GET(2),Ax_GET(3));

	printf("##################\n\n");

return x+z;
}