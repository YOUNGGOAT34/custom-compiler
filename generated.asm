section .data
buffer: TIMES 21 dB 0
section .text
	global _start
_start:
	jmp main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 16
	mov DWORD[rbp-4],3
	mov DWORD[rbp-8],4
	INC DWORD[rbp-4]
	mov eax,DWORD[rbp-4]
	mov DWORD[rbp-12],eax
	jmp .after0
.msg0: db 107,58
.len0: equ $-.msg0
.after0:
	mov rax,1
	mov rdi,1
	lea rsi,[rel .msg0]
	mov rdx,.len0
	syscall
	mov edx,DWORD[rbp-12]
	call print_int
	jmp .after1
.msg1: db 10
.len1 equ $-.msg1
.after1:
	mov rax, 1
	mov rdi, 1
	lea rsi, [rel .msg1]
	mov rdx, .len1
	syscall
	mov eax,DWORD[rbp-8]
	mov ebx,DWORD 2
	add eax,ebx
	push rax
	mov ebx,DWORD[rbp-12]
	pop rax
	add eax,ebx
	push rax
	pop rdi
	mov rax, 60
	mov rsp, rbp
	pop rbp
	syscall
print_int:
	push rbp
	mov rbp, rsp
	mov rax,rdx
	mov rsi,buffer+20
	mov rbx,10
	xor rcx,rcx
	test rax,rax
	jns .loop
	neg rax
	mov rcx,1
.loop:
	xor rdx,rdx
	idiv rbx
	add dl,'0'
	dec rsi
	mov [rsi],dl
	test rax,rax
	jnz .loop
	cmp rcx,1
	jne .print
	dec rsi
	mov byte[rsi],'-'
.print: 
	mov rax,1
	 mov rdi,1
	mov rdx,buffer+20
	sub rdx,rsi
	syscall
	mov rdi,buffer
	mov rcx,21
	mov al,0
	rep stosb
	mov rsp,rbp
	pop rbp
	ret
	print_char:
	push rbp
	mov rbp,rsp
	sub rbp,1
	mov [rbp],dl
	lea rsi,[rbp]
	mov rax,1
	mov rdx,1
	mov rdi,1
	syscall
	mov rbp,rsp
	pop rbp
	ret
