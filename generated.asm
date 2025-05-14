section .data
	a dq 0
section .text
	global _start
_start:
	mov rax,[a]
	push rax
	mov rax,3
	push rax
	pop rbx
	mov [a],rbx
	pop rdi
	pop rax
	push rax
	mov rax, 1
	mov rbx, 2
	cmp rax,rbx
	jle .Lend_if0
	mov rax,[a]
	push rax
	mov rax,4
	push rax
	mov rax,2
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
.Lend_if0:
	mov rax,[a]
	push rax
	pop rdi
	mov rax, 60
	syscall
