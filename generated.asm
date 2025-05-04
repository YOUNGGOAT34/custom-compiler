section .text
	global _start
_start:
	mov rax,3
	push rax
	mov rax,3
	push rax
	pop rdi
	pop rax
	imul rax,rdi
	push rax
	mov rax,5
	push rax
	mov rax,6
	push rax
	pop rdi
	pop rax
	imul rax,rdi
	push rax
	mov rax,3
	push rax
	pop rdi
	pop rax
idiv rdi
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rdi
	mov rax, 60
	syscall
