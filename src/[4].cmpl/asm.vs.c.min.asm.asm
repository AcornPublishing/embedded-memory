; /*--------------------------------------------------------------------------
;  *
;  *			ASSEMBLER IMPLEMENTATION OF THE PROCEDURE OF SEARCHING FOR  MINIMUM
;  *
; ------------------------------------------------------------------------- */
.386
.MODEL FLAT
.CODE

_asm_min	proc
	PUSH	ESI					; saving
	PUSH	EDI					;			registers

	MOV	ESI,[ESP+8+4]			; preparing src
	MOV	EDX,[ESP+8+8]			; preparing n

	CMP	EDX,2					; if n < 2 exit
	JB	@exit					;

	LODSD						; take the first double word and...
	MOV	EDI, EAX				; ...temporarily assign it as the smallest number

@for:
	LODSD						; take the next double word
	CMP	EDI, EAX				; if the smallest double word is smaller 
	JB	@next					; than this value, than everything is OK,
	MOV	EAX, EDI				; otherwise the title of
								; "the smallest" number is given to the next value
@next:
	DEC	EDX						; decrease the counter of processed double words by 1
	JNZ	@for					; repeat until all double words
								; are processed
		
@exit:
	POP	EDI						; restoring
	POP	ESI						;					registers

	ret							; returning
_asm_min	endp
END


