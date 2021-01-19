// function declarations 
extern void __cdecl _prefetchnta(char *x);
extern void __cdecl _prefetcht0(char *x);
extern void __cdecl _prefetcht1(char *x);
extern void __cdecl _mm_memcpy(char *dst, char *src, int len);
extern void __cdecl _sfence();
extern void __cdecl _mmx_cpy(char *dst, char *src);
extern void __cdecl _mmx_memcpy(char *dst, char *src, int len);
extern void __cdecl _ppp_memcpy(char *dst, char *src, int len);
extern void __cdecl _stream_cpy(char *dst, char *src);

#if defined (_MSC_VER) 
/*  -------------------------------------------------------------------------

            IMPLEMENTATION OF NEW COMMANDS OF P-III+ PROCESSORS

    ---------------------------------------------------------------------- */

__forceinline char*  __fastcall __prefetchnta(char *x)
{
    __asm
    {
        mov eax,[x]

        ; prefetchnta [eax]
        _emit   0xF
        _emit   0x18
        _emit   0x0
    }
    return x;
}


void __forceinline __fastcall __stream_cpy(char *dst,char *src)
{
    __asm
    {
        mov eax, [src]
        mov edx, [dst]

        ; movaps  xmm0, oword ptr [eax]
        _emit   0xF
        _emit   0x28
        _emit   0x0

        ; movntps  oword ptr [edx], xmm0
        _emit   0xF
        _emit   0x2B
        _emit   0x2
    }
}


void __forceinline __fastcall __mmx_cpy(char *dst,char *src)
{
    __asm
    {
        mov eax, [src]
        mov edx, [dst]

        ; movq  mm0, [eax]

        _emit   0xF
        _emit   0x6F
        _emit   0x0

        ; movq [edx], mm0
        _emit   0xF
        _emit   0x7F
        _emit   0x2
    }
}

void __forceinline __fastcall __sfence()
{
#if (_CPU>_PII)
    __asm
    {
        _emit 0xF
        _emit 0xAE
        _emit 0xFF
    }
#endif
}
#endif
