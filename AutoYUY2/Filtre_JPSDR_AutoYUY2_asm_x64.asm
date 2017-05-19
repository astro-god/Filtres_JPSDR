.data

align 16

uw_2 word 8 dup(2)
uw_3 word 8 dup(3)
uw_4 word 8 dup(4)
uw_5 word 8 dup(5)
uw_7 word 8 dup(7)


.code


;JPSDR_AutoYUY2_Convert420_to_YUY2_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_AutoYUY2_Convert420_to_YUY2_1 proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push rbx
	.pushreg rbx
	.endprolog

	mov rsi,rcx				; rsi=src_y
	xor rcx,rcx
	mov rdi,r9				; rdi=dst
	mov rbx,r8
	mov ecx,w
	cld

SSE2_1_a:
	mov al,byte ptr[rsi+1]		;al=y2
	mov ah,byte ptr[rbx]		;ah=v
	inc rbx
	shl eax,16
	lodsw				;al=y1 ah=y2
	mov ah,byte ptr[rdx]		;ah=u
	inc rdx
	stosd
	loop SSE2_1_a
	
	pop rbx
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_1 endp



;JPSDR_AutoYUY2_Convert420_to_UYVY_1 proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_AutoYUY2_Convert420_to_UYVY_1 proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	.endprolog

	mov rsi,rcx				; rsi=src_y
	xor rcx,rcx
	mov rdi,r9				; rdi=dst
	mov ecx,w
	cld
		
SSE2_1_b:
	lodsw						;al=y1 ah=y2
	mov al,byte ptr[r8]		;al=v
	inc r8
	shl eax,16
	mov ah,byte ptr[rsi-2]		;ah=y1
	mov al,byte ptr[rdx]		;al=u
	inc rdx
	stosd
	loop SSE2_1_b
	
	pop rdi
	pop rsi
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_1 endp


;JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push r12
	.pushreg r12	
	push r13
	.pushreg r13
	.endprolog

	mov r10,rcx
	xor rcx,rcx
	mov ecx,w
	xor rax,rax
	mov r11,32
	mov r12,2
	mov r13,16
	
	shr ecx,1
	jz short _SSE2_1b_b
	
_SSE2_1b_a:
	movq xmm1,qword ptr[rdx+4*rax]		;00000000UUUUUUUU
	movq xmm0,qword ptr[r8+4*rax]		;00000000VVVVVVVV
	movdqa xmm2,XMMWORD ptr[r10+8*rax]		;YYYYYYYYYYYYYYYY
	punpcklbw xmm1,xmm0					;VUVUVUVUVUVUVUVU
	movdqa xmm3,xmm2
	add rax,r12
	punpcklbw xmm2,xmm1     			;VYUYVYUYVYUYVYUY
	punpckhbw xmm3,xmm1     			;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[r9],xmm2
	movdqa XMMWORD ptr[r9+r13],xmm3
	add r9,r11
	loop _SSE2_1b_a
	
_SSE2_1b_b:
	mov ecx,w
	and ecx,1
	jz short _SSE2_1b_c
	
	movd xmm1,dword ptr[rdx+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r8+4*rax]		;000000000000VVVV
	movq xmm2,qword ptr[r10+8*rax]		;00000000YYYYYYYY
	punpcklbw xmm1,xmm0					;00000000VUVUVUVU
	punpcklbw xmm2,xmm1     			;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[r9],xmm2	
	
_SSE2_1b_c:	
	pop r13
	pop r12
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_1b endp


;JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b proc src_y:dword,src_u:dword,src_v:dword,dst:dword,w:dword
; src_y = rcx
; src_u = rdx
; src_v = r8
; dst = r9

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b proc public frame

w equ dword ptr[rbp+48]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push r12
	.pushreg r12	
	push r13
	.pushreg r13	
	.endprolog

	mov r10,rcx
	xor rcx,rcx
	mov ecx,w
	xor rax,rax
	mov r11,32
	mov r12,2
	mov r13,16
	
	shr ecx,1
	jz short _SSE2_0b_b
	
_SSE2_0b_a:
	movq xmm1,qword ptr[rdx+4*rax]		;00000000UUUUUUUU
	movq xmm0,qword ptr[r8+4*rax]		;00000000VVVVVVVV
	movdqa xmm2,XMMWORD ptr[r10+8*rax]		;YYYYYYYYYYYYYYYY
	punpcklbw xmm1,xmm0					;VUVUVUVUVUVUVUVU
	add rax,r12
	movdqa xmm3,xmm1
	punpcklbw xmm1,xmm2     			;YVYUYVYUYVYUYVYU
	punpckhbw xmm3,xmm2     			;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[r9],xmm1
	movdqa XMMWORD ptr[r9+r13],xmm3
	add r9,r11
	loop _SSE2_0b_a
	
