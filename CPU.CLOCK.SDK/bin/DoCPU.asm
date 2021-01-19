.686p
.MODEL FLAT

ITER    EQU     10                  ; number of iterations
CPU_PAGE_SIZE   EQU 4096

.DATA                               ; data segment
ALIGN 4
COUNTER DD      0                   ; loop counter
subtime	DD		0
time	DD		0
TICS    DD      0                   ; temporarily storing the timer counter
Zero    DD  0
RESULTLIST  DD  ITER DUP (0)        ; list of test results

_CODE segment page "CODE"           ; code segment

; ARG
__CPUID	proc
	PUSH	ESI
	MOV		ESI,[ESP+0Ch] ; <- store
	MOV		EAX,[ESP+08h]	; in
	PUSHA
	CPUID
	MOV		[ESI+00h],EAX
	MOV		[ESI+04h],EBX
	MOV		[ESI+08h],EDX
	MOV		[ESI+0Ch],ECX

	POPA
	POP		ESI
	RETN
__CPUID	endp




	
	

__stream_cpy    proc    
    MOV EAX, [ESP+8]
    MOV EDX, [ESP+4]
;         movaps  xmm0, oword ptr [eax]
    DB  0Fh
    DB  28h
    DB  00h

;         movntps  oword ptr [edx], xmm0
    DB  0Fh
    DB  2Bh
    DB  02h
    RET
__stream_cpy    endp

__mmx_cpy   proc
    MOV EAX, [ESP+8]
        MOV EDX, [ESP+4]

        ;MOVQ   mm0, [EAX]
        DB  0Fh
        DB  6Fh
        DB  00

        ;MOVQ   [EDX],mm0
        DB  0FH
        DB  7Fh
        DB  02h
        RET
__mmx_cpy   endp


__sfence    proc
    DB  0Fh
    DB  0AEh
    DB  0FFh
    ret
__sfence endp


__prefetchnta   proc
    MOV EAX,[ESP+4]
    DB  0Fh
    DB  18h
    DB  00h
    RET
__prefetchnta   endp


__prefetcht0    proc
    MOV EAX,[ESP+4]
    DB  0Fh
    DB  18h
    DB  08h
    RET
__prefetcht0    endp


__prefetcht1    proc
    MOV EAX,[ESP+4]
    DB  0Fh
    DB  18h
    DB  09h
    RET
__prefetcht1    endp

__prefetcht2    proc
    MOV EAX,[ESP+4]
    DB  0Fh
    DB  18h
    DB  0Ah ; <????
    RET
__prefetcht2    endp


; 
__mmx_memcpy    proc
    MOV EAX, [ESP+8]
    MOV EDX, [ESP+4]
    MOV ECX, [ESP+12]
    SHR ECX, 3

@Rool:
        ;MOVQ   mm0, [EAX]
        DB  0Fh
        DB  6Fh
        DB  00

        ;MOVQ   [EDX],mm0
        DB  0FH
        DB  7Fh
        DB  02h

    ADD EAX,8
    ADD EDX,8
    DEC ECX
    JNZ @Rool
    RET

__mmx_memcpy    endp

__ppp_memcpy    proc
    PUSH    ESI
    PUSH    EDI
    PUSH    ECX

    MOV EAX, [ESP+8+4*3]
    MOV EDX, [ESP+4+4*3]
    MOV ECX, [ESP+12+4*3]
    SHR ECX, 3

@Rool:
    MOV ESI,[EAX]
    MOV EDI,[EAX+4]
    MOV [EDX],ESI
    MOV [EDX+4],EDI 

        ADD EAX,8
        ADD EDX,8
    DEC ECX
    JNZ @Rool

    POP ECX
    POP     EDI
    POP ESI
    RET
__ppp_memcpy    endp

__null  proc
    RET
__null  endp


_DoCPU proc
    MOV     ESI, [ESP+4]        ; Loading our argument

BEGIN:  
    MOV     [COUNTER],0         ; Initializing the iterations counter

    
INCLUDE code.ini

    jmp testloop

    align 20h
TESTLOOP:                           ; loop check
	; // калибровка

	; 1)
    xor	eax,eax
	cpuid
	rdtsc
	mov subtime, eax
    xor	eax,eax
	cpuid
	rdtsc
	sub eax, subtime
	mov subtime, eax

	; 2)	
    xor	eax,eax
	cpuid
	rdtsc
	mov subtime, eax

    xor	eax,eax
	cpuid
	rdtsc
	sub eax, subtime
	mov subtime, eax

	; 3)
    xor	eax,eax
	cpuid
	rdtsc
	mov subtime, eax
    xor	eax,eax
	cpuid
	rdtsc
	sub eax, subtime
	mov subtime, eax 

	; ^^ !everything calibrated!
    xor	eax,eax
	cpuid 
	rdtsc 
	mov time, eax
	
    INCLUDE CODE.INC

    xor	eax,eax
	cpuid 
	rdtsc
	sub eax, time 
	sub eax, subtime
    MOV     [TICS],EAX          ; | saving results

	MOV     EDX,[COUNTER]       ; loop counter
	MOV     [RESULTLIST][EDX],EAX   ; saving results in table

	ADD     EDX,TYPE RESULTLIST     ; incrementing the counter
	MOV     [COUNTER],EDX           ; saving the timer counter
	CMP     EDX, ITER * (TYPE RESULTLIST)
	JB      TESTLOOP                ; repeating iteration
	MOV     EAX, offset RESULTLIST 
;	CLC
	RET
    
_DoCPU endp

aaaa:

retn

_A1 PROC
    PUSH    EBX
    PUSH    ECX

;    XOR EAX,EAX
 ;   CPUID
    MOV     EBX, [ESP+4+8]
        RDTSC
        MOV     [EBX],EAX                       

        POP ECX
        POP EBX
    RETN
_A1 ENDP

                            
_A2 PROC
    PUSH    EBX
    PUSH    ECX
;    XOR EAX,EAX
 ;   CPUID

        RDTSC
        MOV EBX, [ESP+4+8]
        SUB EAX, [EBX]

        POP ECX
    POP EBX
        RETN
_A2 ENDP


_CODE ends
END