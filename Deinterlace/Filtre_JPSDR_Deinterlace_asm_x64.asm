.code


;JPSDR_Deinterlace_Blend_Non_MMX_24 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_Non_MMX_24 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1:
	mov ecx,r9d
	xor rax,rax
loop_2:
	movzx edx,byte ptr[rdi+4*rax]
	movzx ebx,byte ptr[rsi+4*rax]
	add bx,dx
	shr bx,1
	mov r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+1]
	movzx ebx,byte ptr[rsi+4*rax+1]
	add bx,dx
	shr bx,1
	shl bx,8
	or r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+2]
	movzx ebx,byte ptr[rsi+4*rax+2]
	add bx,dx
	shr bx,1
	shl ebx,16
	or ebx,r13d
	mov dword ptr[r8+4*rax],ebx
	inc rax
	loop loop_2
	add rsi,r11
	add rdi,r11
	add r8,r12
	dec r10d
	jnz short loop_1
	
fin_:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Non_MMX_24 endp


;JPSDR_Deinterlace_Blend_Non_MMX_32 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_Non_MMX_32 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1_i:
	mov ecx,r9d
	xor rax,rax
loop_2_i:
	movzx edx,byte ptr[rdi+4*rax]
    movzx ebx,byte ptr[rsi+4*rax]
	add bx,dx
	shr bx,1
	mov r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+1]
	movzx ebx,byte ptr[rsi+4*rax+1]
	add bx,dx
	shr bx,1
	shl bx,8
	or r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+2]
	movzx ebx,byte ptr[rsi+4*rax+2]
	add bx,dx
	shr bx,1
	shl ebx,16
	or r13d,ebx
	movzx edx,byte ptr[rdi+4*rax+3]
	movzx ebx,byte ptr[rsi+4*rax+3]
	add bx,dx
	shr bx,1
	shl ebx,24
	or ebx,r13d
	mov dword ptr[r8+4*rax],ebx
	inc rax
	loop loop_2_i
	add rsi,r11
	add rdi,r11
	add r8,r12
	dec r10d
	jnz short loop_1_i
	
fin_i:
	pop r13
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Non_MMX_32 endp


;JPSDR_Deinterlace_Blend_Non_MMX_8 proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_Non_MMX_8 proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov r12,dst_pitch
	xor rcx,rcx

loop_1_j:
	mov ecx,r9d
	xor rax,rax
loop_2_j:
    movzx ebx,byte ptr[rsi+rax]
	movzx edx,byte ptr[rdi+rax]
	add bx,dx
	shr bx,1
	mov byte ptr[r8+rax],bl
	inc rax
	loop loop_2_j
	add rsi,r11
	add rdi,r11
	add r8,r12
	dec r10d
	jnz short loop_1_j
	
fin_j:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Non_MMX_8 endp


;JPSDR_Deinterlace_YadifAbsDiffAvg_SSE proc src_a:dword,src_b:dword,dst_abs:dword,dst_avg:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; src_b = rdx
; dst_abs = r8
; dst_avg = r9

JPSDR_Deinterlace_YadifAbsDiffAvg_SSE proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	.endprolog
	
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10d,w
	mov r11d,h
	mov r12,src_pitch
	mov r13,dst_pitch
	mov rbx,16
	xor rcx,rcx
	
loop_1_Yadif1:
	xor rax,rax
	mov ecx,r10d
loop_2_Yadif1:
	movdqa xmm0,XMMWORD ptr[rsi+rax]		; xmm0=src_a
	movdqa xmm1,XMMWORD ptr[rdi+rax]		; xmm1=src_b
	movdqa xmm2,xmm0
	movdqa xmm3,xmm0
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	pminub xmm3,xmm1			; xmm3=min(src_a,src_b)
	pavgb xmm0,xmm1				; xmm0=(scr_a+src_b+1)>>1
	psubb xmm2,xmm3				; xmm2=xmm2-xmm3=abs(src_a-src_b)
	movdqa XMMWORD ptr[r9+rax],xmm0		; dst_avg=xmm0=(scr_a+src_b+1)>>1
	movdqa XMMWORD ptr[r8+rax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add rax,rbx
	loop loop_2_Yadif1
	add rsi,r12
	add rdi,r12
	add r8,r13
	add r9,r13
	dec r11d
	jnz short loop_1_Yadif1

	pop r13
	pop r12	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_Deinterlace_YadifAbsDiffAvg_SSE endp


;JPSDR_Deinterlace_YadifAbsDiffAvg_AVX proc src_a:dword,src_b:dword,dst_abs:dword,dst_avg:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; src_b = rdx
; dst_abs = r8
; dst_avg = r9

JPSDR_Deinterlace_YadifAbsDiffAvg_AVX proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]
h equ dword ptr[rbp+72]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	.endprolog
	
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10d,w
	mov r11d,h
	mov r12,src_pitch
	mov r13,dst_pitch
	mov rbx,16
	xor rcx,rcx
	