_SSE2_0b_b:
	mov ecx,w
	and ecx,1
	jz short _SSE2_0b_c
	
	movd xmm1,dword ptr[rdx+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r8+4*rax]		;000000000000VVVV
	movq xmm2,qword ptr[r10+8*rax]		;00000000YYYYYYYY
	punpcklbw xmm1,xmm0					;00000000VUVUVUVU
	punpcklbw xmm1,xmm2     			;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[r9],xmm1
	
_SSE2_0b_c:	
	pop r13
	pop r12
	pop rbp

	ret

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_1b endp


;JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b proc public frame

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push r12
	.pushreg r12
	push r13
	.pushreg r13	
	sub rsp,32
	.allocstack 32
	movdqa XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqa XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog
	
	pxor xmm7,xmm7
	
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov rdi,dst
	mov r11,src2_v
	xor rcx,rcx
	mov rdx,32
	mov r12,2
	mov r13,16
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_4
	movdqa xmm5,XMMWORD ptr uw_3
	movdqa xmm4,XMMWORD ptr uw_5
	
	xor rax,rax
	shr ecx,1
	jz _SSE2_2b_b

_SSE2_2b_a:
	movq xmm0,qword ptr[r10+4*rax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[r9+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	movq xmm2,qword ptr[r8+4*rax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[r11+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	pmullw xmm2,xmm4
	pmullw xmm3,xmm4	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	movdqa xmm2,XMMWORD ptr[rsi+8*rax]		;YYYYYYYYYYYYYYYY
	psraw xmm0,3
	psraw xmm1,3
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm3,xmm2
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add rax,r12
	punpcklbw xmm2,xmm0				;VYUYVYUYVYUYVYUY
	punpckhbw xmm3,xmm0				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[rdi],xmm2
	movdqa XMMWORD ptr[rdi+r13],xmm3
	add rdi,rdx
	
	dec ecx
	jnz _SSE2_2b_a

_SSE2_2b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_2b_c
	
	movd xmm1,dword ptr[r10+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r9+4*rax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	movd xmm2,dword ptr[r8+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r11+4*rax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	

	pmullw xmm1,xmm5
	pmullw xmm2,xmm4
	paddsw xmm1,xmm6
	movq xmm0,qword ptr[rsi+8*rax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,3
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm0,xmm1     		;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[rdi],xmm0
		
_SSE2_2b_c:		
	movdqa xmm7,XMMWORD ptr[rsp+16]
	movdqa xmm6,XMMWORD ptr[rsp]
	add rsp,32
	pop r13
	pop r12	
	pop rdi
	pop rsi
	pop rbp

	ret		

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_2b endp


;JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b proc public frame

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push r12
	.pushreg r12
	push r13
	.pushreg r13		
	sub rsp,32
	.allocstack 32
	movdqa XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqa XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog
	
	pxor xmm7,xmm7
	
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov rdi,dst
	mov r11,src2_v
	xor rcx,rcx
	mov rdx,32
	mov r12,2
	mov r13,16
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_4
	movdqa xmm5,XMMWORD ptr uw_3
	movdqa xmm4,XMMWORD ptr uw_5
	
	xor rax,rax
	shr ecx,1
	jz _SSE2_6b_b

_SSE2_6b_a:
	movq xmm0,qword ptr[r10+4*rax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[r9+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	movq xmm2,qword ptr[r8+4*rax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[r11+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	pmullw xmm2,xmm4
	pmullw xmm3,xmm4	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,3
	psraw xmm1,3
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm2,XMMWORD ptr[rsi+8*rax]		;YYYYYYYYYYYYYYYY
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add rax,r12
	movdqa xmm3,xmm0
	punpcklbw xmm0,xmm2				;YVYUYVYUYVYUYVYU
	punpckhbw xmm3,xmm2				;YVYUYVYUYVYUYVYU
	movdqa XMMWORD ptr[rdi],xmm0
	movdqa XMMWORD ptr[rdi+r13],xmm3
	add rdi,rdx
	
	dec ecx
	jnz _SSE2_6b_a

_SSE2_6b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_6b_c
	
	movd xmm1,dword ptr[r10+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r9+4*rax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	movd xmm2,dword ptr[r8+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r11+4*rax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	

	pmullw xmm1,xmm5
	pmullw xmm2,xmm4
	paddsw xmm1,xmm6
	movq xmm0,qword ptr[rsi+8*rax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,3
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm1,xmm0     		;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[rdi],xmm1
		
_SSE2_6b_c:		
	movdqa xmm7,XMMWORD ptr[rsp+16]
	movdqa xmm6,XMMWORD ptr[rsp]
	add rsp,32
	pop r13
	pop r12		
	pop rdi
	pop rsi
	pop rbp

	ret		

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_2b endp


;JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b proc public frame

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push r12
	.pushreg r12
	push r13
	.pushreg r13	
	sub rsp,32
	.allocstack 32
	movdqa XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqa XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog

	pxor xmm7,xmm7
	
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov rdi,dst
	mov r11,src2_v
	xor rcx,rcx
	mov rdx,32
	mov r12,2
	mov r13,16
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_4
	movdqa xmm5,XMMWORD ptr uw_7
	
	xor rax,rax
	shr ecx,1
	jz short _SSE2_3b_b

_SSE2_3b_a:
	movq xmm0,qword ptr[r10+4*rax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[r9+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	movq xmm2,qword ptr[r8+4*rax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[r11+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	movdqa xmm4,XMMWORD ptr[rsi+8*rax]		;YYYYYYYYYYYYYYYY
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,3
	psraw xmm1,3
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm2,xmm4
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add rax,r12
	punpcklbw xmm2,xmm0				;VYUYVYUYVYUYVYUY
	punpckhbw xmm4,xmm0				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[rdi],xmm2
	movdqa XMMWORD ptr[rdi+r13],xmm4
	add rdi,rdx
	
	loop _SSE2_3b_a

_SSE2_3b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_3b_c
	
	movd xmm1,dword ptr[r10+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r9+4*rax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	movd xmm2,dword ptr[r8+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r11+4*rax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	

	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	movq xmm0,qword ptr[rsi+8*rax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,3
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm0,xmm1     		;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[rdi],xmm0
		
_SSE2_3b_c:		
	movdqa xmm7,XMMWORD ptr[rsp+16]
	movdqa xmm6,XMMWORD ptr[rsp]
	add rsp,32
	pop r13
	pop r12	
	pop rdi
	pop rsi
	pop rbp

	ret		

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_3b endp


;JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b proc public frame

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push r12
	.pushreg r12
	push r13
	.pushreg r13	
	sub rsp,32
	.allocstack 32
	movdqa XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqa XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog

	pxor xmm7,xmm7
	
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov rdi,dst
	mov r11,src2_v
	xor rcx,rcx
	mov rdx,32
	mov r12,2
	mov r13,16
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_4
	movdqa xmm5,XMMWORD ptr uw_7
	
	xor rax,rax
	shr ecx,1
	jz short _SSE2_7b_b

_SSE2_7b_a:
	movq xmm0,qword ptr[r10+4*rax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[r9+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	movq xmm2,qword ptr[r8+4*rax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[r11+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,3
	psraw xmm1,3
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm4,XMMWORD ptr[rsi+8*rax]		;YYYYYYYYYYYYYYYY	
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add rax,r12
	movdqa xmm2,xmm0
	punpcklbw xmm0,xmm4				;VYUYVYUYVYUYVYUY
	punpckhbw xmm2,xmm4				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[rdi],xmm0
	movdqa XMMWORD ptr[rdi+r13],xmm2
	add rdi,rdx
	
	loop _SSE2_7b_a

_SSE2_7b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_7b_c
	
	movd xmm1,dword ptr[r10+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r9+4*rax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	movd xmm2,dword ptr[r8+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r11+4*rax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	

	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	movq xmm0,qword ptr[rsi+8*rax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,3
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm1,xmm0     		;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[rdi],xmm1
		
_SSE2_7b_c:		
	movdqa xmm7,XMMWORD ptr[rsp+16]
	movdqa xmm6,XMMWORD ptr[rsp]
	add rsp,32
	pop r13
	pop r12	
	pop rdi
	pop rsi
	pop rbp

	ret		

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_3b endp


;JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9
	
JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b proc public frame	

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push r12
	.pushreg r12
	push r13
	.pushreg r13
	sub rsp,32
	.allocstack 32
	movdqa XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqa XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog
	
	pxor xmm7,xmm7
	
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov rdi,dst
	mov r11,src2_v
	xor rcx,rcx
	mov rdx,32
	mov r12,2
	mov r13,16
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_2
	movdqa xmm5,XMMWORD ptr uw_3
	
	xor rax,rax
	shr ecx,1
	jz short _SSE2_4b_b

_SSE2_4b_a:
	movq xmm0,qword ptr[r10+4*rax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[r9+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	movq xmm2,qword ptr[r8+4*rax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[r11+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	movdqa xmm4,XMMWORD ptr[rsi+8*rax]		;YYYYYYYYYYYYYYYY
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,2
	psraw xmm1,2
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm2,xmm4
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add rax,r12
	punpcklbw xmm2,xmm0				;VYUYVYUYVYUYVYUY
	punpckhbw xmm4,xmm0				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[rdi],xmm2
	movdqa XMMWORD ptr[rdi+r13],xmm4
	add rdi,rdx
	
	loop _SSE2_4b_a

_SSE2_4b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_4b_c
	
	movd xmm1,dword ptr[r10+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r9+4*rax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	movd xmm2,dword ptr[r8+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r11+4*rax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	

	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	movq xmm0,qword ptr[rsi+8*rax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,2
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm0,xmm1     		;VYUYVYUYVYUYVYUY
	
	movdqa XMMWORD ptr[rdi],xmm0
	
_SSE2_4b_c:		
	movdqa xmm7,XMMWORD ptr[rsp+16]
	movdqa xmm6,XMMWORD ptr[rsp]
	add rsp,32
	pop r13
	pop r12
	pop rdi
	pop rsi
	pop rbp

	ret	

JPSDR_AutoYUY2_Convert420_to_YUY2_SSE2_4b endp


;JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b proc src_y:dword,src1_u:dword,src2_u:dword,src1_v:dword,src2_v:dword,dst:dword,w:dword
; src_y = rcx
; src1_u = rdx
; src2_u = r8
; src1_v = r9
	
JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b proc public frame	

src2_v equ qword ptr[rbp+48]
dst equ qword ptr[rbp+56]
w equ dword ptr[rbp+64]

	push rbp
	.pushreg rbp
	mov rbp,rsp
	push rsi
	.pushreg rsi
	push rdi
	.pushreg rdi
	push r12
	.pushreg r12
	push r13
	.pushreg r13	
	sub rsp,32
	.allocstack 32
	movdqa XMMWORD ptr[rsp],xmm6
	.savexmm128 xmm6,0
	movdqa XMMWORD ptr[rsp+16],xmm7
	.savexmm128 xmm7,16
	.endprolog
	
	pxor xmm7,xmm7
	
	mov rsi,rcx		; rsi = src_y
	mov r10,rdx		; r10=src1_u
	mov rdi,dst
	mov r11,src2_v
	xor rcx,rcx
	mov rdx,32
	mov r12,2
	mov r13,16
	mov ecx,w
	
	movdqa xmm6,XMMWORD ptr uw_2
	movdqa xmm5,XMMWORD ptr uw_3
	
	xor rax,rax
	shr ecx,1
	jz short _SSE2_8b_b

_SSE2_8b_a:
	movq xmm0,qword ptr[r10+4*rax]		;00000000UUUUUUUU
	movq xmm1,qword ptr[r9+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm0,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm1,xmm7				;0V0V0V0V0V0V0V0V
	movq xmm2,qword ptr[r8+4*rax]		;00000000UUUUUUUU
	movq xmm3,qword ptr[r11+4*rax]		;00000000VVVVVVVV
	punpcklbw xmm2,xmm7				;0U0U0U0U0U0U0U0U
	punpcklbw xmm3,xmm7				;0V0V0V0V0V0V0V0V
	
	pmullw xmm0,xmm5
	pmullw xmm1,xmm5	
	paddsw xmm2,xmm6
	paddsw xmm3,xmm6
	paddsw xmm0,xmm2
	paddsw xmm1,xmm3
	psraw xmm0,2
	psraw xmm1,2
	packuswb xmm0,xmm7				;00000000UUUUUUUU
	packuswb xmm1,xmm7				;00000000VVVVVVVV
	movdqa xmm4,XMMWORD ptr[rsi+8*rax]		;YYYYYYYYYYYYYYYY	
	punpcklbw xmm0,xmm1     		;VUVUVUVUVUVUVUVU
	add rax,r12
	movdqa xmm2,xmm0
	punpcklbw xmm0,xmm4				;VYUYVYUYVYUYVYUY
	punpckhbw xmm2,xmm4				;VYUYVYUYVYUYVYUY
	movdqa XMMWORD ptr[rdi],xmm0
	movdqa XMMWORD ptr[rdi+r13],xmm2
	add rdi,rdx
	
	loop _SSE2_8b_a

_SSE2_8b_b:	
	mov ecx,w
	and ecx,1
	jz short _SSE2_8b_c
	
	movd xmm1,dword ptr[r10+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r9+4*rax]		;000000000000VVVV
	punpcklbw xmm1,xmm0				;00000000VUVUVUVU
	punpcklbw xmm1,xmm7				;0V0U0V0U0V0U0V0U
	movd xmm2,dword ptr[r8+4*rax]		;000000000000UUUU
	movd xmm0,dword ptr[r11+4*rax]		;000000000000VVVV
	punpcklbw xmm2,xmm0				;00000000VUVUVUVU
	punpcklbw xmm2,xmm7				;0V0U0V0U0V0U0V0U	

	pmullw xmm1,xmm5
	paddsw xmm2,xmm6
	movq xmm0,qword ptr[rsi+8*rax]		;00000000YYYYYYYY
	paddsw xmm1,xmm2
	psraw xmm1,2
	packuswb xmm1,xmm7				;00000000VUVUVUVU
	punpcklbw xmm1,xmm0     		;YVYUYVYUYVYUYVYU
	
	movdqa XMMWORD ptr[rdi],xmm1	
		
_SSE2_8b_c:		
	movdqa xmm7,XMMWORD ptr[rsp+16]
	movdqa xmm6,XMMWORD ptr[rsp]
	add rsp,32
	pop r13
	pop r12	
	pop rdi
	pop rsi
	pop rbp

	ret	

JPSDR_AutoYUY2_Convert420_to_UYVY_SSE2_4b endp


;JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b proc src1:dword,src2:dword,dst:dword,w:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d
JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b proc public frame

	.endprolog
		
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	xor rax,rax
	
	mov ecx,r9d
	or ecx,ecx
	jz short SSE2_2b_d
	
	mov r11,16
SSE2_2b_a:
	movdqa xmm0,XMMWORD ptr[rdx+rax]
	movdqa xmm1,XMMWORD ptr[r10+rax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	pavgb xmm0,xmm1
	pxor xmm0,xmm3
	pavgb xmm0,xmm2

	movdqa XMMWORD ptr[r8+rax],xmm0
	add rax,r11
	loop SSE2_2b_a
		
SSE2_2b_d:		
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_2b endp


;JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b proc src1:dword,src2:dword,dst:dword,w:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d
	
JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b proc public frame	
	
	.endprolog
	
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	xor rax,rax
	
	mov ecx,r9d
	or ecx,ecx
	jz short SSE2_3b_d
		
	mov r11,16
SSE2_3b_a:
	movdqa xmm0,XMMWORD ptr[r10+rax]
	movdqa xmm1,XMMWORD ptr[rdx+rax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm1,xmm0
	pavgb xmm1,xmm0
	pxor xmm1,xmm3
	pavgb xmm1,xmm2
	
	movdqa XMMWORD ptr[r8+rax],xmm1
	add rax,r11
	loop SSE2_3b_a
	
SSE2_3b_d:	
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_3b endp


;JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b proc src1:dword,src2:dword,dst:dword,w:dword
; src1 = rcx
; src2 = rdx
; dst = r8
; w = r9d

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b proc public frame

	.endprolog
		
	pcmpeqb xmm3,xmm3
	
	mov r10,rcx				; r10=src1
	xor rcx,rcx
	xor rax,rax
	
	mov ecx,r9d
	or ecx,ecx
	jz short SSE2_4b_b
	
	mov r11,16
SSE2_4b_a:
	movdqa xmm0,XMMWORD ptr[r10+rax]
	movdqa xmm1,XMMWORD ptr[rdx+rax]
	movdqa xmm2,xmm0
	pxor xmm0,xmm3
	pxor xmm1,xmm3
	pavgb xmm0,xmm1
	pxor xmm0,xmm3
	pavgb xmm0,xmm2
	
	movdqa XMMWORD ptr[r8+rax],xmm0
	add rax,r11
	loop SSE2_4b_a
		
SSE2_4b_b:		
	ret

JPSDR_AutoYUY2_Convert420_to_Planar422_SSE2_4b endp

end





