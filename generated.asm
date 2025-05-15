section .data
	d dq 0
section .text
	global _start
_start:
	mov rax,[d]
	push rax
	mov rax,1
	push rax
	pop rbx
	mov [d],rbx
	pop rdi
	pop rax
	push rax
	mov rax, 1
	mov rbx, 2
	cmp rax,rbx
	jle ._else0
	mov rax,[d]
	push rax
	mov rax,3
	push rax
	pop rbx
	mov [d],rbx
	pop rdi
	pop rax
	push rax
	jmp .end_if1
._else0:
	mov rax,[d]
	push rax
	mov rax,2
	push rax
	pop rbx
	mov [d],rbx
	pop rdi
	pop rax
	push rax
.end_if1:
	mov rax,[d]
	push rax
	pop rdi
	mov rax, 60
	syscall
