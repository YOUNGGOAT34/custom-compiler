section .data
section .text
	global _start
_start:
	jmp main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	jmp .after0
.msg0:db 72,101,108,108,111,10
.after0:
.len0 equ $-.msg0
	mov rax,1
	mov rdi,1
	lea rsi,[rel .msg0]
	mov rdx,.len0
	syscall
	jmp .after1
.msg1:db 87,111,114,108,100,10
.after1:
.len1 equ $-.msg1
	mov rax,1
	mov rdi,1
	lea rsi,[rel .msg1]
	mov rdx,.len1
	syscall
	mov rdi,1
	mov rax, 60
	mov rsp, rbp
	pop rbp
	syscall
