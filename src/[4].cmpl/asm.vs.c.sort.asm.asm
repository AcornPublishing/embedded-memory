; /*--------------------------------------------------------------------------
;  *
;  *			ASSEMBLER IMPLEMENTATION OF THE BUBBLE SORT PROCEDURE
;  *
; ------------------------------------------------------------------------- */
.386
.MODEL FLAT
.CODE

_asm_sort	proc
	MOV	EDX,[ESP+8]				; preparing n

	CMP	EDX,2					; are there at least two numbers for sorting?
	JB	@exit					; exit if the quantity of numbers is insufficient

	PUSH	ESI					; \
	PUSH	EBP					;  |- saving registers
	PUSH	EBX					; /
	
@while:							; do{
	MOV	ESI, [ESP+4+4*3]		;		preparing src
	MOV	EDX, [ESP+8+4*3]		;		preparing n

	XOR	EBP,EBP					;		reset the change flag

	NOP							;		for aligning...
	NOP							;						...next loop

@for:							;		do{
	MOV	EAX, [ESI]				;			take the number next in turn
	MOV	EBX, [ESI+4]			;			take the number next to the previous one

	CMP	EAX, EBX				;			if the numbers follow in the required order
	JAE	@next_for				;			everything is ok, otherwise
								;			change...
	MOV	EBP, EBX				;					...their places
	MOV	[ESI+4], EAX			;			to achieve the required
	MOV	[ESI],EBX				;			order
	
@next_for:
	ADD	ESI, 4					;			move the pointer to the next
								;			number

	DEC	EDX						;			decrease the number of unsorted
								;			ele ents (in this pass!) 
								;			by one
	JNZ	@for					;		} while (there are still numbers to sort);

	OR	EBP,EBP					;		are the numbers sorted completely?
	JNZ	@while					; } while(the numbers are not completely sorted);

	POP	EBX						; \
	POP	EBP						;  |- restore registers
	POP	ESI						; /

@exit:
	ret							; returning from procedure
_asm_sort endp

END

