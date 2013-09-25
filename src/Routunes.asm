BITS 32

GLOBAL rsqrt_asm
GLOBAL shade_asm
GLOBAL noise_asm

SEGMENT .data align=16
half		dd	0.5
one_half 	dd 1.5
seed		dd	34569012h

SEGMENT .text align=16

;------------------------------------

%define dst_buffer 		[ebp+08]
%define dst_buffer_size	[ebp+12]
%define shift 			[ebp+16]

shade_asm:
	push ebp
	mov ebp,esp
	pusha
	
	mov ecx, dst_buffer_size
	mov ebx, dst_buffer
	mov dl, shift
nxb1:
	mov eax,[ebx]
	sub al, dl
	jnc nozero1
	xor al,al
nozero1:		
	sub ah, dl
	jnc nozero2
	xor ah,ah
nozero2:
	ror	eax, 16

	sub al, dl
	jnc nozero3
	xor al,al
nozero3:
	ror	eax, 16
		
	mov [ebx],eax
	inc ebx
	inc ebx
	inc ebx
	inc ebx
	
	loop nxb1	
	
	popa
	pop	ebp
	xor eax,eax
	ret	

;------------------------------------

%define dst_buffer 		[ebp+08]
%define dst_buffer_size	[ebp+12]
%define disp 			[ebp+16]

noise_asm:
	push ebp
	mov ebp,esp
	pusha
	
	mov ecx, dst_buffer_size
	mov ebx, dst_buffer
next_pixel_noise:
	;randomize disp
	mov eax, 214013
	mul dword [seed]
	add eax, 2531011
	mov dword [seed], eax
	xor edx,edx
	div dword disp
	;--------------
	mov eax,[ebx]
	sub al, dl
	jnc nozero_noise_1
	xor al,al
nozero_noise_1:
	sub ah, dl
	jnc nozero_noise_2
	xor ah,ah
nozero_noise_2:
	ror	eax, 16
	sub al, dl
	jnc nozero_noise_3
	xor al,al
nozero_noise_3:
	ror	eax, 16
		
	mov [ebx],eax
	inc ebx
	inc ebx
	inc ebx
	inc ebx
	
	loop next_pixel_noise
	
	popa
	pop	ebp
	xor eax,eax
	ret		

;------------------------------------

%define x_val 	[esp+08h]

rsqrt_asm:
	push ebx
	fld  dword x_val
	fmul dword [half]
	mov ebx, x_val
	shr ebx,1
	mov eax, 5f375a86h
	sub eax, ebx
	mov x_val, eax
		
	fld  dword x_val
	fmul st0
	fmulp
	fsubr dword [one_half]
	fmul dword x_val
	
	xor eax, eax
	pop ebx
	ret
	
;------------------------------------

%define x_val 	[esp+08h]

rsqrt_asm_prec:
	push ebx
	fld  dword x_val
	fmul dword [half]
	fld  st0
	
	mov ebx, x_val
	shr ebx,1
	mov eax, 5f375a86h
	sub eax, ebx
	mov x_val, eax
		
	fld  dword x_val
	fmul st0
	fmulp
	fsubr dword [one_half]
	fmul dword x_val
	fst  dword x_val

	fmul st0
	fmulp
	fsubr dword [one_half]
	fmul dword x_val

	xor eax, eax
	pop ebx
	ret
end
