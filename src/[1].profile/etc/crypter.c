/*-----------------------------------------------------------------------------
 * [If you cannot read this text, you have chosen incorrect encoding]
 * 
 *   						CRYPTER fFOR THE PSWD EXAMPLE
 *							--------------------------
 *
 *		This encryption program will allow you to create your own encrypted
 * files intended for automatic decryption using 
 * pswd.exe
 *
 * Build 0x001 12.08.2002
----------------------------------------------------------------------------*/
#include <memory.h>
#include <stdio.h>

// max. length of each field
#define MAX_STR_LEN	102

// field delimiter
#define SEPARAT		(":")

// End of Line
#define EOLs		("\n")

/*----------------------------------------------------------------------------
 *
 *				procedure for encrypting the cyphertext using the cracked password
 *				-------------------------------------------------
 *	ARG:
 *		pswd		:	password
 *		data		:	text for encryption
 *
 *	RET:
 *		data		:	encrypted cyphertext
 *
 *	NOTE:			0	- invalid password (XOR with cyphertext gives 0)
 *					1	- ALL OK
 *		
-----------------------------------------------------------------------------*/
int Crypt(char *pswd, char *data)
{
	int a;
	int p = 0;		// pointer to the current position within the data being encrypted

	// * * * MAIN ENCRYPTION LOOP * * *
	do {
		// encrypting the current character
		data[p] ^= pswd[p % strlen(pswd)];
		if (!(data[p])) return 0;

		// proceeding with encryption of the next character
	} while(++p < strlen(data));
	return 1;
}


/*----------------------------------------------------------------------------
 *
 *				procedure for calculating the password checksum
 *				---------------------------------------------
 *	ARG:
 *		pswd		:	password
 *
 *	RET:			:	CRC of the current password
 *
 *	NOTE:
 *		none
-----------------------------------------------------------------------------*/
int CalculateCRC(char *pswd)
{
	int a;
	int x = -1;			// CRC calculation error

	for (a = 0; a < strlen(pswd);  a++)
	{
		x += *(int *) ((char *)pswd + a);
	}

	return x;
}

main()
{
	int		a;
	int 	x;
	int 	len;
	int 	CRC;
	char 	rem[MAX_STR_LEN];
	char 	dat[MAX_STR_LEN];
	char 	psw[MAX_STR_LEN];

	memset(psw, 0, 	MAX_STR_LEN);
	memset(dat, 0, 	MAX_STR_LEN);

	// data input from the console
	fputs("KPNC INTERNAL USAGE\n",stderr);
	fputs("= = = encrypting strings for the profiling example pswd = = =\n",stderr);
	fputs("enter the comment text:",stderr);fgets(rem,100,stdin); rem[strlen(rem)-1]=0;
    fputs("enter the text to be encrypted:",stderr);fgets(dat,100,stdin); dat[strlen(dat)-1]=0;
    fputs("enter the encryption password:",stderr);fgets(psw,100,stdin); psw[strlen(psw)-1]=0;

    if ((strlen(rem)==0) || (strlen(dat)==0) || (strlen(psw)==0))
    {
    	fputs("-ERR: Incorrect input! One or more rows are missing!\n",stderr);
    	return 0;
    }

    // calculating the password checksum
    CRC = CalculateCRC(psw);

    // encrypting the password
    if (!Crypt(psw, dat)) 
    {
    	fputs("-ERR: Invalid password - XOR with cyphertext gives zero\n",stderr);
    	return 0;
    }

    // console output of the results	
    fwrite(rem,		1,				strlen(rem),	stdout);
    fwrite(SEPARAT,	1, 				strlen SEPARAT, stdout);
    fwrite(&CRC,	sizeof(CRC),	1,  			stdout);
    fwrite(dat, 	1, 				strlen(dat)+1,	stdout);
    fwrite(EOLs, 	1, 				strlen EOLs,	stdout);


}
	