loop_1_Yadif1_AVX:
	xor rax,rax
	mov ecx,r10d
loop_2_Yadif1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]		; xmm0=src_a
	vmovdqa xmm1,XMMWORD ptr[rdi+rax]		; xmm1=src_b
	vpmaxub xmm2,xmm0,xmm1			; xmm2=max(src_a,src_b)
	vpminub xmm3,xmm0,xmm1			; xmm3=min(src_a,src_b)
	vpavgb xmm0,xmm0,xmm1				; xmm0=(scr_a+src_b+1)>>1
	vpsubb xmm2,xmm2,xmm3				; xmm2=xmm2-xmm3=abs(src_a-src_b)
	vmovdqa XMMWORD ptr[r9+rax],xmm0		; dst_avg=xmm0=(scr_a+src_b+1)>>1
	vmovdqa XMMWORD ptr[r8+rax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add rax,rbx
	loop loop_2_Yadif1_AVX
	add rsi,r12
	add rdi,r12
	add r8,r13
	add r9,r13
	dec r11d
	jnz short loop_1_Yadif1_AVX

	pop r13
	pop r12	
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret
	
JPSDR_Deinterlace_YadifAbsDiffAvg_AVX endp


;JPSDR_Deinterlace_YadifAbsDiff_SSE proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; scr_b = rdx
; dst = r8
; src_pitch = r9

JPSDR_Deinterlace_YadifAbsDiff_SSE proc public frame

dst_pitch equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	.endprolog
	
	mov rsi,rcx
	mov rbx,rdx
	mov rdi,r8
	mov r10,dst_pitch
	mov r8,16
	mov edx,w
	mov r11d,h
	xor rcx,rcx
	
loop_1_Yadif2:
	xor rax,rax
	mov ecx,edx
loop_2_Yadif2:
	movdqa xmm0,XMMWORD ptr[rsi+rax]		; xmm0=src_a
	movdqa xmm1,XMMWORD ptr[rbx+rax]		; xmm1=src_b
	movdqa xmm2,xmm0
	pminub xmm0,xmm1			; xmm0=min(src_a,src_b)	
	pmaxub xmm2,xmm1			; xmm2=max(src_a,src_b)
	psubb xmm2,xmm0				; xmm2=xmm2-xmm0=abs(src_a-src_b)
	movdqa XMMWORD ptr[rdi+rax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add rax,r8
	loop loop_2_Yadif2
	add rsi,r9
	add rbx,r9
	add rdi,r10
	dec r11d
	jnz short loop_1_Yadif2

	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret
	
JPSDR_Deinterlace_YadifAbsDiff_SSE endp
	

;JPSDR_Deinterlace_YadifAbsDiff_AVX proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; scr_b = rdx
; dst = r8
; src_pitch = r9

JPSDR_Deinterlace_YadifAbsDiff_AVX proc public frame

dst_pitch equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	.endprolog
	
	mov rsi,rcx
	mov rbx,rdx
	mov rdi,r8
	mov r10,dst_pitch
	mov r8,16
	mov edx,w
	mov r11d,h
	xor rcx,rcx
	
loop_1_Yadif2_AVX:
	xor rax,rax
	mov ecx,edx
loop_2_Yadif2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]		; xmm0=src_a
	vmovdqa xmm1,XMMWORD ptr[rbx+rax]		; xmm1=src_b
	vpmaxub xmm2,xmm0,xmm1			; xmm2=max(src_a,src_b)
	vpminub xmm0,xmm0,xmm1			; xmm0=min(src_a,src_b)	
	vpsubb xmm2,xmm2,xmm0				; xmm2=xmm2-xmm0=abs(src_a-src_b)
	vmovdqa XMMWORD ptr[rdi+rax],xmm2		; dst_abs=xmm2=abs(src_a-src_b)
	add rax,r8
	loop loop_2_Yadif2_AVX
	add rsi,r9
	add rbx,r9
	add rdi,r10
	dec r11d
	jnz short loop_1_Yadif2_AVX

	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret
	
JPSDR_Deinterlace_YadifAbsDiff_AVX endp


;JPSDR_Deinterlace_YadifAvg_SSE proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; scr_b = rdx
; dst = r8
; src_pitch = r9

JPSDR_Deinterlace_YadifAvg_SSE proc public frame
	
dst_pitch equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rbx,rdx
	mov rdi,r8
	mov r10,dst_pitch
	mov r8,16
	mov edx,w
	mov r11d,h
	xor rcx,rcx
		
loop_1_Yadif3:
	xor rax,rax
	mov ecx,edx
loop_2_Yadif3:
	movdqa xmm0,XMMWORD ptr[rsi+rax]		; xmm0=src_a
	pavgb xmm0,XMMWORD ptr[rbx+rax]		; xmm0=(src_a+src_b+1)>>1
	movdqa XMMWORD ptr[rdi+rax],xmm0		; dst_abs=xmm0=(src_a+src_b+1)>>1
	add rax,r8
	loop loop_2_Yadif3
	add rsi,r9
	add rbx,r9
	add rdi,r10
	dec r11d
	jnz short loop_1_Yadif3

	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret
	
JPSDR_Deinterlace_YadifAvg_SSE endp


;JPSDR_Deinterlace_YadifAvg_AVX proc src_a:dword,src_b:dword,dst:dword,src_pitch:dword,dst_pitch:dword,w:dword,h:dword
; src_a = rcx
; scr_b = rdx
; dst = r8
; src_pitch = r9

JPSDR_Deinterlace_YadifAvg_AVX proc public frame
	
dst_pitch equ qword ptr[rbp+48]
w equ dword ptr[rbp+56]
h equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx
	mov rbx,rdx
	mov rdi,r8
	mov r10,dst_pitch
	mov r8,16
	mov edx,w
	mov r11d,h
	xor rcx,rcx
		
loop_1_Yadif3_AVX:
	xor rax,rax
	mov ecx,edx
loop_2_Yadif3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]		; xmm0=src_a
	vpavgb xmm0,xmm0,XMMWORD ptr[rbx+rax]		; xmm0=(src_a+src_b+1)>>1
	vmovdqa XMMWORD ptr[rdi+rax],xmm0		; dst_abs=xmm0=(src_a+src_b+1)>>1
	add rax,r8
	loop loop_2_Yadif3_AVX
	add rsi,r9
	add rbx,r9
	add rdi,r10
	dec r11d
	jnz short loop_1_Yadif3_AVX

	pop rbx
	pop rsi
	pop rdi
	pop rbp
	
	ret
	
