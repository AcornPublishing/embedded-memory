/* ---------------------------------------------------------------------------
 *
 *			UTILITY FOR COMPARING THE EFFICIENCY OF c AND MFC STRINGS
 *			==================================================
 *
 * Build 0x001 27.06.2002
--------------------------------------------------------------------------- */
// configuration
#define STR_LEN	(4*M)					// string length

#include <afx.h>
#include <DoCPU.H>

main()
{
	int a,x=0;
	CString s0,s1,s2;
	char *p1, *p2, *p3;

	PRINT("= = = comparing the efficiency of C and MFC strings = = =\n");
	PRINT_TITLE;

	// allocating memory
	p1 = (char *)malloc(STR_LEN);
	p2 = (char *)malloc(STR_LEN);
	p3 = (char *)malloc(STR_LEN*2);

	// initializing C strings
	memset(p1,'x',STR_LEN);	p1[STR_LEN]=0;
	memset(p2,'x',STR_LEN);	p2[STR_LEN]=0;

	// initializing MFC strings
	s0.Insert(0,p1);
	s1.Insert(0,p2);

	/*------------------------------------------------------------------------

								STRING COMPARISON

	----------------------------------------------------------------------- */
	
	// comparing MFC strings
	A_BEGIN(0)
		if (s1==s0) x++;
	A_END(0)

	// comparing C strings
	A_BEGIN(1)
		x+=strcmp(p1,p2);
	A_END(1)
	Lx_OUT("strcmp",Ax_GET(1),Ax_GET(0));

	/*------------------------------------------------------------------------

								STRING CONCATENATION

	----------------------------------------------------------------------- */
	// concatenating MFC string
	A_BEGIN(0)
		s2=s1+s0;
	A_END(0)

	// concatenating C strings
	A_BEGIN(1)
		p3[0]=0;
		strcat(p3,p1);
		strcat(p3,p2);
	A_END(1)
	Lx_OUT("strcat",Ax_GET(1),Ax_GET(0));

	/*------------------------------------------------------------------------

							CALCULATING STRING LENGTH

	----------------------------------------------------------------------- */

	// calculating the length of MFC strings
	A_BEGIN(0)
		x+=s0.GetLength();
	A_END(0)

	// calculating the length of C strings
	A_BEGIN(1)
		x+=strlen(s0);
	A_END(1)
	Lx_OUT("strlen",Ax_GET(1),Ax_GET(0));


	return 0;
}
