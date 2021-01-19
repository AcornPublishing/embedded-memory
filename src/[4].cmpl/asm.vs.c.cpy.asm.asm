; /*--------------------------------------------------------------------------
;  *
;  *			ASSEMBLER IMPLEMENTATION OF THE MEMORY COPYING PROCEDURE
;  *
; ------------------------------------------------------------------------- */
.386
.MODEL FLAT
.CODE

_asm_cpy	proc
	PUSH ESI						; \
	PUSH EDI						;  +- saving registers
	PUSH ECX						; /

	MOV	ESI, [ESP +  4 + 3*4]		; preparing src
	MOV	EDI, [ESP +  8 + 3*4]		; preparing dst
	MOV	ECX, [ESP + 12 + 3*4]		; preparing n

									; copying ;)
	REP MOVSD						; src ---n---> dst

	POP ECX							; \
	POP EDI							;  +- restoring registers
	POP ESI							; /

	ret								; returning from procedure
_asm_cpy endp
END