JPSDR_Deinterlace_YadifAvg_AVX endp


;JPSDR_Deinterlace_Blend_SSE_3_A proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_SSE_3_A proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	.endprolog
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e:
	xor rax,rax
	mov ecx,r9d
loop_2_e:
	movdqa xmm0,XMMWORD ptr[rsi+rax]
	pavgb xmm0,XMMWORD ptr[rdi+rax]
	movdqa XMMWORD ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e

	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e
	
fin_e:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_SSE_3_A endp


;JPSDR_Deinterlace_Blend_AVX_3_A proc src1:dword,src2:dword,dst:dword,w:dword,h:dword,
;	src_pitch:dword,dst_pitch:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_Deinterlace_Blend_AVX_3_A proc public frame

h equ dword ptr[rbp+48]
src_pitch equ qword ptr[rbp+56]
dst_pitch equ qword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	.endprolog
	
	mov rsi,rcx
	mov rdi,rdx
	mov r10d,h
	mov r11,src_pitch
	mov rdx,dst_pitch
	mov rbx,16
	xor rcx,rcx

loop_1_e_AVX:
	xor rax,rax
	mov ecx,r9d
loop_2_e_AVX:
	vmovdqa xmm0,XMMWORD ptr[rsi+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdi+rax]
	vmovdqa XMMWORD ptr[r8+rax],xmm0
	add rax,rbx
	loop loop_2_e_AVX

	add rsi,r11
	add rdi,r11
	add r8,rdx
	dec r10d
	jnz short loop_1_e_AVX
	
