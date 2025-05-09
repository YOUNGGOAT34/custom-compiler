section .data
	a dq 0
	b dq 0
	ckk dq 0
section .text
	global _start
_start:
	mov rax,[a]
	push rax
	mov rax,3
	push rax
	mov rax,1
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rbx
	mov [a],rbx
	pop rdi
	pop rax
	push rax
	mov rax,[b]
	push rax
	mov rax,4
	push rax
	pop rbx
	mov [b],rbx
	pop rdi
	pop rax
	push rax
	mov rax,[ckk]
	push rax
	mov rax,6
	push rax
	pop rbx
	mov [ckk],rbx
	pop rdi
	pop rax
	push rax
	mov rax,[b]
	push rax
	mov rax,[ckk]
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rdi
	mov rax, 60
	syscall