fin_e_AVX:
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_AVX_3_A endp


;JPSDR_Deinterlace_Blend_Tri_SSE_3_A proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_A proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2

	add rdi,r11

loop_1_g_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2

	xor rax,rax
	mov ecx,r8d
loop_3_g_2:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2
	
fin_g_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A endp


;JPSDR_Deinterlace_Blend_Tri_AVX_3_A proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_AVX_3_A proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_AVX

	add rdi,r11

loop_1_g_2_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_AVX
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_AVX

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_AVX
	
fin_g_2_AVX:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_AVX_3_A endp


;JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_1:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_1

	add rdi,r11

loop_1_g_2_1:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_1:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_1

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_1

fin_g_2_1:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_a endp


;JPSDR_Deinterlace_Blend_Tri_AVX_3_A_a proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_AVX_3_A_a proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
	xor rax,rax
	mov ecx,r8d	
loop_0_g_2_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_0_g_2_1_AVX

	add rdi,r11

loop_1_g_2_1_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_1_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_1_AVX

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_1_AVX

fin_g_2_1:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_AVX_3_A_a endp


;JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_2:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_2:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_2
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_2
	
fin_g_2_2:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_b endp


;JPSDR_Deinterlace_Blend_Tri_AVX_3_A_b proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_AVX_3_A_b proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_2_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_2_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_2_AVX
	
	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_2_AVX
	
fin_g_2_2_AVX:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_AVX_3_A_b endp


;JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_3:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_3:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rdx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_3

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_3

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_3:
	movdqa xmm0,XMMWORD ptr[rbx+rax]
	pavgb xmm0,XMMWORD ptr[rsi+rax]
	movdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_3
	
fin_g_2_3:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_SSE_3_A_c endp


;JPSDR_Deinterlace_Blend_Tri_AVX_3_A_c proc src:dword,dst:dword,w:dword,h:dword,src_pitch:dword,
;	dst_pitch:dword
; src = rcx
; dst = rdx
; w = r8d
; h = r9d

JPSDR_Deinterlace_Blend_Tri_AVX_3_A_c proc public frame

src_pitch equ qword ptr[rbp+48]
dst_pitch equ qword ptr[rbp+56]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rdi
	.pushreg rdi
	push rsi
	.pushreg rsi	
	push rbx
	.pushreg rbx
	push r12
	.pushreg r12
	.endprolog

	mov r10,src_pitch
	mov r11,dst_pitch
	mov r12,16
	mov rbx,rcx	
	mov rsi,rcx
	mov rdi,rdx
	add rsi,r10
	mov rdx,rsi
	add rdx,r10			;rbx=ln-1 	rsi=ln	rdx=ln+1
	
	xor rcx,rcx
		
loop_1_g_2_3_AVX:
	xor rax,rax
	mov ecx,r8d
loop_2_g_2_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rdx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_2_g_2_3_AVX

	add rsi,r10
	add rbx,r10
	add rdx,r10
	add rdi,r11
	dec r9d
	jnz short loop_1_g_2_3_AVX

	xor rax,rax
	mov ecx,r8d
loop_3_g_2_3_AVX:
	vmovdqa xmm0,XMMWORD ptr[rbx+rax]
	vpavgb xmm0,xmm0,XMMWORD ptr[rsi+rax]
	vmovdqa XMMWORD ptr[rdi+rax],xmm0
	add rax,r12
	loop loop_3_g_2_3_AVX
	
fin_g_2_3_AVX:
	pop r12
	pop rbx
	pop rsi
	pop rdi
	pop rbp

	ret

JPSDR_Deinterlace_Blend_Tri_AVX_3_A_c endp
end





